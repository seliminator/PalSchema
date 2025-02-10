#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Helpers/String.hpp>
#include "Utility/Config.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

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

            json data = {};
            if (f.fail()) {
                std::ofstream out_file(cwd / "config.json");
                data["languageOverride"] = "";
                out_file << data.dump(4);
                out_file.close();
            }
            else
            {
                data = json::parse(f);
            }

            if (!data.contains("languageOverride"))
            {
                data["languageOverride"] = "";
            }

            if (!data.at("languageOverride").is_string())
            {
                Output::send<LogLevel::Error>(STR("languageOverride in config.json wasn't a string, resetting to default.\n"));
                data["languageOverride"] = "";
            }

            m_config->m_languageOverride = data["languageOverride"].get<std::string>();

            Output::send<LogLevel::Normal>(STR("PalSchema config loaded.\n"));
        }
        catch (const std::exception& e)
        {
            Output::send<LogLevel::Error>(STR("Failed to load PalSchema Config: {}\n"), RC::to_generic_string(e.what()));
        }
    }

    std::string PSConfig::GetLanguageOverride()
    {
        if (m_config)
        {
            return m_config->m_languageOverride;
        }

        Output::send<LogLevel::Error>(STR("PalSchema Config must be initialized first before accessing Language Override!"));

        return "";
    }
}