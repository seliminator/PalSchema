#pragma once

#include "Unreal/UObject.hpp"
#include "SDK/Structs/FSoftObjectPath.h"

namespace UECustom {
	class UKismetSystemLibrary : public RC::Unreal::UObject {
	public:
		static FSoftObjectPath MakeSoftObjectPath(const RC::Unreal::FString& Path);
	private:
		static UKismetSystemLibrary* GetDefaultObj();
	};
}