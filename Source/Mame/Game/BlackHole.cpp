#include "BlackHole.h"

#include "../../Taki174/DebugRenderer.h"
#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "MagicCircle.h"
#include "AbilityManager.h"
#include "EnemyManager.h"

#include "../Scene/SceneGame.h"

int BlackHole::nameNum_ = 0;

BlackHole::BlackHole(AbilityManager* abilityManager)
    : Ability(abilityManager)
{
    Graphics& graphics = Graphics::Instance();

    // 仮モデル生成
    model_ = std::make_unique<Model>(
        graphics.GetDevice(),
        "./Resources/Model/cube.fbx"
    );
    // 魔法陣生成
    magicCircle_ = std::make_unique<MagicCircle>();

    // タグ設定
    tag_ = TAG::BLACK_HOLE;

    // 名前設定
    name_ = { "BlackHole : " + std::to_string(nameNum_++) };

}

BlackHole::~BlackHole()
{
    --nameNum_;

}

void BlackHole::Initialize()
{
    magicCircle_->Initialize();

}

void BlackHole::Finalize()
{
    magicCircle_->Finalize();

}

void BlackHole::Begin()
{
    magicCircle_->Begin();

}

void BlackHole::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // 寿命更新
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) { Destroy(); }
    }

    // 敵が吸い込み範囲内にいたら自分の方へ引き寄せる
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        const size_t  enemyCount   = enemyManager.GetEnemyCount();
        for (size_t i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = enemyManager.GetEnemy(i);

            const XMFLOAT3& enmPos  = enemy->GetTransform()->GetPosition();
            const XMFLOAT3& pos     = GetTransform()->GetPosition();

            float length = 0.0f;
            const XMFLOAT3 toBlackHoleVecN = ::XMFloat3Normalize(pos - enmPos, &length);

            if (length > inhaleLength_) continue;
            if (length <= 0.5f)         continue;   // 中心に近かったらcontinueする

            // 敵を減速させる
            const XMFLOAT3 deceleration = enemy->GetVelocity() * (-1.0f);
            enemy->AddVelocity(deceleration * elapsedTime);

            // 自分の方へ引き寄せる
            const XMFLOAT3 force = toBlackHoleVecN * inhaleForce_;
            enemy->GetTransform()->AddPosition(force * elapsedTime);
        }
    }

    // magicCircle_->Update(elapsedTime);
    magicCircle_->GetTransform()->SetPosition(this->GetTransform()->GetPosition());

}

void BlackHole::End()
{
    magicCircle_->End();

}

void BlackHole::Render(
    const float scale, ID3D11PixelShader* psShader)
{
    using DirectX::XMFLOAT3;
    using DirectX::XMFLOAT4;

    // modelは使用しない

    magicCircle_->Render(scale, psShader);

#ifdef _DEBUG

    // 吸い込む範囲を表示
    if (true == SceneGame::isDispCollision_)
    {
        // デバッグ球体描画
        DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
        const XMFLOAT3  pos    = GetTransform()->GetPosition();
        constexpr float height = 0.5f;
        const XMFLOAT4  color  = { 0,0,1,1 };
        debugRenderer->DrawCylinder(pos, inhaleLength_, height, color);
    }

#endif // _DEBUG

}

void BlackHole::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(name_.c_str()))
    {
        // modelは使用しない

        magicCircle_->DrawDebug();

        ImGui::EndMenu();
    }

#endif
}
