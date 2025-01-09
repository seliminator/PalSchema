#pragma once

#include "Unreal/UObject.hpp"

namespace UECustom {
	class UDataAsset : public RC::Unreal::UObject {
	public:
		uint8_t Padding[0x28];
		RC::Unreal::UClass* NativeClass;
	};
}