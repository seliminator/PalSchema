#pragma once

#include <vector>
#include <functional>
#include "Loader/PalMonsterModLoader.h"
#include "Loader/PalHumanModLoader.h"
#include "Loader/PalLanguageModLoader.h"
#include "Loader/PalItemModLoader.h"
#include "Loader/PalSkinModLoader.h"
#include "Loader/PalAppearanceModLoader.h"
#include "Loader/PalBuildingModLoader.h"
#include "Loader/PalRawTableLoader.h"
#include "Loader/PalBlueprintModLoader.h"

namespace UECustom {
    class UDataTable;
}

namespace Palworld {
	class PalMainLoader {
	public:
		void PreInitialize();

		void Initialize();
	private:
		PalLanguageModLoader LanguageModLoader;
		PalMonsterModLoader MonsterModLoader;
		PalHumanModLoader HumanModLoader;
		PalItemModLoader ItemModLoader;
		PalSkinModLoader SkinModLoader;
		PalAppearanceModLoader AppearanceModLoader;
		PalBuildingModLoader BuildingModLoader;
		PalRawTableLoader RawTableLoader;
		PalBlueprintModLoader BlueprintModLoader;

		void Load();

		void LoadLanguageMods(const std::filesystem::path& path);

		void LoadPalMods(const std::filesystem::path& path);

		void LoadItemMods(const std::filesystem::path& path);

		void LoadSkinMods(const std::filesystem::path& path);

		void LoadBuildingMods(const std::filesystem::path& path);

		void LoadRawTables(const std::filesystem::path& path);

		void LoadBlueprintMods(const std::filesystem::path& path);

		void LoadAppearanceMods(const std::filesystem::path& path);

        void IterateModsFolder(const std::function<void(const std::filesystem::directory_entry&)>& callback);

        void ParseJsonFileInPath(const std::filesystem::path& path, const std::function<void(const nlohmann::json&)>& callback);
    private:
        static void HandleDataTableChanged(UECustom::UDataTable* This, RC::Unreal::FName param_1);

        static inline std::vector<std::function<void(UECustom::UDataTable*)>> HandleDataTableChangedCallbacks;

        static inline SafetyHookInline HandleDataTableChanged_Hook;
	};
}