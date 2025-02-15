#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Helpers/String.hpp>
#include "Utility/Config.h"
#include "Utility/Logging.h"

namespace fs = std::filesystem;

using namespace RC;

namespace PS {
    void PSConfig::Load()
    {
        if (m_config) return;

        try
        {
            m_config = std::make_unique<PSConfig>();

            fs::path cwd = fs::current_path() / "Mods" / "PalSchema" / "config";
            if (!fs::exists(cwd))
            {
                cwd = fs::current_path() / "ue4ss" / "Mods" / "PalSchema" / "config";
            }

            if (!fs::exists(cwd))
            {
                fs::create_directories(cwd);
            }

            std::ifstream f(cwd / "config.json");
            bool ShouldResave = false;

            nlohmann::ordered_json data = {};
            if (f.fail()) {
                data["languageOverride"] = "";
                data["enableExperimentalBlueprintSupport"] = false;
                std::ofstream out_file(cwd / "config.json");
                out_file << data.dump(4);
                out_file.close();
            }
            else
            {
                data = nlohmann::ordered_json::parse(f);
            }

            if (!data.contains("languageOverride"))
            {
                data["languageOverride"] = "";
                ShouldResave = true;
            }
            else
            {
                if (!data.at("languageOverride").is_string())
                {
                    PS::Log<RC::LogLevel::Error>(STR("languageOverride in config.json wasn't a string, resetting to default.\n"));
                    data["languageOverride"] = "";
                    ShouldResave = true;
                }
                m_config->m_languageOverride = data["languageOverride"].get<std::string>();
            }

            if (!data.contains("enableExperimentalBlueprintSupport"))
            {
                data["enableExperimentalBlueprintSupport"] = false;
                ShouldResave = true;
            }
            else
            {
                if (!data.at("enableExperimentalBlueprintSupport").is_boolean())
                {
                    PS::Log<RC::LogLevel::Error>(STR("enableExperimentalBlueprintSupport in config.json wasn't a bool, resetting to default.\n"));
                    data["enableExperimentalBlueprintSupport"] = false;
                    ShouldResave = true;
                }
                m_config->m_enableExperimentalBlueprintSupport = data["enableExperimentalBlueprintSupport"].get<bool>();
            }

            if (ShouldResave)
            {
                std::ofstream out_file(cwd / "config.json");
                out_file << data.dump(4);
                out_file.close();
            }

            PS::Log<RC::LogLevel::Normal>(STR("PalSchema config loaded.\n"));
        }
        catch (const std::exception& e)
        {
            PS::Log<RC::LogLevel::Error>(STR("Failed to load PalSchema Config: {}\n"), RC::to_generic_string(e.what()));
        }
    }

    std::string PSConfig::GetLanguageOverride()
    {
        if (m_config)
        {
            return m_config->m_languageOverride;
        }

        PS::Log<RC::LogLevel::Error>(STR("PalSchema Config must be initialized first before accessing GetLanguageOverride!"));

        return "";
    }

    bool PSConfig::IsExperimentalBlueprintSupportEnabled()
    {
        if (m_config)
        {
            return m_config->m_enableExperimentalBlueprintSupport;
        }

        PS::Log<RC::LogLevel::Error>(STR("PalSchema Config must be initialized first before accessing IsExperimentalBlueprintSupportEnabled!"));

        return false;
    }
}