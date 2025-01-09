#include "SDK/Classes/PalStaticConsumeItemData.h"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UClass.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
	UClass* UPalStaticConsumeItemData::StaticClass()
	{
		auto Class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/Pal.PalStaticConsumeItemData"));
		return Class;
	}
}