#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

namespace Palworld {
	class EnumCache {
	public:
		static bool TryGetEnumValue(const std::string& key, uint8_t& outValue);

		template <typename T>
		static bool TryGetEnumValueT(const std::string& key, T& outValue)
		{
			auto enumIterator = EnumMap.find(key);
			if (enumIterator != EnumMap.end())
			{
				outValue = static_cast<T>(enumIterator->second);
				return true;
			}
			return false;
		}

		static void Initialize();
	private:
		static inline std::unordered_map<std::string, uint8_t> EnumMap;
		static inline std::unordered_set<std::wstring> EnumsToCache = {
			TEXT("EPalOneDayTimeType"),
			TEXT("EPalWeatherConditionType")
		};
	};
}