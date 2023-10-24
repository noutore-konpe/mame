#include "EnemyManager.h"

#include "../../Taki174/Common.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../../Taki174/NumeralManager.h"
#include "../Scene/SceneGame.h"
#include "Collision.h"
#include "PlayerManager.h"

void EnemyManager::Initialize()
{
    // CRA : 0.Initialize : ������
    craActionCoolTimer_ = 0.0f;
    isRunningCRAAction_ = false;

    for (Enemy*& enemy : enemies_)
    {
        enemy->Initialize();
    }

    projectileManager_.Initialize();
}

void EnemyManager::Finalize()
{
    projectileManager_.Finalize();
    projectileManager_.Clear();

    for (Enemy*& enemy : enemies_)
    {
        enemy->Finalize();
    }
    Clear();
}

void EnemyManager::Begin()
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->Begin();
    }

    projectileManager_.Begin();
}

// �X�V����
void EnemyManager::Update(const float elapsedTime)
{
    // CRA : 1.Update : �ߐڍU���s���N�[���^�C�}�[����
    craActionCoolTimer_ = (std::max)(0.0f, craActionCoolTimer_ - elapsedTime);

    //����
    for (Enemy* enemy : generates_)
    {
        enemies_.emplace_back(enemy);
    }
    generates_.clear();

    // �X�V
    for (Enemy*& enemy : enemies_)
    {
        enemy->Update(elapsedTime);
    }

    // �j������
    {
        for (Enemy* enemy : removes_)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            auto it = std::find(enemies_.begin(), enemies_.end(), enemy);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != enemies_.end())
            {
                enemies_.erase(it);
            }

            // ���������G���e�ۂ̐e�̏ꍇ�A�e�ۂ̐e������������
            const size_t projectileCount = projectileManager_.GetProjectileCount();
            for (size_t i = 0; i < projectileCount; ++i)
            {
                Projectile* projectile = projectileManager_.GetProjectile(i);

                if (nullptr == projectile->GetParent()) continue;
                if (enemy != projectile->GetParent()) continue;

                projectile->SetParent(nullptr);
            }

            // ���������G�������\���̐e�̏ꍇ�A�����\���̐e������������
            NumeralManager& numeralManager = NumeralManager::Instance();
            const size_t numeralCount = numeralManager.GetNumeralCount();
            for (size_t i = 0; i < numeralCount; ++i)
            {
                Numeral* numeral = numeralManager.GetNumeral(i);

                if (nullptr == numeral->GetParent()) continue;
                if (enemy != numeral->GetParent()) continue;

                numeral->SetParent(nullptr);
            }



            // �G�̔j������
            SafeDeletePtr(enemy);
        }
        // �j�����X�g���N���A
        removes_.clear();
    }

    projectileManager_.Update(elapsedTime);

    // �e�ۂƃv���C���[�̏Փˏ���
    CollisionProjectileVsPlayer();

    // �G�l�~�[���m�̏Փˏ���
    CollisionEnemyVsEnemy(elapsedTime);

    // �G�ƃX�e�[�W�Ƃ̏Փˏ���
    CollisionEnemyVsStage(elapsedTime);

}

void EnemyManager::End()
{
    for (Enemy*& enemy : enemies_)
    {
        enemy->End();
    }

    projectileManager_.End();
}


// �`�揈��
void EnemyManager::Render(const float scale, ID3D11PixelShader* psShader)
{
    for (Enemy*& enemy : enemies_)
    {
        // �J�����O�Ȃ�`�悵�Ȃ�
        bool isInCamera = false;
        Sprite::ConvertToScreenPos(enemy->GetTransform()->GetPosition(), &isInCamera);
        if (false == isInCamera) { continue; }

        enemy->Render(scale, psShader);
    }

    projectileManager_.Render(scale, psShader);

}


// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
    //enemies_.emplace_back(enemy);
    generates_.insert(enemy);
}


// �G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
    // �j�����X�g�ɒǉ�
    removes_.insert(enemy);
}


// �G�l�~�[�S�폜
void EnemyManager::Clear()
{
    for (Enemy*& enemy : enemies_)
    {
        SafeDeletePtr(enemy);
    }
    enemies_.clear();
    enemies_.shrink_to_fit();    // vector�̗]���ȃ����������
}


// �f�o�b�O
void EnemyManager::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("Enemies"))
    {

        // �G�Ђ�܂���{�^��
        if (ImGui::Button("FlinchEnemies"))
        {
            for (Enemy*& enemy : enemies_)
            {
                enemy->Flinch();
            }
        }

        for (Enemy*& enemy : enemies_)
        {
            enemy->DrawDebug();
        }
        ImGui::Separator();

        // Behavior���ȈՂɌ���p
        //for (Enemy*& enemy : enemies_)
        //{
        //    std::string str = (enemy->GetActiveNode() != nullptr)
        //        ? enemy->GetActiveNode()->GetName()
        //        : u8"�Ȃ�";
        //    ImGui::Text(u8"Behavior�F%s", str.c_str());
        //}

        ImGui::EndMenu();
    }
    ImGui::Separator();


    if (ImGui::BeginMenu("Enemy_ProjectileManager"))
    {
        projectileManager_.DrawDebug();
        ImGui::EndMenu();
    }
    ImGui::Separator();


#endif
}

void EnemyManager::RenderShadow(const float scale, ID3D11PixelShader* psShader)
{
    for (Enemy*& enemy : enemies_)
    {
        // �S�[��������Ȃ�������`��
        if (enemy->GetType() != Enemy::TYPE::Golem)
        {
            enemy->Render(scale, psShader);
        }
        else
        {   // �S�[�����̉e
            enemy->Render(scale, false);
        }
    }


    projectileManager_.Render(scale, psShader);
}

void EnemyManager::CollisionProjectileVsPlayer()
{
    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();

    const size_t projCount = projectileManager_.GetProjectileCount();
    for (size_t projIndex = 0; projIndex < projCount; ++projIndex)
    {
        // �e�ێ擾
        Projectile* proj = projectileManager_.GetProjectile(projIndex);

        // �v���C���[�擾
        const std::unique_ptr<Player>& player = plManager.GetPlayer();

        // �v���C���[������ł�����break
        if (true == player->GetIsDead()) break;

        const size_t plHitColliderCount = player->GetHitColliderCount();
        for (size_t plHitColliderIndex = 0; plHitColliderIndex < plHitColliderCount; ++plHitColliderIndex)
        {
            // �v���C���[�̐H�炢����擾
            const Character::SphereCollider plHitCollder = player->GetHitColliderAt(plHitColliderIndex);

            const XMFLOAT3& projPos             = proj->GetPosition();
            const XMFLOAT3& plHitColliderPos    = plHitCollder.position;
            const float     projRadius          = proj->GetRadius();
            const float     plHitColliderRadius = plHitCollder.radius;

            // �������ĂȂ����continue
            if (false == Collision::IntersectSphereVsSphere(
                projPos, projRadius, plHitColliderPos, plHitColliderRadius))
            {
                continue;
            }

            // �v���C���[�Ƀ_���[�W��^����
            player->ApplyDamage(proj->GetAttack(),plHitColliderPos,proj->GetOwner());

            // �e����������
            proj->Destroy();

            // ���������̂�break
            break;
        }

    }

}

void EnemyManager::CollisionEnemyVsEnemy(const float /*elapsedTime*/)
{
    using DirectX::XMFLOAT3;

    const size_t enemyCount = GetEnemyCount();
    for (size_t a = 0; a < enemyCount; ++a)
    {
        Enemy* enemyA = GetEnemy(a);

        // a�ȍ~�̓G�Ɣ�����s���ia�ȑO�͂��łɔ���ς݂̂��߁j
        for (size_t b = a + 1; b < enemyCount; ++b)
        {
            Enemy* enemyB = GetEnemy(b);

            const XMFLOAT3& positionA = enemyA->GetPosition();
            const XMFLOAT3& positionB = enemyB->GetPosition();
            const float     radiusA   = enemyA->GetRadius();
            const float     radiusB   = enemyB->GetRadius();

            const XMFLOAT3  vecAtoB   = positionB - positionA;
            const float     lengthSq  = ::XMFloat3LengthSq(vecAtoB);
            const float     range     = radiusA + radiusB;

            if (lengthSq > (range * range)) continue;

            const XMFLOAT3 vecAtoB_N = ::XMFloat3Normalize(vecAtoB);

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
            enemyB->SetPosition(positionA + vecAtoB_N * range);
#endif

        }

    }

}


void EnemyManager::CollisionEnemyVsStage(const float /*elapsedTime*/)
{
    using DirectX::XMFLOAT3;
    EnemyManager& enemyManager = EnemyManager::Instance();

    // �G�̃X�e�[�W���O���菈�����s��
    const size_t enemyCount = enemyManager.GetEnemyCount();
    for (size_t i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        // �G���܂��X�e�[�W�ɓ��ꂵ�Ă��Ȃ���Δ�����s��Ȃ�
        if (false == enemy->GetEntryStageFlag()) continue;

        // �����̓����擾
        Transform*     enemyT   = enemy->GetTransform();
        const XMFLOAT3 vec      = enemyT->GetPosition() - SceneGame::stageCenter;
        const float    lengthSq = ::XMFloat3LengthSq(vec);

        // �X�e�[�W�̔��a�̓����擾
        const float stageRadiusSq = SceneGame::stageRadius * SceneGame::stageRadius;

        // �X�e�[�W���ɂ���Ȃ�continue
        if (lengthSq <= stageRadiusSq) continue;

        // �x�N�g���𐳋K��
        const XMFLOAT3 vecN = vec / ::sqrtf(lengthSq);

        // �C�������ʒu����
        enemyT->SetPosition(SceneGame::stageCenter + vecN * SceneGame::stageRadius);
    }

}

bool EnemyManager::AttackCollisionPlayerToEnemy(Enemy* my,DirectX::XMFLOAT3& hitPos)
{
    for (auto& atkCollider : my->GetAttackCollider())
    {
        for (auto& hitCollider : PlayerManager::Instance().GetPlayer()->GetHitCollider())
        {
            if (Collision::IntersectSphereVsSphere(
                atkCollider.position, atkCollider.radius,
                hitCollider.position, hitCollider.radius))
            {
                hitPos = hitCollider.position;
                return true;
            }
        }
    }
    return false;
}

bool EnemyManager::ACAttackCollisionPlayerToEnemy(Enemy* my)
{
    for (auto& atkCollider : my->GetAttackCollider())
    {
        if (atkCollider.activeAttack)continue;

        for (auto& hitCollider : PlayerManager::Instance().GetPlayer()->GetHitCollider())
        {
            if (Collision::IntersectSphereVsSphere(
                atkCollider.position, atkCollider.radius,
                hitCollider.position, hitCollider.radius))
            {
                return true;
            }
        }
    }
    return false;
}
