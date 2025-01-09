#pragma once

#include "SDK/Classes/UDataTable.h"
#include "SDK/Classes/TSoftObjectPtr.h"
#include "SDK/Classes/Texture2D.h"
#include "SDK/Structs/FTableRowBase.h"

namespace Palworld {
    struct FPalCharacterIconDataRow : public UECustom::FTableRowBase
    {
        FPalCharacterIconDataRow(const RC::StringType& Path) : Icon(UECustom::TSoftObjectPtr<UECustom::UTexture2D>(UECustom::FSoftObjectPath(Path)))
        {
        }

        UECustom::TSoftObjectPtr<UECustom::UTexture2D> Icon;
    };
}