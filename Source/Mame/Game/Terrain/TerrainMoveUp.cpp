#include "TerrainMoveUp.h"
#include "../PlayerManager.h"

void TerrainMoveUp::Initialize()
{
    Terrain::Initialize();

    terrainType_ = Terrain::Type::Move; // 動く地形タイプに設定
}

void TerrainMoveUp::Update(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    // 上昇させる
    {
        velocity_.y = (addSpeedY_ * elapsedTime);
        transform->AddPositionY(velocity_.y);

        // 一定より上に行ったら位置をリセット
        if (transform->GetPosition().y > positionLimitY_)
        {
            transform->SetPositionY(resetPositionY_);

            // プレイヤーが保存している地形情報の地形が自分だったらその地形情報を消去
            // (リセット時にプレイヤーがついてこないようにするため)
            PlayerManager& playerManager = PlayerManager::Instance();
            if (this == playerManager.GetPlayer()->saveTerrain_)
            {
                playerManager.GetPlayer()->saveTerrain_ = nullptr;
            }
        }
    }

    UpdateAABB();   // AABBの更新処理
}

void TerrainMoveUp::DrawDebug()
{
    ImGui::Begin(GetName());
    Terrain::DrawDebug();
    ImGui::End();
}
