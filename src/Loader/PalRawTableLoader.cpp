#include "Unreal/UClass.hpp"
#include "SDK/Classes/UDataTable.h"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UScriptStruct.hpp"
#include "Unreal/FProperty.hpp"
#include "Unreal/NameTypes.hpp"
#include "Utility/DataTableHelper.h"
#include "Utility/Logging.h"
#include "Helpers/String.hpp"
#include "Loader/PalRawTableLoader.h"

using namespace RC;
using namespace RC::Unreal;

namespace fs = std::filesystem;

namespace Palworld {
	PalRawTableLoader::PalRawTableLoader() : PalModLoaderBase("raw") {}

	PalRawTableLoader::~PalRawTableLoader() {}

	void PalRawTableLoader::Initialize() {}

    void PalRawTableLoader::Apply(UECustom::UDataTable* Table)
    {
        auto Name = Table->GetNamePrivate().ToString();
        RC::StringType Suffix = STR("_Common");
        size_t Pos = Name.rfind(Suffix);

        if (Pos != RC::StringType::npos && Pos + Suffix.size() == Name.size()) {
            Name.erase(Pos);
        }

        auto It = m_tableDataMap.find(FName(Name, FNAME_Add));
        if (It != m_tableDataMap.end())
        {
            for (auto& Data : It->second)
            {
                for (auto& [row_key, row_data] : Data.items())
                {
                    auto RowStruct = Table->GetRowStruct().UnderlyingObjectPointer;

                    auto RowKey = FName(RC::to_generic_string(row_key), FNAME_Add);
                    auto Row = Table->FindRowUnchecked(RowKey);
                    if (Row)
                    {
                        try
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
                        catch (const std::exception& e)
                        {
                            PS::Log<LogLevel::Error>(STR("Failed to modify Row '{}' in {}: {}\n"), RowKey.ToString(), Name, RC::to_generic_string(e.what()));
                        }
                    }
                    else
                    {
                        auto NewRowData = FMemory::Malloc(RowStruct->GetStructureSize());
                        RowStruct->InitializeStruct(NewRowData);

                        try
                        {
                            for (auto Property : RowStruct->ForEachPropertyInChain())
                            {
                                auto PropertyName = RC::to_string(Property->GetName());
                                if (row_data.contains(PropertyName))
                                {
                                    DataTableHelper::CopyJsonValueToTableRow(NewRowData, Property, row_data.at(PropertyName));
                                }
                            }

                            Table->AddRow(RowKey, *reinterpret_cast<UECustom::FTableRowBase*>(NewRowData));
                        }
                        catch (const std::exception& e)
                        {
                            FMemory::Free(NewRowData);
                            PS::Log<LogLevel::Error>(STR("Failed to add Row '{}' in {}: {}\n"), RowKey.ToString(), Name, RC::to_generic_string(e.what()));
                        }
                    }
                }
            }
        }
    }

	void PalRawTableLoader::Load(const nlohmann::json& data)
	{
        for (auto& [table_key, table_data] : data.items())
        {
            auto TableKey = RC::to_generic_string(table_key);
            auto TableName = FName(TableKey, FNAME_Add);
            AddData(TableName, table_data);
        }
	}

    void PalRawTableLoader::AddData(const RC::Unreal::FName& TableName, const nlohmann::json& Data)
    {
        auto It = m_tableDataMap.find(TableName);
        if (It != m_tableDataMap.end())
        {
            It->second.push_back(Data);
        }
        else
        {
            std::vector<nlohmann::json> NewDataArray{
                Data
            };
            m_tableDataMap.emplace(TableName, NewDataArray);
        }
    }
}