#include "PlayerManager.h"
#include "EnemyManager.h"

#include "../../Taki174/FunctionXMFloat3.h"

#include "Collision.h"

PlayerManager::PlayerManager()
{
    
}

// 初期化
void PlayerManager::Initialize()
{

    static bool CallOnce = [&]() {
        //ドレイン
        drainSkill = std::make_unique<PlayerSkill::Drain>(player.get());
        skillArray.emplace_back(drainSkill.get());
        //移動速度アップ
        moveSpeedUpSkill = std::make_unique<PlayerSkill::MoveSpeedUp>(player.get());
        skillArray.emplace_back(moveSpeedUpSkill.get());
        //攻撃力アップ
        attackPowerUpSkill = std::make_unique<PlayerSkill::AttackPowerUp>(player.get());
        skillArray.emplace_back(attackPowerUpSkill.get());
        //攻撃速度アップ
        attackSpeedUpSkill = std::make_unique<PlayerSkill::AttackSpeedUp>(player.get());
        skillArray.emplace_back(attackSpeedUpSkill.get());
        //本の数増加
        bookIncreaseSkill = std::make_unique<PlayerSkill::BookIncrease>(player.get());
        skillArray.emplace_back(bookIncreaseSkill.get());
        //体力上限アップ
        maxHitPointUpSkill = std::make_unique<PlayerSkill::MaxHitPointUp>(player.get());
        skillArray.emplace_back(maxHitPointUpSkill.get());
        //防御力アップ
        defenseUpSkill = std::make_unique<PlayerSkill::DefenseUp>(player.get());
        skillArray.emplace_back(defenseUpSkill.get());
        //ブラックホール自動生成
        blackHoleSkill = std::make_unique<PlayerSkill::BlackHoleSkill>(player.get());
        skillArray.emplace_back(blackHoleSkill.get());
        //カウンター
        counterSkill = std::make_unique<PlayerSkill::CanCounterAttack>(player.get());
        skillArray.emplace_back(counterSkill.get());
        //3段攻撃
        tripleAttackSkill = std::make_unique<PlayerSkill::CanTripleAttack>(player.get());
        skillArray.emplace_back(tripleAttackSkill.get());
        //ホーミング
        homingSkill = std::make_unique<PlayerSkill::ChangeHomingSkill>(player.get());
        skillArray.emplace_back(homingSkill.get());
        //ポイズン
        poisonSkill = std::make_unique<PlayerSkill::PoisonSkill>(player.get());
        skillArray.emplace_back(poisonSkill.get());
        //魔導書の弾の連射速度アップ
        bulletRateUpSkill = std::make_unique<PlayerSkill::BulletRateUp>(player.get());
        skillArray.emplace_back(bulletRateUpSkill.get());
        //弾のサイズ
        bulletSizeUpSkill = std::make_unique<PlayerSkill::BulletSizeUp>(player.get());
        skillArray.emplace_back(bulletSizeUpSkill.get());
        //仕返し
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

// 終了化
void PlayerManager::Finalize()
{
    player->Finalize();
}

// Updateの前に呼ばれる
void PlayerManager::Begin()
{
    player->Begin();
}

// 更新処理
void PlayerManager::Update(const float& elapsedTime)
{
    player->Update(elapsedTime);

    for (auto& skill : skillArray)
    {
        skill->Update(elapsedTime);
    }

    //自機と敵の当たり判定
    CollisionPlayerVsEnemy();
}

// Updateの後に呼ばれる
void PlayerManager::End()
{
    player->End();
}

// 描画処理
void PlayerManager::Render(const float& scale, ID3D11PixelShader* psShader)
{
    player->Render(scale, psShader);
}

// ImGui用
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

        // 死んでいたら判定しない
        if (true == enemy->GetIsDead()) continue;

        const XMFLOAT3& enmPos    = enemy->GetPosition();
        const XMFLOAT3& plPos     = player->GetPosition();

        // 敵からプレイヤーに向かうベクトル
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
        // 吹っ飛ばし（仮）
        {
#if 1
            // 敵が死亡したときの吹っ飛び


            // 吹っ飛ぶ方向ベクトル(未正規化)と吹っ飛ぶ力の度合いを保存
            //enemy->SaveBlowOffInfo(
            //    -vecFromEnemyToPlayer,
            //    BLOW_OFF_FORCE_LEVEL::MIDDLE
            //);
            //enemy->ApplyDamage(999999,enemy->GetTransform()->GetPosition()); // 死亡フラグと吹っ飛びフラグを立てる

#else
            // 任意の吹っ飛ばし
            enemy->BlowOff(
                -vecFromEnemyToPlayer,
                BLOW_OFF_FORCE_LEVEL::MIDDLE
            );
#endif

        }
#endif

        const XMFLOAT3 vecN_fromEnemyToPlayer = XMFloat3Normalize(vecFromEnemyToPlayer);

        // CRA : 7.Collisition : 近接攻撃中じゃない方の敵を押し出す
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
                        // 吹っ飛び情報を保存
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
                        // 吹っ飛び情報を保存
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
