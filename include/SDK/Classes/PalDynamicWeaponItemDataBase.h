#pragma once

namespace RC::Unreal {
    class UClass;
}

namespace Palworld {
    class UPalDynamicWeaponItemDataBase {
    public:
        static RC::Unreal::UClass* StaticClass();
    };
}