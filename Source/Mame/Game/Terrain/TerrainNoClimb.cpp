#include "TerrainNoClimb.h"

TerrainNoClimb::TerrainNoClimb(const char* const filename)
    : Terrain(filename)
{
}

TerrainNoClimb::~TerrainNoClimb()
{
}

void TerrainNoClimb::Initialize()
{
    terrainType_ = Terrain::Type::NoClimb; // 登れない地形タイプに設定
}

void TerrainNoClimb::Finalize()
{
}

void TerrainNoClimb::Update(const float& elapsedTime)
{
    UpdateAABB();   // AABBの更新処理
}

void TerrainNoClimb::Render(const float& elapsedTime)
{
    Terrain::Render(elapsedTime);
}

void TerrainNoClimb::DrawDebug()
{
    ImGui::Begin(GetName());

    Terrain::DrawDebug();

    ImGui::End();
}
