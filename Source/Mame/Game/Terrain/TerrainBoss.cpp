#include "TerrainBoss.h"

TerrainBoss::TerrainBoss(const char* const filename)
    : Terrain(filename)
{
}

TerrainBoss::~TerrainBoss()
{
}

void TerrainBoss::Initialize()
{
    terrainType_ = Terrain::Type::Normal; // 普通の地形タイプに設定
}

void TerrainBoss::Finalize()
{
}

void TerrainBoss::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABBの更新処理
}

void TerrainBoss::Render(const float& elapsedTime)
{
    Terrain::Render(elapsedTime);
}

void TerrainBoss::DrawDebug()
{
    ImGui::Begin(GetName());

    Terrain::DrawDebug();

    ImGui::End();
}
