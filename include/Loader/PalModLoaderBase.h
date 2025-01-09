#pragma once

#include "nlohmann/json.hpp"

namespace UECustom {
	class UDataTable;
}

namespace Palworld {
	class PalModLoaderBase {
	public:
		PalModLoaderBase();

		virtual ~PalModLoaderBase();

		virtual void Load(const nlohmann::json& json) = 0;

		void Initialize();
	};
}