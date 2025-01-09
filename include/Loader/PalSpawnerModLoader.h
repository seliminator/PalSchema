#pragma once

#pragma once

#include "Unreal/NameTypes.hpp"
#include "Loader/PalModLoaderBase.h"

namespace RC::Unreal {
	class AActor;
	class UObject;
}

namespace UECustom {
	class UDataAsset;
}

namespace Palworld {
	struct SpawnerPalData {
		RC::Unreal::FName PalID = RC::Unreal::NAME_None;
		RC::Unreal::FName NPCID = RC::Unreal::NAME_None;
		int Level = 1;
		int Level_Max = 1;
		int Num = 1;
		int Num_Max = 1;
	};

	struct SpawnerDataRow {
		int Weight = 1;
		std::string OnlyTime = "EPalOneDayTimeType::Undefined";
		std::string OnlyWeather = "EPalWeatherConditionType::Undefined";
		std::vector<SpawnerPalData> PalList;
	};

	class PalSpawnerModLoader : public PalModLoaderBase {
	public:
		PalSpawnerModLoader();

		~PalSpawnerModLoader();

		void Initialize();

		virtual void Load(const nlohmann::json& Data) override final;

		void ApplySpawnerMod(RC::Unreal::AActor* Spawner);
	private:
		std::unordered_map<RC::Unreal::FName, std::vector<SpawnerDataRow>> m_spawners;
	};
}