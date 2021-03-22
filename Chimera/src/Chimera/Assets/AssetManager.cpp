#include "cmpch.h"
#include "AssetManager.h"

namespace Chimera
{
	std::unordered_map<AssetType, Scope<IAssetCache>> AssetManager::s_Caches;

	void AssetManager::RegisterCache(Scope<IAssetCache> cache)
	{
		s_Caches[cache->GetCachedType()] = std::move(cache);
	}

	void AssetManager::RegisterAsset(const std::string& id)
	{

	}

	void AssetManager::CollectGarbage()
	{
		for (auto& kv : s_Caches)
		{
			kv.second->CollectGarbage();
		}
	}


	template<>
	Ref<Texture2D> AssetManager::LoadAsset(const std::string& path)
	{
		Ref<Texture2D> tex = Texture2D::Create(path.c_str());
		return tex;
	}

}