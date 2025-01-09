#pragma once

#include "SDK/Classes/UDataTable.h"
#include "SDK/Classes/TSoftClassPtr.h"
#include "SDK/Classes/Texture2D.h"
#include "SDK/Structs/FTableRowBase.h"

namespace Palworld {
    struct FPalBPClassDataRow : public UECustom::FTableRowBase
    {
        FPalBPClassDataRow(const RC::StringType& Path) : BPClass(UECustom::TSoftClassPtr<RC::Unreal::UClass>(UECustom::FSoftObjectPath(Path)))
        {
        }
        UECustom::TSoftClassPtr<RC::Unreal::UClass> BPClass;
    };
}