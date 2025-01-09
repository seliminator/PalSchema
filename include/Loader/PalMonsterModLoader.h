#pragma once

#include "Loader/PalModLoaderBase.h"
#include "Loader/PalLanguageModLoader.h"
#include "nlohmann/json.hpp"

namespace UECustom {
	class UDataTable;
}

namespace Palworld {
	class PalMonsterModLoader : public PalModLoaderBase {
	public:
		PalMonsterModLoader();

		~PalMonsterModLoader();

		void Initialize();
		
		virtual void Load(const nlohmann::json& json) override final;
	private:
		void Add(const RC::Unreal::FName& CharacterId, const nlohmann::json& properties);

		void Edit(uint8_t* TableRow, const RC::Unreal::FName& CharacterId, const nlohmann::json& properties);

		void AddIcon(const RC::Unreal::FName& CharacterId, const RC::StringType& IconPath);

		void AddBlueprint(const RC::Unreal::FName& CharacterId, const RC::StringType& BlueprintPath);

		void AddAbilities(const RC::Unreal::FName& CharacterId, const nlohmann::json& properties);

		void AddLoot(const RC::Unreal::FName& CharacterId, const nlohmann::json& properties);

		void AddTranslations(const RC::Unreal::FName& CharacterId, const nlohmann::json& Data);

		void EditTranslations(const RC::Unreal::FName& CharacterId, const nlohmann::json& Data);

		UECustom::UDataTable* m_dataTable;
		UECustom::UDataTable* m_iconDataTable;
		UECustom::UDataTable* m_palBpClassTable;
		UECustom::UDataTable* m_wazaMasterLevelTable;
		UECustom::UDataTable* m_palDropItemTable;
		UECustom::UDataTable* m_palNameTable;
		UECustom::UDataTable* m_palShortDescTable;
		UECustom::UDataTable* m_palLongDescTable;
	};
}