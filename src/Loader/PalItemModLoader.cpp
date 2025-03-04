#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UClass.hpp"
#include "Unreal/UScriptStruct.hpp"
#include "SDK/Classes/UDataTable.h"
#include "SDK/Classes/PalStaticArmorItemData.h"
#include "SDK/Classes/PalStaticConsumeItemData.h"
#include "SDK/Classes/PalStaticWeaponItemData.h"
#include "SDK/Classes/PalDynamicWeaponItemDataBase.h"
#include "SDK/Classes/PalDynamicArmorItemDataBase.h"
#include "SDK/Structs/FPalCharacterIconDataRow.h"
#include "Helpers/String.hpp"
#include "Utility/DataTableHelper.h"
#include "Utility/Logging.h"
#include "Loader/PalItemModLoader.h"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
	PalItemModLoader::PalItemModLoader() : PalModLoaderBase("items") {}

	PalItemModLoader::~PalItemModLoader() {}

	void PalItemModLoader::Initialize()
	{
		m_itemDataAsset = UObjectGlobals::StaticFindObject<UPalStaticItemDataAsset*>(nullptr, nullptr,
			STR("/Game/Pal/DataAsset/Item/DA_StaticItemDataAsset.DA_StaticItemDataAsset"));

		m_itemRecipeTable = UObjectGlobals::StaticFindObject<UECustom::UDataTable*>(nullptr, nullptr,
			STR("/Game/Pal/DataTable/Item/DT_ItemRecipeDataTable.DT_ItemRecipeDataTable"));

		m_nameTranslationTable = UObjectGlobals::StaticFindObject<UECustom::UDataTable*>(nullptr, nullptr,
			STR("/Game/Pal/DataTable/Text/DT_ItemNameText.DT_ItemNameText"));

		m_descriptionTranslationTable = UObjectGlobals::StaticFindObject<UECustom::UDataTable*>(nullptr, nullptr,
			STR("/Game/Pal/DataTable/Text/DT_ItemDescriptionText.DT_ItemDescriptionText"));
	}

	void PalItemModLoader::Load(const nlohmann::json& Data)
	{
		for (auto& [Key, Value] : Data.items())
		{
			auto ItemId = FName(RC::to_generic_string(Key), FNAME_Add);
			if (m_itemDataAsset->StaticItemDataMap.Contains(ItemId))
			{
				auto Row = m_itemDataAsset->StaticItemDataMap.FindRef(ItemId);
				Edit(ItemId, Row, Value);
			}
			else
			{
				Add(ItemId, Value);
			}
		}
	}

	void PalItemModLoader::Add(const RC::Unreal::FName& ItemId, const nlohmann::json& Data)
	{
		if (ItemId == NAME_None)
		{
			throw std::runtime_error("ID was set to None");
		}

		if (!Data.contains("Type"))
		{
			throw std::runtime_error(std::format("You must supply a Type field in {} when adding new items", RC::to_string(ItemId.ToString())));
		}

		if (!Data.at("Type").is_string())
		{
			throw std::runtime_error(std::format("Type must be a string in {}", RC::to_string(ItemId.ToString())));
		}

		auto Type = Data.at("Type").get<std::string>();

		UClass* DatabaseClass = nullptr;
		UClass* DynamicDatabaseClass = nullptr;
		if (Type == "Armor" || Type == "PalStaticArmorItemData")
		{
			DatabaseClass = UPalStaticArmorItemData::StaticClass();
			DynamicDatabaseClass = UPalDynamicArmorItemDataBase::StaticClass();
		}
		else if (Type == "Weapon" || Type == "PalStaticWeaponItemData")
		{
			DatabaseClass = UPalStaticWeaponItemData::StaticClass();
			DynamicDatabaseClass = UPalDynamicWeaponItemDataBase::StaticClass();
		}
		else if (Type == "Consumable" || Type == "PalStaticConsumeItemData")
		{
			DatabaseClass = UPalStaticConsumeItemData::StaticClass();
		}
		else if (Type == "Generic" || Type == "PalStaticItemDataBase")
		{
			DatabaseClass = UPalStaticItemDataBase::StaticClass();
		}
		else
		{
			throw std::runtime_error(std::format("Type {} in {} isn't supported, must be Armor, Weapon, Consumable or Generic", Type, RC::to_string(ItemId.ToString())));
		}

		if (!DatabaseClass->GetPropertyByNameInChain(STR("ID")))
		{
			throw std::runtime_error("Property 'ID' has changed in DA_StaticItemDataAsset. Update to Pal Schema is needed.");
		}

		if (!DatabaseClass->GetPropertyByNameInChain(STR("DynamicItemDataClass")))
		{
			throw std::runtime_error("Property 'DynamicItemDataClass' has changed in DA_StaticItemDataAsset. Update to Pal Schema is needed.");
		}

		FStaticConstructObjectParameters ConstructParams(DatabaseClass, m_itemDataAsset);
		ConstructParams.Name = NAME_None;

		auto Item = UObjectGlobals::StaticConstructObject<UPalStaticItemDataBase*>(ConstructParams);

		auto IDProperty = Item->GetValuePtrByPropertyNameInChain<FName>(STR("ID"));
		if (IDProperty)
		{
			*IDProperty = ItemId;
		}

		auto DynamicItemDataClassProperty = Item->GetValuePtrByPropertyNameInChain<UClass*>(STR("DynamicItemDataClass"));
		if (DynamicItemDataClassProperty)
		{
			*DynamicItemDataClassProperty = DynamicDatabaseClass;
		}

		for (auto& Property : DatabaseClass->ForEachPropertyInChain())
		{
			auto PropertyName = RC::to_string(Property->GetName());
			if (PropertyName == "DynamicItemDataClass")
			{
				// We've already set this earlier so we skip it.
				continue;
			}
			if (Data.contains(PropertyName))
			{
				DataTableHelper::CopyJsonValueToTableRow(reinterpret_cast<uint8_t*>(Item), Property, Data.at(PropertyName));
			}
		}
		
		if (Data.contains("Recipe"))
		{
			AddRecipe(ItemId, Data.at("Recipe"));
		}

		AddTranslations(ItemId, Data);

		m_itemDataAsset->StaticItemDataMap.Add(ItemId, Item);

		PS::Log<RC::LogLevel::Normal>(STR("Added new Item '{}'\n"), ItemId.ToString());
	}

	void PalItemModLoader::Edit(const RC::Unreal::FName& ItemId, UPalStaticItemDataBase* Item, const nlohmann::json& Data)
	{
		for (auto& Property : Item->GetClassPrivate()->ForEachPropertyInChain())
		{
			auto PropertyName = RC::to_string(Property->GetName());
			if (PropertyName == "DynamicItemDataClass")
			{
				continue;
			}
			if (PropertyName == "ID")
			{
				// Editing the ID is a bad idea, hence we skip it.
				continue;
			}
			if (Data.contains(PropertyName))
			{
				DataTableHelper::CopyJsonValueToTableRow(reinterpret_cast<uint8_t*>(Item), Property, Data.at(PropertyName));
			}
		}

		if (Data.contains("Recipe"))
		{
			EditRecipe(ItemId, Data.at("Recipe"));
		}

		EditTranslations(ItemId, Data);
	}

	void PalItemModLoader::AddRecipe(const RC::Unreal::FName& ItemId, const nlohmann::json& Recipe)
	{
		auto RowStruct = m_itemRecipeTable->GetRowStruct().UnderlyingObjectPointer;

		auto ItemRecipeData = FMemory::Malloc(RowStruct->GetStructureSize());
		RowStruct->InitializeStruct(ItemRecipeData);

		for (auto& [property_name, property_value] : Recipe.items())
		{
			auto KeyName = RC::to_generic_string(property_name);

			if (KeyName == STR("Product_Id"))
			{
				// We will set this later based on the key used for the json object, so we skip it for now.
				continue;
			}

			if (KeyName == STR("Editor_RowNameHash"))
			{
				// We don't need to change this due to it being editor related, skip.
				continue;
			}

			auto Property = RowStruct->GetPropertyByName(KeyName.c_str());
			if (Property)
			{
				try
				{
					DataTableHelper::CopyJsonValueToTableRow(ItemRecipeData, Property, property_value);
				}
				catch (const std::exception& e)
				{
					FMemory::Free(ItemRecipeData);
					throw std::runtime_error(e.what());
				}
			}
		}

		auto ProductIdProperty = RowStruct->GetPropertyByName(STR("Product_Id"));
		if (ProductIdProperty)
		{
			FMemory::Memcpy(ProductIdProperty->ContainerPtrToValuePtr<void>(ItemRecipeData), &ItemId, sizeof(FName));
		}

		m_itemRecipeTable->AddRow(ItemId, *reinterpret_cast<UECustom::FTableRowBase*>(ItemRecipeData));
	}

	void PalItemModLoader::EditRecipe(const RC::Unreal::FName& ItemId, const nlohmann::json& Recipe)
	{
		auto RowStruct = m_itemRecipeTable->GetRowStruct().UnderlyingObjectPointer;

		auto RecipeRow = m_itemRecipeTable->FindRowUnchecked(ItemId);
		if (!RecipeRow)
		{
			throw std::runtime_error(std::format("Row for Recipe '{}' doesn't exist", RC::to_string(ItemId.ToString())));
		}

		for (auto& [property_name, property_value] : Recipe.items())
		{
			auto KeyName = RC::to_generic_string(property_name);
			if (KeyName == STR("Editor_RowNameHash"))
			{
				// We don't need to change this due to it being editor related, skip.
				continue;
			}

			auto Property = RowStruct->GetPropertyByName(KeyName.c_str());
			if (Property)
			{
				DataTableHelper::CopyJsonValueToTableRow(RecipeRow, Property, property_value);
			}
		}
	}

	void PalItemModLoader::AddTranslations(const RC::Unreal::FName& ItemId, const nlohmann::json& Data)
	{
		if (Data.contains("Name"))
		{
			auto FixedItemId = std::format(STR("ITEM_NAME_{}"), ItemId.ToString());
			auto TranslationRowStruct = m_nameTranslationTable->GetRowStruct().UnderlyingObjectPointer;
			auto TextProperty = TranslationRowStruct->GetPropertyByName(STR("TextData"));
			if (TextProperty)
			{
				auto TranslationRowData = FMemory::Malloc(TranslationRowStruct->GetStructureSize());
				TranslationRowStruct->InitializeStruct(TranslationRowData);

				try
				{
					DataTableHelper::CopyJsonValueToTableRow(TranslationRowData, TextProperty, Data.at("Name"));
				}
				catch (const std::exception& e)
				{
					FMemory::Free(TranslationRowData);
					throw std::runtime_error(e.what());
				}

				m_nameTranslationTable->AddRow(FName(FixedItemId, FNAME_Add), *reinterpret_cast<UECustom::FTableRowBase*>(TranslationRowData));
			}
		}

		if (Data.contains("Description"))
		{
			auto FixedItemId = std::format(STR("ITEM_DESC_{}"), ItemId.ToString());
			auto TranslationRowStruct = m_descriptionTranslationTable->GetRowStruct().UnderlyingObjectPointer;
			auto TextProperty = TranslationRowStruct->GetPropertyByName(STR("TextData"));
			if (TextProperty)
			{
				auto TranslationRowData = FMemory::Malloc(TranslationRowStruct->GetStructureSize());
				TranslationRowStruct->InitializeStruct(TranslationRowData);

				try
				{
					DataTableHelper::CopyJsonValueToTableRow(TranslationRowData, TextProperty, Data.at("Description"));
				}
				catch (const std::exception& e)
				{
					FMemory::Free(TranslationRowData);
					throw std::runtime_error(e.what());
				}

				m_descriptionTranslationTable->AddRow(FName(FixedItemId, FNAME_Add), *reinterpret_cast<UECustom::FTableRowBase*>(TranslationRowData));
			}
		}
	}

	void PalItemModLoader::EditTranslations(const RC::Unreal::FName& ItemId, const nlohmann::json& Data)
	{
		if (Data.contains("Name"))
		{
			auto FixedItemId = std::format(STR("ITEM_NAME_{}"), ItemId.ToString());
			auto TranslationRowStruct = m_nameTranslationTable->GetRowStruct().UnderlyingObjectPointer;
			auto TextProperty = TranslationRowStruct->GetPropertyByName(STR("TextData"));
			if (TextProperty)
			{
				auto Row = m_nameTranslationTable->FindRowUnchecked(FName(FixedItemId, FNAME_Add));
				if (Row)
				{
					DataTableHelper::CopyJsonValueToTableRow(Row, TextProperty, Data.at("Name"));
				}
			}
		}

		if (Data.contains("Description"))
		{
			auto FixedItemId = std::format(STR("ITEM_DESC_{}"), ItemId.ToString());
			auto TranslationRowStruct = m_nameTranslationTable->GetRowStruct().UnderlyingObjectPointer;
			auto TextProperty = TranslationRowStruct->GetPropertyByName(STR("TextData"));
			if (TextProperty)
			{
				auto Row = m_descriptionTranslationTable->FindRowUnchecked(FName(FixedItemId, FNAME_Add));
				if (Row)
				{
					DataTableHelper::CopyJsonValueToTableRow(Row, TextProperty, Data.at("Description"));
				}
			}
		}
	}
}