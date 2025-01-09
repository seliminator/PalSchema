#pragma once

#include "Loader/PalModLoaderBase.h"

namespace RC::Unreal {
	class UObject;
}

namespace UECustom {
	class UDataAsset;
	class UDataTable;
}

namespace Palworld {
	class PalSkinModLoader : public PalModLoaderBase {
	public:
		PalSkinModLoader();

		~PalSkinModLoader();

		void Initialize();

		virtual void Load(const nlohmann::json& json) override final;
	private:
		UECustom::UDataAsset* m_skinDataAsset;
		UECustom::UDataTable* m_skinIconTable;
		UECustom::UDataTable* m_skinTranslationTable;

		std::unordered_map<RC::StringType, RC::Unreal::UClass*> m_palSkinDataBaseClassCache;

		void Add(const RC::Unreal::FName& SkinId, RC::Unreal::TMap<RC::Unreal::FName, RC::Unreal::UObject*>* StaticSkinMap, const nlohmann::json& Data);

		void Edit(const RC::Unreal::FName& SkinId, RC::Unreal::UObject* Item, const nlohmann::json& Data);

	private:
		void AddTranslation(const RC::Unreal::FName& SkinId, const nlohmann::json& Data);

		void EditTranslation(const RC::Unreal::FName& SkinId, const nlohmann::json& Data);

		RC::Unreal::UClass* LoadOrCacheSkinDataBaseClass(const RC::StringType& Path);
	};
}