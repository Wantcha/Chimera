#pragma once

#include "AssetCache.h"
#include "Chimera/Renderer/Texture.h"
#include "Chimera/Core/Log.h"

namespace Chimera
{
	class AssetManager
	{
	public:
		static void RegisterCache(Scope<IAssetCache> cache);
		static void RegisterAsset(const std::string& id);

		template<typename T>
		static Ref<T> LoadAsset(const std::string& path);

		template<typename T>
		static Ref<T> GetAsset(const std::string& id)
		{
			AssetType type = T::GetAssetType();
			if (s_Caches.find(type) != s_Caches.end())
			{
				AssetCache<T>* cache = static_cast<AssetCache<T>*>(s_Caches[type].get());
				Ref<T> asset = cache->GetAsset(id);

				if (asset) return asset;

				CM_CORE_WARN("No asset found. Loading it...");
				asset = LoadAsset<T>(id);
				cache->CacheAsset(id, asset);

				return asset;
			}
		}

		static void CollectGarbage();
	private:
		static std::unordered_map<AssetType, Scope<IAssetCache>> s_Caches;
	};
}
