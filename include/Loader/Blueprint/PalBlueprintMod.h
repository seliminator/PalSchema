#pragma once

#include <string>
#include "Unreal/NameTypes.hpp"
#include "nlohmann/json.hpp"

namespace Palworld {
    class PalBlueprintMod {
    public:
        PalBlueprintMod(const std::string& BlueprintName, const nlohmann::json& Data);

        virtual ~PalBlueprintMod() {};
    public:
        const RC::Unreal::FName& GetBlueprintName() const;

        const RC::Unreal::FName& GetDefaultObjectName() const;

        const nlohmann::json& GetData() const;
    private:
        RC::Unreal::FName m_name;
        RC::Unreal::FName m_defaultObjectName;
        nlohmann::json m_data;
    };
}