#pragma once

#include "Unreal/FString.hpp"
#include "File/Macros.hpp"
#include "nlohmann/json.hpp"
#include "Loader/PalModLoaderBase.h"

namespace UECustom {
	class UDataTable;
}

namespace Palworld {
	class PalLanguageModLoader : public PalModLoaderBase {
	public:
		PalLanguageModLoader();

		~PalLanguageModLoader();

		virtual void Load(const nlohmann::json& Data) override final;

		void Initialize();

		const std::string& GetCurrentLanguage();
	private:
		std::unordered_map<std::string, UECustom::UDataTable*> m_tables;

		std::string m_currentLanguage{};

		void Add(const std::string& TableKey, const std::string& RowId, const std::string& RowValue);

		void AddTable(const std::string& Id, UECustom::UDataTable* Table);

		UECustom::UDataTable* GetTable(const std::string& Id);
	};
}