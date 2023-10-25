#include "PlayerManager.h"
#include "EnemyManager.h"

#include "../../Taki174/FunctionXMFloat3.h"

#include "Collision.h"

PlayerManager::PlayerManager()
{
    
}

// ������
void PlayerManager::Initialize()
{

    static bool CallOnce = [&]() {
        //�h���C��
        drainSkill = std::make_unique<PlayerSkill::Drain>(player.get());
        skillArray.emplace_back(drainSkill.get());
        //�ړ����x�A�b�v
        moveSpeedUpSkill = std::make_unique<PlayerSkill::MoveSpeedUp>(player.get());
        skillArray.emplace_back(moveSpeedUpSkill.get());
        //�U���̓A�b�v
        attackPowerUpSkill = std::make_unique<PlayerSkill::AttackPowerUp>(player.get());
        skillArray.emplace_back(attackPowerUpSkill.get());
        //�U�����x�A�b�v
        attackSpeedUpSkill = std::make_unique<PlayerSkill::AttackSpeedUp>(player.get());
        skillArray.emplace_back(attackSpeedUpSkill.get());
        //�{�̐�����
        bookIncreaseSkill = std::make_unique<PlayerSkill::BookIncrease>(player.get());
        skillArray.emplace_back(bookIncreaseSkill.get());
        //�̗͏���A�b�v
        maxHitPointUpSkill = std::make_unique<PlayerSkill::MaxHitPointUp>(player.get());
        skillArray.emplace_back(maxHitPointUpSkill.get());
        //�h��̓A�b�v
        defenseUpSkill = std::make_unique<PlayerSkill::DefenseUp>(player.get());
        skillArray.emplace_back(defenseUpSkill.get());
        //�u���b�N�z�[����������
        blackHoleSkill = std::make_unique<PlayerSkill::BlackHoleSkill>(player.get());
        skillArray.emplace_back(blackHoleSkill.get());
        //�J�E���^�[
        counterSkill = std::make_unique<PlayerSkill::CanCounterAttack>(player.get());
        skillArray.emplace_back(counterSkill.get());
        //3�i�U��
        tripleAttackSkill = std::make_unique<PlayerSkill::CanTripleAttack>(player.get());
        skillArray.emplace_back(tripleAttackSkill.get());
        //�z�[�~���O
        homingSkill = std::make_unique<PlayerSkill::ChangeHomingSkill>(player.get());
        skillArray.emplace_back(homingSkill.get());
        //�|�C�Y��
        poisonSkill = std::make_unique<PlayerSkill::PoisonSkill>(player.get());
        skillArray.emplace_back(poisonSkill.get());
        //�������̒e�̘A�ˑ��x�A�b�v
        bulletRateUpSkill = std::make_unique<PlayerSkill::BulletRateUp>(player.get());
        skillArray.emplace_back(bulletRateUpSkill.get());
        //�e�̃T�C�Y
        bulletSizeUpSkill = std::make_unique<PlayerSkill::BulletSizeUp>(player.get());
        skillArray.emplace_back(bulletSizeUpSkill.get());
        //�d�Ԃ�
        revengeSkill = std::make_unique<PlayerSkill::RevengeSkill>(player.get());
        skillArray.emplace_back(revengeSkill.get());
        

        return true;
    }();

    for (auto& skill : skillArray)
    {
        skill->Initialize();
    }

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

    for (auto& skill : skillArray)
    {
        skill->Update(elapsedTime);
    }

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

    ImGui::SliderInt("type", &tamaType, 0, 3);

    for (auto& skill : skillArray)
    {
        skill->DrawDebug();
    }
#endif // USE_IMGUI
}

void PlayerManager::SkillImagesRender()
{
    int i = 0;
    float posX = 1220.0f;
    for (auto& skill : skillArray)
    {
        skill->card->GetSpriteDissolve()->SetEdgeThreshold(0.0f);
        skill->Render();
        if (skill->GetOverlapNum() == 0)continue;
        skill->SetIconPos(DirectX::XMFLOAT2(posX, 0));
        posX -= skill->icon->GetSpriteTransform()->GetSize().x;
        i++;
    }
}

void PlayerManager::SkillImagesBloomRender()
{
    int i = 0;
    float posX = 1220.0f;
    for (auto& skill : skillArray)
    {
        skill->card->GetSpriteDissolve()->SetEdgeThreshold(0.1f);
        skill->Render();
        if (skill->GetOverlapNum() == 0)continue;
        skill->SetIconPos(DirectX::XMFLOAT2(posX, 0));
        posX -= skill->icon->GetSpriteTransform()->GetSize().x;
        i++;
    }
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

        if (player->GetStateMachine()->GetStateIndex() == Player::STATE::AVOID)
        {
            auto hitPos = enemy->GetTransform()->GetPosition();
            hitPos.y += 0.6f;
            enemy->ApplyDamage(0.1f * player->GetBasePower(), hitPos,nullptr,0.1f);
        }

#ifdef _DEBUG
        // ������΂��i���j
        {
#if 1
            // �G�����S�����Ƃ��̐������


            // ������ԕ����x�N�g��(�����K��)�Ɛ�����ԗ͂̓x������ۑ�
            //enemy->SaveBlowOffInfo(
            //    -vecFromEnemyToPlayer,
            //    BLOW_OFF_FORCE_LEVEL::MIDDLE
            //);
            //enemy->ApplyDamage(999999,enemy->GetTransform()->GetPosition()); // ���S�t���O�Ɛ�����уt���O�𗧂Ă�

#else
            // �C�ӂ̐�����΂�
            enemy->BlowOff(
                -vecFromEnemyToPlayer,
                BLOW_OFF_FORCE_LEVEL::MIDDLE
            );
#endif

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

bool PlayerManager::AttackCollisionPlayerToEnemy(std::vector<Enemy*>& hitEnemies, DirectX::XMFLOAT3& hitPos)
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
                        // ������я���ۑ�
                        const DirectX::XMFLOAT3 vec = enemy->GetPosition() - atkCollider.position;
                        enemy->SaveBlowOffInfo(vec, player->GetInflictBlowOffForceLevel());

                        hitPos = hitCollider.position;
                        hitEnemies.emplace_back(enemy);
                        
                        hit = true;
                    }
                }
            }
        }
    }
    return hit;
}

bool PlayerManager::HitCollisionPlayerToEnemy(std::vector<Enemy*>& hitEnemies, DirectX::XMFLOAT3& hitPos)
{
    bool hit = false;
    if (player->isActiveAttackFrame)
    {
        for (auto& enemy : EnemyManager::Instance().GetEnemies())
        {
            for (auto& atkCollider : player->GetHitCollider())
            {
                for (auto& hitCollider : enemy->GetHitCollider())
                {
                    if (Collision::IntersectSphereVsSphere(
                        atkCollider.position, atkCollider.radius,
                        hitCollider.position, hitCollider.radius))
                    {
                        // ������я���ۑ�
                        const DirectX::XMFLOAT3 vec = enemy->GetPosition() - atkCollider.position;
                        enemy->SaveBlowOffInfo(vec, player->GetInflictBlowOffForceLevel());

                        hitPos = hitCollider.position;
                        hitEnemies.emplace_back(enemy);
                        hit = true;
                    }
                }
            }
        }
    }
    return hit;
}
