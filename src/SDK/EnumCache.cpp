#include "Unreal/UEnum.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "SDK/EnumCache.h"

using namespace RC;
using namespace RC::Unreal;

namespace Palworld {
	bool EnumCache::TryGetEnumValue(const std::string& key, uint8_t& outValue)
	{
		auto enumIterator = EnumMap.find(key);
		if (enumIterator != EnumMap.end())
		{
			outValue = enumIterator->second;
			return true;
		}
		return false;
	}

	void EnumCache::Initialize()
	{
        std::vector<UObject*> EnumObjects;
        UObjectGlobals::FindAllOf(TEXT("Enum"), EnumObjects);

        for (auto& EnumObject : EnumObjects)
        {
            auto Enum = static_cast<UEnum*>(EnumObject);
            if (EnumsToCache.count(Enum->GetName()))
            {
                auto& Names = Enum->GetNames();
                for (auto& Name : Names)
                {
                    auto EnumString = RC::to_string(Name.Key.ToString());
                    EnumMap.emplace(EnumString, static_cast<uint8_t>(Name.Value));
                }
            }
        }
	}
}