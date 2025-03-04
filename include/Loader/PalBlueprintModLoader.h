#pragma once

#include "Loader/PalModLoaderBase.h"
#include "Loader/Blueprint/PalBlueprintMod.h"
#include "Unreal/NameTypes.hpp"
#include "Unreal/UObjectArray.hpp"
#include <unordered_map>
#include <safetyhook.hpp>

namespace Palworld {
    class PalBlueprintModLoader : public PalModLoaderBase {
    public:
        PalBlueprintModLoader();

        ~PalBlueprintModLoader();

        virtual void Load(const nlohmann::json& Data) override final;

        void Initialize();
    private:
        static inline std::unordered_map<RC::Unreal::FName, std::vector<PalBlueprintMod>> BPModRegistry;

        static std::vector<PalBlueprintMod>& GetModsForBlueprint(const RC::Unreal::FName& Name);
        
        static void ApplyMod(const PalBlueprintMod& BPMod, RC::Unreal::UObject* Object);

        static void ApplyMod(const nlohmann::json& Data, RC::Unreal::UObject* Object);
    private:
        static void PostLoadDefaultObject(RC::Unreal::UClass* This, RC::Unreal::UObject* DefaultObject);

        static inline SafetyHookInline PostLoadDefaultObject_Hook;
    };
}