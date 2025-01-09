#pragma once

#include "Unreal/UObject.hpp"
#include "SDK/Classes/PalStaticItemDataBase.h"

namespace Palworld {
    class UPalStaticArmorItemData : public UPalStaticItemDataBase {
    public:
        static RC::Unreal::UClass* StaticClass();
    };
}