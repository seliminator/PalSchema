#include "Unreal/UClass.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "SDK/Classes/PalStaticItemDataBase.h"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
	RC::Unreal::UClass* UPalStaticItemDataBase::StaticClass()
	{
		static auto Class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, TEXT("/Script/Pal.PalStaticItemDataBase"));
		return Class;
	}
}
