#include <fstream>
#include <filesystem>
#include "Unreal/AActor.hpp"
#include "Unreal/UClass.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/UScriptStruct.hpp"
#include "Unreal/FProperty.hpp"
#include "Unreal/Property/FEnumProperty.hpp"
#include "Unreal/Hooks.hpp"
#include "DynamicOutput/DynamicOutput.hpp"
#include "Helpers/String.hpp"
#include "Loader/PalMainLoader.h"
#include <Utility/Config.h>

using namespace RC;
using namespace RC::Unreal;

namespace fs = std::filesystem;

namespace Palworld {
    void PalMainLoader::PreInitialize()
    {
        InitializeBlueprintModLoader();
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
		SpawnerModLoader.Initialize();
		RawTableLoader.Initialize();

		Load();
	}

    void PalMainLoader::InitializeBlueprintModLoader()
    {
        if (PS::PSConfig::IsExperimentalBlueprintSupportEnabled())
        {
            Output::send<LogLevel::Normal>(STR("[PalSchema] Experimental Blueprint Support is enabled.\n"));
            BlueprintModLoader.Initialize();

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
                        auto& modsPath = entry.path();

                        auto blueprintFolder = modsPath / "blueprints";
                        if (fs::is_directory(blueprintFolder))
                        {
                            Output::send<LogLevel::Normal>(STR("Loading mod: {}\n"), modsPath.stem().native());

                            LoadBlueprintMods(blueprintFolder);
                        }
                    }
                }
            }
        }
    }

    void PalMainLoader::Load()
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
					auto& modsPath = entry.path();

					Output::send<LogLevel::Normal>(STR("Loading mod: {}\n"), modsPath.stem().native());

					auto palFolder = modsPath / "pals";
					if (fs::is_directory(palFolder))
					{
						LoadPalMods(palFolder);
					}

					auto itemsFolder = modsPath / "items";
					if (fs::is_directory(itemsFolder))
					{
						LoadItemMods(itemsFolder);
					}

					auto skinsFolder = modsPath / "skins";
					if (fs::is_directory(skinsFolder))
					{
						LoadSkinMods(skinsFolder);
					}

					auto appearanceFolder = modsPath / "appearance";
					if (fs::is_directory(appearanceFolder))
					{
						LoadAppearanceMods(appearanceFolder);
					}

					auto spawnerFolder = modsPath / "spawner";
					if (fs::is_directory(spawnerFolder))
					{
						LoadSpawnerMods(spawnerFolder);
					}

					auto buildingsFolder = modsPath / "buildings";
					if (fs::is_directory(buildingsFolder))
					{
						LoadBuildingMods(buildingsFolder);
					}

					auto rawFolder = modsPath / "raw";
					if (fs::is_directory(rawFolder))
					{
						LoadRawTables(rawFolder);
					}

					auto translationsFolder = modsPath / "translations";
					if (fs::is_directory(translationsFolder))
					{
						LoadLanguageMods(translationsFolder);
					}
				}
			}
		}
	}

	void PalMainLoader::LoadLanguageMods(const std::filesystem::path& path)
	{
		const auto& currentLanguage = LanguageModLoader.GetCurrentLanguage();
		auto translationLanguageFolder = path / currentLanguage;
		if (fs::exists(translationLanguageFolder))
		{
			for (const auto& translationFile : fs::directory_iterator(translationLanguageFolder)) {
				try
				{
					auto translationFilePath = translationFile.path();
					if (translationFilePath.has_extension())
					{
						if (translationFilePath.extension() == ".json")
						{
							std::ifstream f(translationFilePath);
							nlohmann::json data = nlohmann::json::parse(f);
							LanguageModLoader.Load(data);
						}
					}
				}
				catch (const std::exception& e)
				{
					Output::send<LogLevel::Error>(STR("Failed parsing language file {} - {}.\n"), RC::to_generic_string(translationFile.path().native()), RC::to_generic_string(e.what()));
				}
			}
		}
	}

	void PalMainLoader::LoadPalMods(const std::filesystem::path& path)
	{
		for (const auto& palFile : fs::directory_iterator(path)) {
			try
			{
				auto palFilePath = palFile.path();
				if (palFilePath.has_extension())
				{
					if (palFilePath.extension() == ".json")
					{
						std::ifstream f(palFilePath);
						nlohmann::json data = nlohmann::json::parse(f);
						MonsterModLoader.Load(data);
						Output::send<LogLevel::Normal>(STR("Pal Mod '{}' loaded.\n"), palFilePath.filename().native());
					}
				}
			}
			catch (const std::exception& e)
			{
				Output::send<LogLevel::Error>(STR("Failed parsing pal file {} - {}.\n"), RC::to_generic_string(palFile.path().native()), RC::to_generic_string(e.what()));
			}
		}
	}

	void PalMainLoader::LoadItemMods(const std::filesystem::path& path)
	{
		for (const auto& palFile : fs::directory_iterator(path)) {
			try
			{
				auto palFilePath = palFile.path();
				if (palFilePath.has_extension())
				{
					if (palFilePath.extension() == ".json")
					{
						std::ifstream f(palFilePath);
						nlohmann::json data = nlohmann::json::parse(f);
						ItemModLoader.Load(data);
						Output::send<LogLevel::Normal>(STR("Item Mod '{}' loaded.\n"), palFilePath.filename().native());
					}
				}
			}
			catch (const std::exception& e)
			{
				Output::send<LogLevel::Error>(STR("Failed parsing item file {} - {}.\n"), RC::to_generic_string(palFile.path().native()), RC::to_generic_string(e.what()));
			}
		}
	}

	void PalMainLoader::LoadSkinMods(const std::filesystem::path& path)
	{
		for (const auto& palFile : fs::directory_iterator(path)) {
			try
			{
				auto palFilePath = palFile.path();
				if (palFilePath.has_extension())
				{
					if (palFilePath.extension() == ".json")
					{
						std::ifstream f(palFilePath);
						nlohmann::json data = nlohmann::json::parse(f);
						SkinModLoader.Load(data);
						Output::send<LogLevel::Normal>(STR("Skin Mod '{}' loaded.\n"), palFilePath.filename().native());
					}
				}
			}
			catch (const std::exception& e)
			{
				Output::send<LogLevel::Error>(STR("Failed parsing skin file {} - {}.\n"), RC::to_generic_string(palFile.path().native()), RC::to_generic_string(e.what()));
			}
		}
	}

	void PalMainLoader::LoadSpawnerMods(const std::filesystem::path& path)
	{
		for (const auto& file : fs::directory_iterator(path)) {
			try
			{
				auto filePath = file.path();
				if (filePath.has_extension())
				{
					if (filePath.extension() == ".json")
					{
						std::ifstream f(filePath);
						nlohmann::json data = nlohmann::json::parse(f);
						SpawnerModLoader.Load(data);
						Output::send<LogLevel::Normal>(STR("Spawner Mod '{}' loaded.\n"), filePath.filename().native());
					}
				}
			}
			catch (const std::exception& e)
			{
				Output::send<LogLevel::Error>(STR("Failed parsing spawner file {} - {}.\n"), RC::to_generic_string(file.path().native()), RC::to_generic_string(e.what()));
			}
		}
	}

	void PalMainLoader::LoadBuildingMods(const std::filesystem::path& path)
	{
		for (const auto& file : fs::directory_iterator(path)) {
			try
			{
				auto filePath = file.path();
				if (filePath.has_extension())
				{
					if (filePath.extension() == ".json")
					{
						std::ifstream f(filePath);
						nlohmann::json data = nlohmann::json::parse(f);
						BuildingModLoader.Load(data);
						Output::send<LogLevel::Normal>(STR("Building Mod '{}' loaded.\n"), filePath.filename().native());
					}
				}
			}
			catch (const std::exception& e)
			{
				Output::send<LogLevel::Error>(STR("Failed parsing building file {} - {}.\n"), RC::to_generic_string(file.path().native()), RC::to_generic_string(e.what()));
			}
		}
	}

	void PalMainLoader::LoadAppearanceMods(const std::filesystem::path& path)
	{
		for (const auto& palFile : fs::directory_iterator(path)) {
			try
			{
				auto palFilePath = palFile.path();
				if (palFilePath.has_extension())
				{
					if (palFilePath.extension() == ".json")
					{
						std::ifstream f(palFilePath);
						nlohmann::json data = nlohmann::json::parse(f);
						AppearanceModLoader.Load(data);
						Output::send<LogLevel::Normal>(STR("Appearance Mod '{}' loaded.\n"), palFilePath.filename().native());
					}
				}
			}
			catch (const std::exception& e)
			{
				Output::send<LogLevel::Error>(STR("Failed parsing appearance file {} - {}.\n"), RC::to_generic_string(palFile.path().native()), RC::to_generic_string(e.what()));
			}
		}
	}

	void PalMainLoader::LoadRawTables(const std::filesystem::path& path)
	{
		for (const auto& rawFile : fs::directory_iterator(path)) {
			try
			{
				auto rawFilePath = rawFile.path();
				if (rawFilePath.has_extension())
				{
					if (rawFilePath.extension() == ".json")
					{
						std::ifstream f(rawFilePath);
						nlohmann::json data = nlohmann::json::parse(f);
						RawTableLoader.Load(data);
						Output::send<LogLevel::Normal>(STR("Raw Tables in '{}' loaded.\n"), rawFilePath.filename().native());
					}
				}
			}
			catch (const std::exception& e)
			{
				Output::send<LogLevel::Error>(STR("Failed parsing raw table file {} - {}.\n"), RC::to_generic_string(rawFile.path().native()), RC::to_generic_string(e.what()));
			}
		}
	}

    void PalMainLoader::LoadBlueprintMods(const std::filesystem::path& path)
    {
        for (const auto& blueprintFile : fs::directory_iterator(path)) {
            try
            {
                auto blueprintFilePath = blueprintFile.path();
                if (blueprintFilePath.has_extension())
                {
                    if (blueprintFilePath.extension() == ".json")
                    {
                        std::ifstream f(blueprintFilePath);
                        nlohmann::json data = nlohmann::json::parse(f);
                        BlueprintModLoader.Load(data);
                        Output::send<LogLevel::Normal>(STR("Blueprint mods in '{}' loaded.\n"), blueprintFilePath.filename().native());
                    }
                }
            }
            catch (const std::exception& e)
            {
                Output::send<LogLevel::Error>(STR("Failed parsing blueprint mod {} - {}.\n"), RC::to_generic_string(blueprintFile.path().native()), RC::to_generic_string(e.what()));
            }
        }
    }
}
