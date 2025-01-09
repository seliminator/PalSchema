#pragma once

#include "Unreal/NameTypes.hpp"

namespace UECustom {
    struct FTopLevelAssetPath
    {
    public:
        FTopLevelAssetPath() {}

        FTopLevelAssetPath(RC::Unreal::FName InPackageName, RC::Unreal::FName InAssetName)
        {
            TrySetPath(InPackageName, InAssetName);
        }

        bool TrySetPath(RC::Unreal::FName InPackageName, RC::Unreal::FName InAssetName)
        {
            PackageName = InPackageName;
            AssetName = InAssetName;
            return PackageName != RC::Unreal::NAME_None;
        }

        void Reset()
        {
            PackageName = AssetName = RC::Unreal::FName();
        }

        /** Return the package name part e.g. /Path/To/Package as an FName. */
        RC::Unreal::FName GetPackageName() const { return PackageName; }

        /** Return the asset name part e.g. AssetName as an FName. */
        RC::Unreal::FName GetAssetName() const { return AssetName; }
    private:
        RC::Unreal::FName PackageName;
        RC::Unreal::FName AssetName;
    };
}