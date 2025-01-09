#include "SDK/Classes/KismetSystemLibrary.h"
#include "Unreal/UFunction.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace UECustom {
	FSoftObjectPath UKismetSystemLibrary::MakeSoftObjectPath(const RC::Unreal::FString& Path)
	{
		static auto Function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, TEXT("/Script/Engine.KismetSystemLibrary:MakeSoftObjectPath"));

		struct {
			RC::Unreal::FString Path;
			FSoftObjectPath ReturnValue;
		}params;

		params.Path = Path;

		GetDefaultObj()->ProcessEvent(Function, &params);

		return params.ReturnValue;
	}

	UKismetSystemLibrary* UKismetSystemLibrary::GetDefaultObj()
	{
		static auto Self = UObjectGlobals::StaticFindObject<UKismetSystemLibrary*>(nullptr, nullptr, TEXT("/Script/Engine.Default__KismetSystemLibrary"));
		return Self;
	}
}