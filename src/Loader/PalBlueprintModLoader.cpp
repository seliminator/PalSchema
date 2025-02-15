#include <regex>
#include "Unreal/UClass.hpp"
#include "Unreal/UObject.hpp"
#include "Unreal/AActor.hpp"
#include "Unreal/Property/FObjectProperty.hpp"
#include "Helpers/String.hpp"
#include "Utility/DataTableHelper.h"
#include "Utility/Config.h"
#include "Loader/PalBlueprintModLoader.h"

#include <Signatures.hpp>
#include <SigScanner/SinglePassSigScanner.hpp>

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
    PalBlueprintModLoader::PalBlueprintModLoader()
    {
    }

    PalBlueprintModLoader::~PalBlueprintModLoader()
    {
        PostLoadDefaultObject_Hook = {};
    }

    void PalBlueprintModLoader::Load(const nlohmann::json& Data)
    {
        for (auto& [BlueprintName, BlueprintData] : Data.items())
        {
            auto BlueprintName_Conv = RC::to_generic_string(BlueprintName);
            auto BlueprintFName = FName(BlueprintName_Conv, FNAME_Add);
            auto NewBlueprintMod = PalBlueprintMod(BlueprintName, BlueprintData);
            BPModRegistry.emplace(BlueprintFName, NewBlueprintMod);
        }
    }

    void PalBlueprintModLoader::Initialize()
    {
        SignatureContainer SigContainer1 = [=]() -> SignatureContainer {
            return {
                {{"48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8D 99 58 03 00 00"}},
                [=](SignatureContainer& self) {
                    void* FunctionPointer = static_cast<void*>(self.get_match_address());

                    PostLoadDefaultObject_Hook = safetyhook::create_inline(FunctionPointer,
                        reinterpret_cast<void*>(PostLoadDefaultObject));
    ;
                    self.get_did_succeed() = true;

                    return true;
                },
                [=](const SignatureContainer& self) {
                    if (!self.get_did_succeed())
                    {
                        Output::send<LogLevel::Error>(STR("[PalSchema] Failed to find signature for UBlueprintGeneratedClass::PostLoadDefaultObject.\n"));
                    }
                }
            };
        }();

        std::vector<SignatureContainer> SigContainerBox;
        SinglePassScanner::SignatureContainerMap SigContainerMap;
        SigContainerBox.emplace_back(SigContainer1);
        SigContainerMap.emplace(ScanTarget::MainExe, SigContainerBox);
        SinglePassScanner::start_scan(SigContainerMap);
    }

    PalBlueprintMod& PalBlueprintModLoader::GetMod(const RC::Unreal::FName& Name)
    {
        auto Iterator = BPModRegistry.find(Name);
        if (Iterator != BPModRegistry.end())
        {
            return Iterator->second;
        }

        throw std::runtime_error(RC::fmt("Something went wrong getting a blueprint mod entry from BPModRegistry. Affected mod name: %S", Name.ToString().c_str()));
    }

    void PalBlueprintModLoader::ApplyMod(const PalBlueprintMod& BPMod, UObject* Object)
    {
        UClass* Class = Object->GetClassPrivate();
        auto ClassFullName = Class->GetFullName();

        std::wregex PathRegex(STR("/Game/[^ ]+"));
        std::wsmatch MatchResult;

        if (std::regex_search(ClassFullName, MatchResult, PathRegex))
        {
            auto ExtractedPath = MatchResult.str();

            auto& Data = BPMod.GetData();
            for (auto& [Key, Value] : Data.items())
            {
                auto PropertyName = RC::to_generic_string(Key);
                auto Property = Class->GetPropertyByNameInChain(PropertyName.c_str());
                if (Property)
                {
                    if (auto ObjectProperty = CastField<FObjectProperty>(Property))
                    {
                        auto ObjectValue = *Property->ContainerPtrToValuePtr<UObject*>(static_cast<void*>(Object));
                        if (!ObjectValue)
                        {
                            auto GEN_VAR_PATH = std::format(STR("{}:{}_GEN_VARIABLE"), ExtractedPath, PropertyName);
                            auto GEN_VAR_OBJECT = UObjectGlobals::StaticFindObject<UObject*>(nullptr, nullptr, GEN_VAR_PATH);
                            if (GEN_VAR_OBJECT)
                            {
                                if (Value.is_object())
                                {
                                    for (auto& [InnerKey, InnerValue] : Value.items())
                                    {
                                        auto GEN_VAR_OBJECT_PROPERTY_NAME = RC::to_generic_string(InnerKey);
                                        auto GEN_VAR_OBJECT_PROPERTY = GEN_VAR_OBJECT->GetPropertyByNameInChain(GEN_VAR_OBJECT_PROPERTY_NAME.c_str());
                                        if (GEN_VAR_OBJECT_PROPERTY)
                                        {
                                            Palworld::DataTableHelper::CopyJsonValueToTableRow(static_cast<void*>(GEN_VAR_OBJECT), GEN_VAR_OBJECT_PROPERTY, InnerValue);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        Palworld::DataTableHelper::CopyJsonValueToTableRow(static_cast<void*>(Object), Property, Value);
                    }
                }
                else
                {
                    Output::send<LogLevel::Warning>(STR("[PalSchema] Property '{}' does not exist in {}\n"), PropertyName, BPMod.GetBlueprintName().ToString());
                }
            }
        }
    }

    void PalBlueprintModLoader::PostLoadDefaultObject(UClass* This, UObject* DefaultObject) {
        if (DefaultObject)
        {
            if (Palworld::PalBlueprintModLoader::BPModRegistry.contains(This->GetNamePrivate()))
            {
                auto& Mod = PalBlueprintModLoader::GetMod(This->GetNamePrivate());
                try
                {
                    PalBlueprintModLoader::ApplyMod(Mod, DefaultObject);
                    Output::send<LogLevel::Verbose>(STR("[PalSchema] Applied modifications to {}\n"), Mod.GetBlueprintName().ToString());
                }
                catch (const std::exception& e)
                {
                    Output::send<LogLevel::Error>(STR("[PalSchema] Failed modifying blueprint '{}', {}\n"), Mod.GetBlueprintName().ToString(), RC::to_generic_string(e.what()));
                }
            }
        }

        PostLoadDefaultObject_Hook.call(This, DefaultObject);
    }
}