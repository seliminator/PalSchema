#include "Loader/Blueprint/PalBlueprintMod.h"
#include <Helpers/String.hpp>

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
    PalBlueprintMod::PalBlueprintMod(const std::string& BlueprintName, const nlohmann::json& Data)
    {
        auto BlueprintName_Conv = RC::to_generic_string(BlueprintName);
        m_name = FName(BlueprintName_Conv, FNAME_Add);
        m_defaultObjectName = FName(std::format(STR("Default__{}"), BlueprintName_Conv), FNAME_Add);
        m_data = Data;
    }

    const FName& PalBlueprintMod::GetBlueprintName() const
    {
        return m_name;
    }

    const FName& PalBlueprintMod::GetDefaultObjectName() const
    {
        return m_defaultObjectName;
    }

    const nlohmann::json& PalBlueprintMod::GetData() const
    {
        return m_data;
    }
}