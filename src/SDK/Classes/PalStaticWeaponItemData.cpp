#include "SDK/Classes/PalStaticWeaponItemData.h"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UClass.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
	UClass* UPalStaticWeaponItemData::StaticClass()
	{
		auto Class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/Pal.PalStaticWeaponItemData"));
		return Class;
	}
}