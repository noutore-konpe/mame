#pragma once

#include <map>
#include <string>
#include <memory>

#include "../Resource/skinned_mesh.h"
#include "../Game/Common.h"

class ResourceManager
{
private:
    ResourceManager()  {}
    ~ResourceManager() {}

public:
    static ResourceManager& Instance()
    {
        static ResourceManager instance;
        return instance;
    }

    // モデルリソース読み込み
    const std::shared_ptr<skinned_mesh> LoadModelResource(
        NO_CONST ID3D11Device* device, 
        const char* const      fbx_filename, 
        const bool             triangulate      = false, 
        const float            sampling_rate    = 0
    );

    // モデルリソース読み込み(アニメーション)
    const std::shared_ptr<skinned_mesh> LoadModelResource(
        NO_CONST ID3D11Device*             device, 
        const char* const                  fbx_filename, 
        NO_CONST std::vector<std::string>& animation_filenames, 
        const bool                         triangulate     = false, 
        const float                        sampling_rate   = 0
    );

private:
    using ModelMap = std::map<const char*, std::weak_ptr<skinned_mesh>>;

private:
    ModelMap models = {};

};

