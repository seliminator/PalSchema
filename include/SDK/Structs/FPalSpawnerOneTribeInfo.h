#pragma once

#include "SDK/Structs/FPalDataTableRowName_PalMonsterData.h"
#include "SDK/Structs/FPalDataTableRowName_PalHumanData.h"

namespace Palworld {
    struct FPalSpawnerOneTribeInfo
    {
        FPalDataTableRowName_PalMonsterData PalID;
        FPalDataTableRowName_PalHumanData NPCID;
        int Level = 1;
        int Level_Max = 1;
        int Num = 1;
        int Num_Max = 1;
    };
}