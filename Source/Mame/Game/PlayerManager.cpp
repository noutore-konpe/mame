#include "PlayerManager.h"
#include "EnemyManager.h"


#include "../../Taki174/FunctionXMFloat3.h"

// ������
void PlayerManager::Initialize()
{
    player->Initialize();
}

// �I����
void PlayerManager::Finalize()
{
    player->Finalize();
}

// Update�̑O�ɌĂ΂��
void PlayerManager::Begin()
{
    player->Begin();
}

// �X�V����
void PlayerManager::Update(const float& elapsedTime)
{
    player->Update(elapsedTime);

    //���@�ƓG�̓����蔻��
    CollisionPlayerVsEnemy();
}

// Update�̌�ɌĂ΂��
void PlayerManager::End()
{
    player->End();
}

// �`�揈��
void PlayerManager::Render(const float& scale, ID3D11PixelShader* psShader)
{
    player->Render(scale, psShader);
}

// ImGui�p
void PlayerManager::DrawDebug()
{
#ifdef USE_IMGUI
    player->DrawDebug();
#endif // USE_IMGUI
}

void PlayerManager::CollisionPlayerVsEnemy()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    const size_t enemyCount = enemyManager.GetEnemyCount();
    for (size_t a = 0; a < enemyCount; ++a)
    {
        Enemy* enemy = EnemyManager::Instance().GetEnemy(a);

        const DirectX::XMFLOAT3  positionA = enemy->GetPosition();
        const DirectX::XMFLOAT3  positionB = player->GetPosition();
        const float     radiusA = enemy->GetRadius();
        const float     radiusB = player->GetRadius();

        const DirectX::XMFLOAT3  vecAtoB = positionB - positionA;
        const float     lengthSq = XMFloat3LengthSq(vecAtoB);
        const float     range = radiusA + radiusB;


        if (lengthSq > (range * range)) continue;


        const DirectX::XMFLOAT3 vecAtoB_N = XMFloat3Normalize(vecAtoB);

        // CRA : 7.Collisition : �ߐڍU��������Ȃ����̓G�������o��
#if 0
        if (enemyB->GetActiveNode() != nullptr &&
            "CloseRangeAttack" == enemyB->GetActiveNode()->GetName())
        {
            const XMFLOAT3 vecBtoA_N = vecAtoB_N * (-1.0f);
            enemyA->SetPosition(positionB + vecBtoA_N * range);
        }
        else
        {
            enemyB->SetPosition(positionA + vecAtoB_N * range);
        }
#else
        player->SetPosition(positionA + vecAtoB_N * range);
#endif
    }
}
