#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UScriptStruct.hpp"
#include "Unreal/FProperty.hpp"
#include "Helpers/String.hpp"
#include "SDK/Classes/UDataTable.h"
#include "Utility/DataTableHelper.h"
#include "Loader/PalBuildingModLoader.h"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
	PalBuildingModLoader::PalBuildingModLoader() : PalModLoaderBase("buildings") {}

	PalBuildingModLoader::~PalBuildingModLoader() {}

	void PalBuildingModLoader::Initialize()
	{
		m_mapObjectAssignData = UObjectGlobals::StaticFindObject<UECustom::UDataTable*>(nullptr, nullptr,
			STR("/Game/Pal/DataTable/MapObject/DT_MapObjectAssignData.DT_MapObjectAssignData"));

		m_mapObjectFarmCrop = UObjectGlobals::StaticFindObject<UECustom::UDataTable*>(nullptr, nullptr,
			STR("/Game/Pal/DataTable/MapObject/DT_MapObjectFarmCrop.DT_MapObjectFarmCrop"));

		m_mapObjectItemProductDataTable = UObjectGlobals::StaticFindObject<UECustom::UDataTable*>(nullptr, nullptr,
			STR("/Game/Pal/DataTable/MapObject/DT_MapObjectItemProductDataTable.DT_MapObjectItemProductDataTable"));

		m_mapObjectMasterDataTable = UObjectGlobals::StaticFindObject<UECustom::UDataTable*>(nullptr, nullptr,
			STR("/Game/Pal/DataTable/MapObject/DT_MapObjectMasterDataTable.DT_MapObjectMasterDataTable"));

		m_buildObjectDataTable = UObjectGlobals::StaticFindObject<UECustom::UDataTable*>(nullptr, nullptr,
			STR("/Game/Pal/DataTable/MapObject/Building/DT_BuildObjectDataTable.DT_BuildObjectDataTable"));

		m_buildObjectIconDataTable = UObjectGlobals::StaticFindObject<UECustom::UDataTable*>(nullptr, nullptr,
			STR("/Game/Pal/DataTable/MapObject/Building/DT_BuildObjectIconDataTable.DT_BuildObjectIconDataTable"));
	}

	void PalBuildingModLoader::Load(const nlohmann::json& Data)
	{
		for (auto& [Key, Properties] : Data.items())
		{
			auto BuildingId = FName(RC::to_generic_string(Key), FNAME_Add);
			auto TableRow = m_mapObjectMasterDataTable->FindRowUnchecked(BuildingId);
			if (TableRow)
			{
				Edit(TableRow, BuildingId, Properties);
			}
			else
			{
				Add(BuildingId, Properties);
			}
		}
	}

	void PalBuildingModLoader::Add(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data)
	{
		if (!Data.contains("BlueprintClassName"))
		{
			throw std::runtime_error(RC::fmt("%S is missing the property 'BlueprintClassName'", BuildingId.ToString().c_str()));
		}

		if (!Data.contains("BlueprintClassSoft"))
		{
			throw std::runtime_error(RC::fmt("%S is missing the property 'BlueprintClassSoft'", BuildingId.ToString().c_str()));
		}

		if (!Data.contains("IconTexture"))
		{
			throw std::runtime_error(RC::fmt("%S is missing the property 'IconTexture'", BuildingId.ToString().c_str()));
		}

		auto MasterBuildingRowStruct = m_mapObjectMasterDataTable->GetRowStruct().UnderlyingObjectPointer;
		auto MasterBuildingRowData = FMemory::Malloc(MasterBuildingRowStruct->GetStructureSize());
		MasterBuildingRowStruct->InitializeStruct(MasterBuildingRowData);

		try
		{
			for (auto& Property : MasterBuildingRowStruct->ForEachProperty())
			{
				auto PropertyName = RC::to_string(Property->GetName());
				if (Data.contains(PropertyName))
				{
					DataTableHelper::CopyJsonValueToTableRow(MasterBuildingRowData, Property, Data.at(PropertyName));
				}
			}
		}
		catch (const std::exception&)
		{
			FMemory::Free(MasterBuildingRowData);
		}

		SetupIconData(BuildingId, Data);

		if (Data.contains("BuildingData"))
		{
			SetupBuildData(BuildingId, Data);
		}

		if (Data.contains("AssignData"))
		{
			SetupAssignData(BuildingId, Data);
		}

		if (Data.contains("CropData"))
		{
			SetupCropData(BuildingId, Data);
		}

		if (Data.contains("ItemProduceData"))
		{
			SetupItemProduceData(BuildingId, Data);
		}
	}

	void PalBuildingModLoader::Edit(uint8_t* ExistingRow, const RC::Unreal::FName& BuildingId, const nlohmann::json& Data)
	{

	}

	void PalBuildingModLoader::SetupBuildData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data)
	{

	}

	void PalBuildingModLoader::SetupIconData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data)
	{

	}

	void PalBuildingModLoader::SetupAssignData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data)
	{

	}

	void PalBuildingModLoader::SetupCropData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data)
	{

	}

	void PalBuildingModLoader::SetupItemProduceData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data)
	{

	}
}