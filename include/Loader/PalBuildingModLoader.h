#pragma once

#include "Loader/PalModLoaderBase.h"
#include "Unreal/NameTypes.hpp"

namespace RC::Unreal {
	class UObject;
}

namespace UECustom {
	class UDataAsset;
	class UDataTable;
}

namespace Palworld {
	class PalBuildingModLoader : public PalModLoaderBase {
	public:
		PalBuildingModLoader();

		~PalBuildingModLoader();

		void Initialize();

		virtual void Load(const nlohmann::json& json) override final;
	private:
		UECustom::UDataTable* m_mapObjectAssignData;
		UECustom::UDataTable* m_mapObjectFarmCrop;
		UECustom::UDataTable* m_mapObjectItemProductDataTable;
		UECustom::UDataTable* m_mapObjectMasterDataTable;
		UECustom::UDataTable* m_buildObjectDataTable;
		UECustom::UDataTable* m_buildObjectIconDataTable;

		void Add(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data);

		void Edit(uint8_t* ExistingRow, const RC::Unreal::FName& BuildingId, const nlohmann::json& Data);

		void SetupBuildData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data);

		void SetupIconData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data);

		void SetupAssignData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data);

		void SetupCropData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data);

		void SetupItemProduceData(const RC::Unreal::FName& BuildingId, const nlohmann::json& Data);
	};
}