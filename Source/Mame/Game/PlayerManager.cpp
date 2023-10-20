#include "PlayerManager.h"
#include "EnemyManager.h"

#include "../../Taki174/FunctionXMFloat3.h"

#include "Collision.h"

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
    using DirectX::XMFLOAT3;

    EnemyManager& enemyManager = EnemyManager::Instance();

    const size_t enemyCount = enemyManager.GetEnemyCount();
    for (size_t i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);

        // ����ł����画�肵�Ȃ�
        if (true == enemy->GetIsDead()) continue;

        const XMFLOAT3& enmPos    = enemy->GetPosition();
        const XMFLOAT3& plPos     = player->GetPosition();

        // �G����v���C���[�Ɍ������x�N�g��
        const XMFLOAT3  vecFromEnemyToPlayer = plPos - enmPos;
        const float     lengthSq         = ::XMFloat3LengthSq(vecFromEnemyToPlayer);

        const float     enmRadius = enemy->GetRadius();
        const float     plRadius  = player->GetRadius();
        const float     range     = enmRadius + plRadius;

        if (lengthSq > (range * range)) continue;

#ifdef _DEBUG
        // ������΂��i���j
        {
            // ������ԕ����x�N�g��(�����K��)��ۑ�
            enemy->SetBlowOffVec(-vecFromEnemyToPlayer);

            enemy->ApplyDamage(999999, nullptr); // ���S�t���O�Ɛ�����уt���O�𗧂Ă�
        }
#endif

        const XMFLOAT3 vecN_fromEnemyToPlayer = XMFloat3Normalize(vecFromEnemyToPlayer);

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
        player->SetPosition(enmPos + vecN_fromEnemyToPlayer * range);
#endif
    }
}

bool PlayerManager::AttackCollisionPlayerToEnemy(std::vector<Enemy*>& hitEnemies)
{
    bool hit = false;
    if (player->isActiveAttackFrame)
    {
        for (auto& enemy : EnemyManager::Instance().GetEnemies())
        {
            for (auto& atkCollider : player->GetAttackCollider())
            {
                for (auto& hitCollider : enemy->GetHitCollider())
                {
                    if (Collision::IntersectSphereVsSphere(
                        atkCollider.position, atkCollider.radius,
                        hitCollider.position, hitCollider.radius))
                    {
                        // ������ԕ����x�N�g��(�����K��)��ۑ�
                        const DirectX::XMFLOAT3 vec = enemy->GetPosition() - atkCollider.position;
                        enemy->SetBlowOffVec(vec);

                        hitEnemies.emplace_back(enemy);
                        hit = true;
                    }
                }
            }
        }
    }
    return hit;
}
