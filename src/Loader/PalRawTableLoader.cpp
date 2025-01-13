#include "Unreal/AActor.hpp"
#include "Unreal/UClass.hpp"
#include "SDK/Classes/UDataTable.h"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UScriptStruct.hpp"
#include "Unreal/FProperty.hpp"
#include "Unreal/NameTypes.hpp"
#include "SDK/EnumCache.h"
#include "Utility/DataTableHelper.h"
#include "Helpers/String.hpp"
#include "Loader/PalRawTableLoader.h"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
	PalRawTableLoader::PalRawTableLoader() {}

	PalRawTableLoader::~PalRawTableLoader() {}

	void PalRawTableLoader::Initialize()
	{
		std::vector<UObject*> DataTables;
		UObjectGlobals::FindAllOf(STR("DataTable"), DataTables);
		for (auto& Table : DataTables)
		{
			auto TableName = Table->GetNamePrivate().ToString();
			m_cachedTables.emplace(TableName, reinterpret_cast<UECustom::UDataTable*>(Table));
		}
	}

	void PalRawTableLoader::Load(const nlohmann::json& Data)
	{
		for (auto& [table_key, table_data] : Data.items())
		{
			auto TableKey = RC::to_generic_string(table_key);
			auto Table = GetTable(TableKey);
			if (Table)
			{
				if (table_data.is_object())
				{
					auto RowStruct = Table->GetRowStruct().UnderlyingObjectPointer;

					for (auto& [row_key, row_data] : table_data.items())
					{
						auto RowKey = FName(RC::to_generic_string(row_key), FNAME_Add);
						auto Row = Table->FindRowUnchecked(RowKey);
						if (Row)
						{
							for (auto Property : RowStruct->ForEachPropertyInChain())
							{
								auto PropertyName = RC::to_string(Property->GetName());
								if (row_data.contains(PropertyName))
								{
									DataTableHelper::CopyJsonValueToTableRow(Row, Property, row_data.at(PropertyName));
								}
							}
						}
						else
						{
							auto NewRowData = FMemory::Malloc(RowStruct->GetStructureSize());
							RowStruct->InitializeStruct(NewRowData);

							for (auto Property : RowStruct->ForEachPropertyInChain())
							{
								auto PropertyName = RC::to_string(Property->GetName());
								if (row_data.contains(PropertyName))
								{
									try
									{
										DataTableHelper::CopyJsonValueToTableRow(NewRowData, Property, row_data.at(PropertyName));
									}
									catch (const std::exception& e)
									{
										FMemory::Free(NewRowData);
										throw std::runtime_error(e.what());
									}
								}
							}

							Table->AddRow(RowKey, *reinterpret_cast<UECustom::FTableRowBase*>(NewRowData));
						}
					}
				}
			}
		}
	}

	UECustom::UDataTable* PalRawTableLoader::GetTable(const RC::StringType& TableName)
	{
		auto TableIterator = m_cachedTables.find(TableName);
		if (TableIterator != m_cachedTables.end())
		{
			return TableIterator->second;
		}
		return nullptr;
	}
}