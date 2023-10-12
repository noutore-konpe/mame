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

    // �����f������
    model_ = std::make_unique<Model>(
        graphics.GetDevice(),
        "./Resources/Model/cube.fbx"
    );
    // ���@�w����
    magicCircle_ = std::make_unique<MagicCircle>();

    // �^�O�ݒ�
    tag_ = TAG::BLACK_HOLE;

    // ���O�ݒ�
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

    // �����X�V
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) { Destroy(); }
    }

    // �G���z�����ݔ͈͓��ɂ����玩���̕��ֈ����񂹂�
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
            if (length <= 0.5f)         continue;   // ���S�ɋ߂�������continue����

            // �G������������
            const XMFLOAT3 deceleration = enemy->GetVelocity() * (-0.5f);
            enemy->AddVelocity(deceleration * elapsedTime);

            // �����̕��ֈ����񂹂�
            const XMFLOAT3 force = toBlackHoleVecN * inhaleForce_;
            enemy->GetTransform()->AddPosition(force * elapsedTime);
        }
    }

    // magicCircle_->Update(elapsedTime);

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

    // model�͎g�p���Ȃ�

    magicCircle_->Render(scale, psShader);

#ifdef _DEBUG
    if (true == SceneGame::isDrawCollision_)
    {
        // �f�o�b�O���̕`��
        DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
        const XMFLOAT3 center = GetTransform()->GetPosition();
        const XMFLOAT4 color = { 1,1,1,1 };
        debugRenderer->DrawSphere(center, radius_, color);
    }
#endif // _DEBUG

}

void BlackHole::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(name_.c_str()))
    {
        // model�͎g�p���Ȃ�

        magicCircle_->DrawDebug();

        ImGui::EndMenu();
    }

#endif
}
