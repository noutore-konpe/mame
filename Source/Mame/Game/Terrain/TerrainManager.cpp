#include "TerrainManager.h"
#include "../../Game/Common.h"


void TerrainManager::Initialize()
{
    for (Terrain*& terrain : terrains)
    {
        terrain->Initialize();
    }
}


void TerrainManager::Finalize()
{
    for (Terrain*& terrain : terrains)
    {
        terrain->Finalize();
    }
}

// 更新処理
void TerrainManager::Update(const float& elapsedTime)
{
    // 更新
    for (Terrain*& terrain : terrains)
    {
        terrain->Update(elapsedTime);
    }

    // 破棄処理
    for (Terrain* terrain : removes)
    {
        // vectorから要素を削除する場合はイテレーターで削除
        std::vector<Terrain*>::iterator it = std::find(terrains.begin(), terrains.end(), terrain);

        // std::vectorで管理されている要素を削除するにはerase()関数を使用する
        // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
        if (it != terrains.end())
        {
            terrains.erase(it);
        }

        // 敵の破棄処理
        SafeDelete(terrain);
    }
    // 破棄リストをクリア
    removes.clear();
}

// 描画処理
void TerrainManager::Render(const float& elapsedTime)
{
    for (Terrain*& terrain : terrains)
    {
        terrain->Render(elapsedTime);
    }
}

// 地形登録
void TerrainManager::Register(Terrain* terrain)
{
    terrains.emplace_back(terrain);
}

// 地形削除
void TerrainManager::Remove(Terrain* terrain)
{
    // 破棄リストに追加
    removes.insert(terrain);
}

// 地形全削除
void TerrainManager::Clear()
{
    for (Terrain*& terrain : terrains)
    {
        SafeDelete(terrain);
    }
    terrains.clear();
    terrains.shrink_to_fit();    // vectorの余分なメモリを解放する関数(C++11)
}



// デバッグ
void TerrainManager::DrawDebug()
{
    for (Terrain*& terrain : terrains)
    {
        terrain->DrawDebug();
    }
}
