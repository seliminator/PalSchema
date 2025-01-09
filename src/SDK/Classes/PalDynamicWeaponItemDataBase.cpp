#include "SDK/Classes/PalDynamicWeaponItemDataBase.h"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UClass.hpp"

using namespace RC::Unreal;

namespace Palworld {
	UClass* UPalDynamicWeaponItemDataBase::StaticClass()
	{
		static auto Class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/Pal.PalDynamicWeaponItemDataBase"));
		return Class;
	}
}