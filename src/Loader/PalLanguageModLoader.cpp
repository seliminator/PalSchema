#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UScriptStruct.hpp"
#include "SDK/Structs/FPalLocalizedTextData.h"
#include "SDK/Classes/UDataTable.h"
#include "SDK/Classes/KismetInternationalizationLibrary.h"
#include "Loader/PalLanguageModLoader.h"
#include "Utility/StringHelpers.h"
#include "Helpers/String.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
	PalLanguageModLoader::PalLanguageModLoader() {}

	PalLanguageModLoader::~PalLanguageModLoader() {}

	void PalLanguageModLoader::Load(const nlohmann::json& Data)
	{
		for (auto& [TableName, TableData] : Data.items())
		{
			for (auto& [RowId, RowValue] : TableData.items())
			{
				auto Table = GetTable(TableName);
				if (Table)
				{
					auto RowName = FName(RC::to_generic_string(RowId), FNAME_Add);
					auto Row = std::bit_cast<FPalLocalizedTextData*>(Table->FindRowUnchecked(RowName));
					if (Row)
					{
						Row->TextData = FText(RC::to_generic_string(RowValue.get<std::string>()));
					}
					else
					{
						FPalLocalizedTextData NewRow{};
						NewRow.TextData = FText(RC::to_generic_string(RowValue.get<std::string>()));
						Table->AddRow(RowName, NewRow);
					}
				}
			}
		}
	}

	void PalLanguageModLoader::Initialize()
	{
		auto language = Palworld::UKismetInternationalizationLibrary::GetCurrentLanguage();
		m_currentLanguage = RC::to_string(language.GetCharArray());

		std::vector<UObject*> Objects;
		UObjectGlobals::FindAllOf(TEXT("DataTable"), Objects);

		for (auto& Object : Objects)
		{
			auto DT = static_cast<UECustom::UDataTable*>(Object);
			auto RowStruct = DT->GetRowStruct();
			if (RowStruct.UnderlyingObjectPointer)
			{
				static auto NAME_PalLocalizedTextData = FName(TEXT("PalLocalizedTextData"), FNAME_Add);
				if (RowStruct.UnderlyingObjectPointer->GetNamePrivate() == NAME_PalLocalizedTextData)
				{
					auto Name = Object->GetNamePrivate().ToString();
					AddTable(RC::to_string(Name), DT);
				}
			}
		}
	}

	const std::string& PalLanguageModLoader::GetCurrentLanguage()
	{
		return m_currentLanguage;
	}

	void PalLanguageModLoader::Add(const std::string& TableKey, const std::string& RowId, const std::string& RowValue)
	{
		auto Table = GetTable(TableKey);
		if (Table)
		{
			auto NewRow = FPalLocalizedTextData{};
			auto RowName = FName(RC::to_generic_string(RowId), FNAME_Add);
			NewRow.TextData = FText(RC::to_generic_string(RowValue));
			Table->AddRow(RowName, NewRow);
		}
	}

	void PalLanguageModLoader::AddTable(const std::string& Id, UECustom::UDataTable* Table)
	{
		m_tables.emplace(Id, Table);
	}

	UECustom::UDataTable* PalLanguageModLoader::GetTable(const std::string& Id)
	{
		auto It = m_tables.find(Id);
		if (It != m_tables.end())
		{
			return It->second;
		}
		return nullptr;
	}
}