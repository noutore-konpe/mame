#include "ResourceManager.h"

const std::shared_ptr<skinned_mesh> ResourceManager::LoadModelResource(
    ID3D11Device*  device, 
    const char* const       fbx_filename, 
    const bool              triangulate,
    const float             sampling_rate)
{
    // モデルを検索
    const ModelMap::iterator it = models.find(fbx_filename);   
    if (it != models.end())
    {
        // リンク(寿命)が切れていないか確認
        if (it->second.expired() == false)
        {
            // 検索したモデルと同じモデルが見つかったらそれを返す
            return it->second.lock();
        }
    }

    // 見つからなかったら新規モデルリソースを作成・読み込み
    const std::shared_ptr<skinned_mesh> model = {
        std::make_shared<skinned_mesh>(
            device, 
            fbx_filename, 
            triangulate, 
            sampling_rate)
    };

    // マップに登録
    models[fbx_filename] = model;

    return model;
}

const std::shared_ptr<skinned_mesh> ResourceManager::LoadModelResource(
    ID3D11Device*             device, 
    const char* const                  fbx_filename, 
    std::vector<std::string> animation_filenames, 
    const bool                         triangulate, 
    const float                        sampling_rate)
{
    // モデルを検索
    const ModelMap::iterator it = models.find(fbx_filename);
    if (it != models.end())
    {
        // リンク(寿命)が切れていないか確認
        if (it->second.expired() == false)
        {
            // 検索したモデルと同じモデルが見つかったらそれを返す
            return it->second.lock();
        }
    }

    // 見つからなかったら新規モデルリソースを作成・読み込み
    const std::shared_ptr<skinned_mesh> model = {
        std::make_shared<skinned_mesh>(
            device,
            fbx_filename,
            animation_filenames,
            triangulate,
            sampling_rate)
    };

    // マップに登録
    models[fbx_filename] = model;

    return model;
}

