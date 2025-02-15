#pragma once

#include "Loader/PalMonsterModLoader.h"
#include "Loader/PalHumanModLoader.h"
#include "Loader/PalLanguageModLoader.h"
#include "Loader/PalItemModLoader.h"
#include "Loader/PalSkinModLoader.h"
#include "Loader/PalAppearanceModLoader.h"
#include "Loader/PalBuildingModLoader.h"
#include "Loader/PalSpawnerModLoader.h"
#include "Loader/PalRawTableLoader.h"
#include "Loader/PalBlueprintModLoader.h"

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
		PalSpawnerModLoader SpawnerModLoader;
		PalRawTableLoader RawTableLoader;
		PalBlueprintModLoader BlueprintModLoader;

        void InitializeBlueprintModLoader();

		void Load();

		void LoadLanguageMods(const std::filesystem::path& path);

		void LoadPalMods(const std::filesystem::path& path);

		void LoadItemMods(const std::filesystem::path& path);

		void LoadSkinMods(const std::filesystem::path& path);

		void LoadSpawnerMods(const std::filesystem::path& path);

		void LoadBuildingMods(const std::filesystem::path& path);

		void LoadRawTables(const std::filesystem::path& path);

		void LoadBlueprintMods(const std::filesystem::path& path);

		void LoadAppearanceMods(const std::filesystem::path& path);
	};
}