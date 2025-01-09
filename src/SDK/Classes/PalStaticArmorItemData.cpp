#include "SDK/Classes/PalStaticArmorItemData.h"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UClass.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
	UClass* UPalStaticArmorItemData::StaticClass()
	{
		auto Class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/Pal.PalStaticArmorItemData"));
		return Class;
	}
}