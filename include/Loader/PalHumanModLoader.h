#pragma once

#include "Loader/PalModLoaderBase.h"
#include "nlohmann/json.hpp"

namespace Palworld {
	class PalHumanModLoader : public PalModLoaderBase {
	public:
		PalHumanModLoader();

		~PalHumanModLoader();

		virtual void Load(const nlohmann::json& json) override final;
	};
}