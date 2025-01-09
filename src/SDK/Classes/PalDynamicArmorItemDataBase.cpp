#include "SDK/Classes/PalDynamicArmorItemDataBase.h"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UClass.hpp"

using namespace RC::Unreal;

namespace Palworld {
	UClass* UPalDynamicArmorItemDataBase::StaticClass()
	{
		static auto Class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/Pal.PalDynamicArmorItemDataBase"));
		return Class;
	}
}