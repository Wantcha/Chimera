#pragma once

#include "Chimera/Core/Core.h"
#include <unordered_map>
#include "Asset.h"

namespace Chimera
{
	class IAssetCache
	{
	public:
		virtual AssetType GetCachedType() = 0;
		virtual void CollectGarbage() = 0;
	};

	template<typename T>
	class AssetCache : public IAssetCache
	{
	public:
		AssetType GetCachedType() override
		{
			return T::GetAssetType();
		}

		void CacheAsset(const std::string& id, Ref<T> asset)
		{
			if (!IsAssetCached(id))
				m_Cache[id] = std::move(asset);
		}

		std::shared_ptr<T> GetAsset(const std::string& id) const
		{
			const auto& found = m_Cache.find(id);
			if (found != m_Cache.end())
			{
				return (*found).second;
			}
			return nullptr;
		}

		bool IsAssetCached(const std::string& id) const
		{
			const auto& found = m_Cache.find(id);
			return (found != m_Cache.end()/* && found->second*/);
		}

		void CollectGarbage() override
		{
			for (auto iter = m_Cache.begin(); iter != m_Cache.end();)
			{
				//CM_CORE_WARN("{0}", iter->second.use_count());
				if (iter->second.use_count() <= 1/* || iter->second == nullptr*/)
				{
					// in this case the only reference left is the cache reference
					// we can free the asset from the cache
					iter = m_Cache.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}

	private:
		std::unordered_map<std::string, Ref<T>> m_Cache;
	};
}