#pragma once

#include "Unreal/FText.hpp"
#include "SDK/Structs/FTableRowBase.h"

namespace Palworld {
    struct FPalLocalizedTextData final : public UECustom::FTableRowBase
    {
    public:
        RC::Unreal::FText TextData;
    };
}