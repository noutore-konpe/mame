#include "TerrainNormal.h"

TerrainNormal::TerrainNormal(const char* const filename)
    : Terrain(filename)
{
}

TerrainNormal::~TerrainNormal()
{
}

void TerrainNormal::Initialize()
{
    terrainType_ = Terrain::Type::Normal; // 普通の地形タイプに設定
}

void TerrainNormal::Finalize()
{
}

void TerrainNormal::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABBの更新処理
}

void TerrainNormal::Render(const float& elapsedTime)
{
    Terrain::Render(elapsedTime);
}

void TerrainNormal::DrawDebug()
{
    ImGui::Begin(GetName());

    Terrain::DrawDebug();

    ImGui::End();
}
