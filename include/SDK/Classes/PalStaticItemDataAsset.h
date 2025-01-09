#pragma once

#include "SDK/Classes/UDataAsset.h"
#include "SDK/Classes/PalStaticItemDataBase.h"
#include "Unreal/Core/Containers/Map.hpp"

namespace Palworld {
	class UPalStaticItemDataAsset : public UECustom::UDataAsset {
	public:
		RC::Unreal::TMap<RC::Unreal::FName, UPalStaticItemDataBase*> StaticItemDataMap;
	};
}