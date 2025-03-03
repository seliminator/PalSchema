#include <fstream>
#include <filesystem>
#include "Unreal/UClass.hpp"
#include "Unreal/Hooks.hpp"
#include "Helpers/String.hpp"
#include "Loader/PalMainLoader.h"
#include "Utility/Config.h"
#include "Utility/Logging.h"
#include "SDK/Classes/UDataTable.h"
#include "SDK/PalSignatures.h"

using namespace RC;
using namespace RC::Unreal;

namespace fs = std::filesystem;

namespace Palworld {
    void PalMainLoader::PreInitialize()
    {
        IterateModsFolder([&](const fs::directory_entry& modFolder) {
            auto rawFolder = modFolder.path() / "raw";
            LoadRawTables(rawFolder);
        });

        if (PS::PSConfig::IsExperimentalBlueprintSupportEnabled())
        {
            PS::Log<RC::LogLevel::Normal>(STR("Experimental Blueprint Support is enabled.\n"));
            BlueprintModLoader.Initialize();
        }

        HandleDataTableChangedCallbacks.push_back([&](UECustom::UDataTable* Table) {
            RawTableLoader.Apply(Table);
        });

        auto HandleDataTableChanged_Address = Palworld::SignatureManager::GetSignature("UDataTable::HandleDataTableChanged");
        if (HandleDataTableChanged_Address)
        {
            HandleDataTableChanged_Hook = safetyhook::create_inline(reinterpret_cast<void*>(HandleDataTableChanged_Address),
                HandleDataTableChanged);
        }
    }

    void PalMainLoader::Initialize()
	{
		LanguageModLoader.Initialize();
		MonsterModLoader.Initialize();
		HumanModLoader.Initialize();
		ItemModLoader.Initialize();
		SkinModLoader.Initialize();
		AppearanceModLoader.Initialize();
		BuildingModLoader.Initialize();
		RawTableLoader.Initialize();

		Load();
	}

    void PalMainLoader::Load()
	{
        IterateModsFolder([&](const fs::directory_entry& modFolder) {
            auto& modsPath = modFolder.path();

            PS::Log<RC::LogLevel::Normal>(STR("Loading mod: {}\n"), modsPath.stem().native());

            auto palFolder = modsPath / "pals";
            LoadPalMods(palFolder);

            auto itemsFolder = modsPath / "items";
            LoadItemMods(itemsFolder);

            auto skinsFolder = modsPath / "skins";
            LoadSkinMods(skinsFolder);

            auto appearanceFolder = modsPath / "appearance";
            LoadAppearanceMods(appearanceFolder);

            auto buildingsFolder = modsPath / "buildings";
            LoadBuildingMods(buildingsFolder);

            auto translationsFolder = modsPath / "translations";
            LoadLanguageMods(translationsFolder);

            if (PS::PSConfig::IsExperimentalBlueprintSupportEnabled())
            {
                auto blueprintFolder = modsPath / "blueprints";
                LoadBlueprintMods(blueprintFolder);
            }
        });
	}

	void PalMainLoader::LoadLanguageMods(const std::filesystem::path& path)
	{
		const auto& currentLanguage = LanguageModLoader.GetCurrentLanguage();

        auto globalLanguageFolder = path / "global";
        if (fs::exists(globalLanguageFolder))
        {
            ParseJsonFileInPath(globalLanguageFolder, [&](nlohmann::json data) {
                LanguageModLoader.Load(data);
            });
        }

        auto translationLanguageFolder = path / currentLanguage;
		if (fs::exists(translationLanguageFolder))
		{
            ParseJsonFileInPath(translationLanguageFolder, [&](nlohmann::json data) {
                LanguageModLoader.Load(data);
            });
		}
	}

	void PalMainLoader::LoadPalMods(const std::filesystem::path& path)
	{
        ParseJsonFileInPath(path, [&](nlohmann::json data) {
            MonsterModLoader.Load(data);
        });
	}

	void PalMainLoader::LoadItemMods(const std::filesystem::path& path)
	{
        ParseJsonFileInPath(path, [&](nlohmann::json data) {
            ItemModLoader.Load(data);
        });
	}

	void PalMainLoader::LoadSkinMods(const std::filesystem::path& path)
	{
        ParseJsonFileInPath(path, [&](nlohmann::json data) {
            SkinModLoader.Load(data);
        });
	}

	void PalMainLoader::LoadBuildingMods(const std::filesystem::path& path)
	{
        ParseJsonFileInPath(path, [&](nlohmann::json data) {
            BuildingModLoader.Load(data);
        });
	}

	void PalMainLoader::LoadAppearanceMods(const std::filesystem::path& path)
	{
        ParseJsonFileInPath(path, [&](nlohmann::json data) {
            AppearanceModLoader.Load(data);
        });
	}

    void PalMainLoader::LoadRawTables(const std::filesystem::path& path)
    {
        ParseJsonFileInPath(path, [&](nlohmann::json data) {
            RawTableLoader.Load(data);
        });
    }

    void PalMainLoader::LoadBlueprintMods(const std::filesystem::path& path)
    {
        ParseJsonFileInPath(path, [&](nlohmann::json data) {
            BlueprintModLoader.Load(data);
        });
    }

    void PalMainLoader::IterateModsFolder(const std::function<void(const std::filesystem::directory_entry&)>& callback)
    {
        // For backwards compatibility with old UE4SS path
        fs::path cwd = fs::current_path() / "Mods" / "PalSchema" / "mods";

        if (!fs::exists(cwd))
        {
            cwd = fs::current_path() / "ue4ss" / "Mods" / "PalSchema" / "mods";
        }

        if (fs::exists(cwd))
        {
            for (const auto& entry : fs::directory_iterator(cwd)) {
                if (entry.is_directory())
                {
                    callback(entry);
                }
            }
        }
    }

    void PalMainLoader::ParseJsonFileInPath(const std::filesystem::path& path, const std::function<void(const nlohmann::json&)>& callback)
    {
        if (!fs::is_directory(path))
        {
            return;
        }

        for (const auto& file : fs::directory_iterator(path)) {
            try
            {
                auto filePath = file.path();
                if (filePath.has_extension())
                {
                    if (filePath.extension() == ".json" || filePath.extension() == ".jsonc")
                    {
                        auto ignoreComments = filePath.extension() == ".jsonc";
                        std::ifstream f(filePath);
                        nlohmann::json data = nlohmann::json::parse(f, nullptr, true, ignoreComments);
                        callback(data);
                        PS::Log<RC::LogLevel::Normal>(STR("Mod '{}' loaded.\n"), filePath.filename().native());
                    }
                }
            }
            catch (const std::exception& e)
            {
                PS::Log<RC::LogLevel::Error>(STR("Failed parsing mod file {} - {}.\n"), RC::to_generic_string(file.path().native()), RC::to_generic_string(e.what()));
            }
        }
    }

    void PalMainLoader::HandleDataTableChanged(UECustom::UDataTable* This, RC::Unreal::FName param_1)
    {
        HandleDataTableChanged_Hook.call(This, param_1);

        static auto CompositeDataTableClass = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/Engine.CompositeDataTable"));
        if (This->GetClassPrivate() == CompositeDataTableClass) return;

        for (auto& Callback : HandleDataTableChangedCallbacks)
        {
            Callback(This);
        }
    }
}
