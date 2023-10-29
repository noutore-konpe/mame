#include "ExperiencePointManager.h"

#include "../../Taki174/FunctionXMFloat3.h"
#include "../Scene/SceneGame.h"
#include "PlayerManager.h"
#include "Collision.h"
#include "../Resource/AudioManager.h"

void ExperiencePointManager::Initialize()
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->Initialize();
    }
}


void ExperiencePointManager::Finalize()
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->Finalize();
    }
    Clear();
}


void ExperiencePointManager::Begin()
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->Begin();
    }
}


void ExperiencePointManager::Update(const float elapsedTime)
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->Update(elapsedTime);
    }

    // 破棄処理
    for (ExperiencePoint* exp : removes_)
    {
        auto it = std::find(exps_.begin(), exps_.end(), exp);
        if (it != exps_.end()) { exps_.erase(it); }

        ::SafeDeletePtr(exp);
    }
    removes_.clear();

    CollisionExpVsPlayer(elapsedTime);
    CollisionExpVsExp(elapsedTime);
    CollisionExpVsStage(elapsedTime);
}


void ExperiencePointManager::End()
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->End();
    }
}


void ExperiencePointManager::Render(
    const float scale, ID3D11PixelShader* psShader)
{
    for (ExperiencePoint*& exp : exps_)
    {
        // カメラ外なら描画しない
        bool isInCamera = false;
        Sprite::ConvertToScreenPos(exp->GetTransform()->GetPosition(), &isInCamera);
        if (false == isInCamera) { continue; }

        exp->Render(scale, psShader);
    }
}


void ExperiencePointManager::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu("Exps"))
    {
        for (ExperiencePoint*& exp : exps_)
        {
            exp->DrawDebug();
            int step = static_cast<int>(exp->GetStep());
            ImGui::DragInt("Step", &step);
        }
        ImGui::Separator();

        ImGui::EndMenu();
    }

#endif
}


void ExperiencePointManager::CollisionExpVsPlayer(const float /*elapsedTime*/)
{
    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();

    const size_t expCount = exps_.size();
    for (size_t i = 0; i < expCount; ++i)
    {
        ExperiencePoint* exp = GetExp(i);

        // プレイヤーが死亡している場合は取得しないようにする
        if (true == plManager.GetPlayer()->GetIsDead()) continue;

        // 演出時間中の場合はcontinue
        if (exp->GetAttractTimer() > 0.0f) continue;

        const XMFLOAT3& expPos    = exp->GetTransform()->GetPosition();
        const float     expRadius = exp->GetRadius();
        XMFLOAT3        plPos     = plManager.GetPlayer()->GetTransform()->GetPosition();
        plPos.y += 1.0f;
        //const float     plRadius  = plManager.GetPlayer()->GetRadius();
        const float     plRadius  = 0.5f;

        if (Collision::IntersectSphereVsSphere(
            expPos, expRadius, plPos, plRadius))
        {
            Remove(exp);

            PlayerManager::Instance().GetPlayer()->ApplyExp(5);

            AudioManager::Instance().PlaySE(SE_NAME::GetExp, SE::GetExp_01, SE::GetExp_20);
        }

    }
}


void ExperiencePointManager::CollisionExpVsExp(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    const size_t enemyCount = GetExpCount();
    for (size_t a = 0; a < enemyCount; ++a)
    {
        ExperiencePoint* expA = GetExp(a);

        // a以降の敵と判定を行う（a以前はすでに判定済みのため）
        for (size_t b = a + 1; b < enemyCount; ++b)
        {
            ExperiencePoint* expB = GetExp(b);

            const XMFLOAT3& posA    = expA->GetTransform()->GetPosition();
            const XMFLOAT3& posB    = expB->GetTransform()->GetPosition();
            const float     radiusA = expA->GetRadius();
            const float     radiusB = expB->GetRadius();

            const XMFLOAT3  vecFromAtoB = posB - posA;
            const float     lengthSq = ::XMFloat3LengthSq(vecFromAtoB);
            const float     range = radiusA + radiusB;

            if (lengthSq > (range * range)) continue;

            const XMFLOAT3 vecN_fromAtoB = ::XMFloat3Normalize(vecFromAtoB);

            // じんわりとBを押しのける
            const XMFLOAT3 moveForce = vecN_fromAtoB * 0.5f;
            expB->GetTransform()->AddPosition(moveForce * elapsedTime);
        }

    }

}


void ExperiencePointManager::CollisionExpVsStage(const float /*elapsedTime*/)
{
    using DirectX::XMFLOAT3;

    // 経験値のステージ内外判定処理を行う
    const size_t enemyCount = GetExpCount();
    for (size_t i = 0; i < enemyCount; ++i)
    {
        ExperiencePoint* exp = GetExp(i);

        // 距離の二乗を取得
        Transform*     expT     = exp->GetTransform();
        const XMFLOAT3 vec      = expT->GetPosition() - SceneGame::stageCenter;
        const float    lengthSq = ::XMFloat3LengthSq(vec);

        // ステージの半径の二乗を取得
        const float stageRadiusSq = SceneGame::stageRadius * SceneGame::stageRadius;

        // ステージ内にいるならcontinue
        if (lengthSq <= stageRadiusSq) continue;

        // ベクトルを正規化
        const XMFLOAT3 vecN = vec / ::sqrtf(lengthSq);

        // 修正した位置を代入
        expT->SetPosition(SceneGame::stageCenter + vecN * SceneGame::stageRadius);
    }

}


void ExperiencePointManager::Register(ExperiencePoint* exp)
{
    exps_.emplace_back(exp);
}

void ExperiencePointManager::Remove(ExperiencePoint* exp)
{
    removes_.insert(exp);
}

void ExperiencePointManager::Clear()
{
    for (ExperiencePoint*& exp : exps_)
    {
        ::SafeDeletePtr(exp);
    }
    exps_.clear();
    exps_.shrink_to_fit();
}


void ExperiencePointManager::CreateExp(
    const DirectX::XMFLOAT3& position,
    const int expCount,
    const DirectX::XMFLOAT3& minForce,
    const DirectX::XMFLOAT3& maxForce)
{
    using DirectX::XMFLOAT3;

    for (int i = 0; i < expCount; ++i)
    {
        ExperiencePoint* exp = new ExperiencePoint();

        exp->Initialize();  // 初期化しておく
        exp->GetTransform()->SetPosition(position); // 生成位置設定

        // 散らばる速度を設定
        const XMFLOAT3 force = {
            ::RandFloat(minForce.x, maxForce.x),
            ::RandFloat(minForce.y, maxForce.y),
            ::RandFloat(minForce.z, maxForce.z),
        };
        exp->SetVelocity(force);

        this->Register(exp);
    }
}
