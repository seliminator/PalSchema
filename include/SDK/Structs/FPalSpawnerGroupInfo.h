#pragma once

#include <cstdint>
#include "Unreal/Core/Containers/Array.hpp"
#include "SDK/Structs/FPalSpawnerOneTribeInfo.h"

namespace Palworld {
    struct FPalSpawnerGroupInfo final
    {
    public:
        int Weight;
        uint8_t OnlyTime;
        uint8_t OnlyWeather;
        uint8_t Pad_6[0x2];
        RC::Unreal::TArray<FPalSpawnerOneTribeInfo> PalList;
    };
}