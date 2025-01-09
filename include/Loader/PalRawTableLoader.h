#pragma once

#include "Unreal/NameTypes.hpp"
#include "Loader/PalModLoaderBase.h"
#include "nlohmann/json.hpp"

namespace UECustom {
	class UDataTable;
}

namespace Palworld {
	class PalRawTableLoader : public PalModLoaderBase {
	public:
		PalRawTableLoader();

		~PalRawTableLoader();

		void Initialize();

		virtual void Load(const nlohmann::json& json) override final;
	private:
		UECustom::UDataTable* GetTable(const RC::StringType& TableName);

		std::unordered_map<RC::StringType, UECustom::UDataTable*> m_cachedTables;
	};
}