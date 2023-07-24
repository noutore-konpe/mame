#pragma once

#include <memory>
#include "Terrain.h"

class TerrainManager
{
private:
    TerrainManager()  = default;
    ~TerrainManager() = default;
    
public:
    static TerrainManager& Instance()
    {
        static TerrainManager instance;
        return instance;
    }

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Update(const float& elapsedTime);  // 更新処理
    void Render(const float& elapsedTime);  // 描画処理
    void DrawDebug();                       // デバッグ描画

public:
    void Register(Terrain* enemy);  // 地形登録 
    void Remove(Terrain* enemy);    // 地形削除  
    void Clear();                   // 地形全削除

public: 
    const int GetTerrainCount() const { return static_cast<int>(terrains.size()); } // 地形数取得  
    Terrain* GetTerrain(int index) { return terrains.at(index); }                   // 地形取得



private:
    std::vector<Terrain*> terrains;
    std::set<Terrain*> removes;
};

