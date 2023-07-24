#include "TerrainFall.h"
#include "../PlayerManager.h"

TerrainFall::TerrainFall(const char* const filename)
    : Terrain(filename)
{
}

void TerrainFall::Initialize()
{
    Terrain::Initialize();

    terrainType_ = Terrain::Type::Move; // 動く地形タイプに設定

    SetMaterialColor(DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f));
}

void TerrainFall::Update(const float& elapsedTime)
{
    Transform* transform = GetTransform();

    // 落下フラグが立っている場合は落下処理を行う
    if (isFall_)
    {
        velocity_.y += (fallAcceleration_ * elapsedTime);   // 落下速度を加算
        if (velocity_.y < fallVelocityMax_) velocity_.y = fallVelocityMax_;
        transform->AddPositionY(velocity_.y);               // 落下

        // 一定まで落下したら初期位置に戻ってリセットする
        if (transform->GetPosition().y < -20.0f)
        {
            transform->SetPositionY(originPosition_.y);
            velocity_.y = 0.0f;
            isFall_     = false;
            blinkTimer_ = defaultBlinkTime_;

            // プレイヤーが保存している地形情報の地形が自分だったらその地形情報を消去
            // (リセット時にプレイヤーがついてこないようにするため)
            PlayerManager& playerManager = PlayerManager::Instance();
            if (this == playerManager.GetPlayer()->saveTerrain_)
            {
                playerManager.GetPlayer()->saveTerrain_ = nullptr;
            }
        }
    }

    // 点滅タイマーが設定されていれば点滅させる
    if (blinkTimer_ > 0.0f)
    {
        blinkTimer_ -= elapsedTime;

        if (blinkTimer_ <= 0.0f)
        {
            blinkTimer_     = 0.0f;
            materialColor   = { 0.7f,0.7f,0.7f,1 };
        }
        else if (static_cast<int>(blinkTimer_ * 100.0f) & 0x08)
        {
            materialColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        }
        else
        {
            materialColor = { 0.7f,0.7f,0.7f,1 };
        }
    }

    UpdateAABB();   // AABBの更新処理
}


void TerrainFall::DrawDebug()
{
    ImGui::Begin(GetName());
    Terrain::DrawDebug();
    ImGui::End();
}


void TerrainFall::OnRiding()
{
    isFall_ = true;     // 落下開始
}
