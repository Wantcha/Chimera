#pragma once

#define ASSET_CLASS(type) static AssetType GetAssetType() { return type; }

namespace Chimera
{
    enum class AssetType : unsigned int
    {
        Undefined,
        Texture2D,
        Shader,
        LuaScript
    };
}
