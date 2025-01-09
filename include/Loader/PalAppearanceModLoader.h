#pragma once

#include "Unreal/NameTypes.hpp"
#include "Loader/PalModLoaderBase.h"
#include "Unreal/Core/Math/UnrealMathUtility.hpp"

namespace RC::Unreal {
	class UObject;
}

namespace UECustom {
	class UDataAsset;
	class UDataTable;
}

namespace Palworld {
	class PalAppearanceModLoader : public PalModLoaderBase {
	public:
		PalAppearanceModLoader();

		~PalAppearanceModLoader();

		void Initialize();

		virtual void Load(const nlohmann::json& Data) override final;
	private:
		UECustom::UDataTable* m_hairTable;
		UECustom::UDataTable* m_headTable;
		UECustom::UDataTable* m_eyesTable;
		UECustom::UDataTable* m_bodyTable;
		UECustom::UDataTable* m_presetTable;
		UECustom::UDataTable* m_colorPresetTable;
		UECustom::UDataTable* m_equipmentTable;

		void Add(const RC::Unreal::FName& RowId, UECustom::UDataTable* DataTable, const nlohmann::json& Data, const std::vector<std::string>& RequiredFields);

		void AddColorPreset(const RC::Unreal::FName& ColorPresetId, const nlohmann::json& Data);

		// This is its own function due to it using TMap properties which I haven't figured out yet how to dynamically set
		void AddEquipment(const RC::Unreal::FName& EquipmentId, const nlohmann::json& Data);
	};
}