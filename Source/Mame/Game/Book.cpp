#include "Book.h"
#include "BookState.h"

#include "../../Taki174/FunctionXMFloat3.h"
#include "../Graphics/Graphics.h"

#include "ProjectileStraight.h"
#include "ProjectileHorming.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "Collision.h"

int Book::totalNum = 0;

// コンストラクタ
Book::Book()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Item/Book.fbx");

    // ステートマシン
    {
        stateMachine.reset(new StateMachine<State<Book>>);

        GetStateMachine()->RegisterState(new BookState::IdleState(this));
        GetStateMachine()->RegisterState(new BookState::OpenState(this));
        GetStateMachine()->RegisterState(new BookState::JabAttackState(this));
        GetStateMachine()->RegisterState(new BookState::CloseState(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::Idle));
    }

    // Tagを設定
    tag = Tag::BOOK;

    // 現在 "Book" が何個生成されてるか知るためのもの
    num = ++totalNum;

    // ImGui名前設定
    SetName("Book" + std::to_string(nameNum++));
}

// デストラクタ
Book::~Book()
{
}

// 初期化
void Book::Initialize()
{
    GetTransform()->SetScaleFactor(3.0f);
}

// 終了化
void Book::Finalize()
{
}

// Updateの前に呼ばれる
void Book::Begin()
{
}

// 更新処理
void Book::Update(const float& elapsedTime)
{
    GetTransform()->SetScaleFactor(3.0f);

    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);

    // Transform設定
    SetTransform(elapsedTime);

    // 弾丸更新処理
    projectileManager.Update(elapsedTime);

    // アニメーション更新
    UpdateAnimation(elapsedTime);

    // 弾丸と敵との衝突処理
    CollisionProjectileVsEnemies();
}

// Updateの後に呼ばれる
void Book::End()
{
}

// 描画処理
void Book::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Item::Render(scale, psShader);

    // 弾丸描画処理
    projectileManager.Render(0.01f);
}

// ImGui用
void Book::DrawDebug()
{
#ifdef USE_IMGUI
    // ホーミング弾生成
    if (ImGui::Button("Projectile Horming"))
    {
        using DirectX::XMFLOAT3;
        const XMFLOAT3& position = this->GetTransform()->GetPosition();
        const XMFLOAT3  forward  = this->GetTransform()->CalcForward();
        ProjectileHorming* projHorming = new ProjectileHorming(&projectileManager, false); // 毒なし
        projHorming->Launch(position, forward);
    }

    if (ImGui::BeginMenu(GetName()))
    {
        Item::DrawDebug();

        projectileManager.DrawDebug();

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}

void Book::CollisionProjectileVsEnemies()
{
    using DirectX::XMFLOAT3;

    EnemyManager& enmManager = EnemyManager::Instance();

    const size_t projCount = projectileManager.GetProjectileCount();
    for (size_t projI = 0; projI < projCount; ++projI)
    {
        // 弾丸取得
        Projectile* proj = projectileManager.GetProjectile(projI);

        const size_t enmCount = enmManager.GetEnemyCount();
        for (size_t enmI = 0; enmI < enmCount; ++enmI)
        {
            // 敵取得
            Enemy* enemy = enmManager.GetEnemy(enmI);

            // 敵が死んでいたらcontinue
            if (true == enemy->GetIsDead()) continue;

            // 衝突フラグ
            bool isHit = false;

            const size_t enmHitColCount = enemy->GetHitCollider().size();
            for (size_t hitColI = 0; hitColI < enmHitColCount; ++hitColI)
            {
                // 敵の食らい判定取得
                const Character::SphereCollider enmHitCol = enemy->GetHitColliderAt(hitColI);

                const XMFLOAT3& projPos         = proj->GetPosition();
                const XMFLOAT3& enmHitColPos    = enmHitCol.position;
                const float     projRadius      = proj->GetRadius();
                const float     enmHitColRadius = enmHitCol.radius;

                isHit = Collision::IntersectSphereVsSphere(
                    projPos, projRadius, enmHitColPos, enmHitColRadius
                );

                // 当たってなければcontinue
                if (false == isHit) continue;

                // 敵の吹っ飛び情報を保存
                const XMFLOAT3 vec = enmHitColPos - projPos;
                enemy->SaveBlowOffInfo(vec, proj->GetInflictBlowOffForceLevel());

                // 敵にダメージを与える
                enemy->ApplyDamage(proj->GetAttack());

                // 弾を消去する
                proj->Destroy();

                // 当たったのでbreak
                break;
            }

            // 当たっていたらbreak
            if (true == isHit) break;
        }
    }
}

// 弾生成&発射
bool Book::LaunchProjectile(const float& elapsedTime)
{
    if (launchTimer <= 0.0f)
    {
        DirectX::XMFLOAT3 bookPosition = GetTransform()->GetPosition();
        DirectX::XMFLOAT3 bookForward = PlayerManager::Instance().GetPlayer()->GetTransform()->CalcForward();

        float length = 0.3f;

        bookPosition = { bookPosition.x + bookForward.x * length, bookPosition.y + bookForward.y * length, bookPosition.z + bookForward.z * length };

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(bookForward, bookPosition);

        // 発射までの時間を設定
        launchTimer = launchTime;

        return true; // 発射した
    }
    launchTimer -= elapsedTime;

    return false; // 発射してない
}


void Book::SetTransform(const float& elapsedTime)
{
    Transform* playerTransform = PlayerManager::Instance().GetPlayer()->GetTransform();
    DirectX::XMFLOAT3 playerPos = playerTransform->GetPosition();

    // プレイヤーの右ベクトル
    DirectX::XMFLOAT3 playerRightVec = playerTransform->CalcRight();
    // 長さ調整
    float length = 0.6f;
    playerRightVec = { playerRightVec.x * length, playerRightVec.y * length, playerRightVec.z * length };

    // 調整用
    float playerRightVecY = 1.0f;

    // １フレーム前の位置を保存する
    prevPosition = GetTransform()->GetPosition();

    switch (num)
    {
    case 0: break; // ０個の時は何もしない
    case 1: // プレイヤーの右
        // プレイヤーの右の位置を計算
        playerRightVec = { playerPos.x + playerRightVec.x, playerPos.y + playerRightVec.y , playerPos.z + playerRightVec.z };

        // いい感じになるように位置調整
        playerRightVec.y += playerRightVecY;

        break;

    case 2: // プレイヤーの左
        // プレイヤーの左の位置を計算
        playerRightVec = { playerPos.x - playerRightVec.x, playerPos.y - playerRightVec.y , playerPos.z - playerRightVec.z };

        // いい感じになるように位置調整
        playerRightVec.y += playerRightVecY;

        break;

    case 3: // プレイヤーの右上
        // プレイヤーの右の位置を計算
        {
            float l = 0.5f;
            playerRightVec = { playerRightVec.x * l, playerRightVec.y * l, playerRightVec.z * l };
            playerRightVec = { playerPos.x + playerRightVec.x, playerPos.y + playerRightVec.y , playerPos.z + playerRightVec.z };
        }

        // いい感じになるように位置調整
        playerRightVec.y += playerRightVecY + 0.5f;

    break;

    case 4: // プレイヤーの左上
        // プレイヤーの左の位置を計算
        {
            float l = 0.5f;
            playerRightVec = { playerRightVec.x * l, playerRightVec.y * l, playerRightVec.z * l };
            playerRightVec = { playerPos.x - playerRightVec.x, playerPos.y - playerRightVec.y , playerPos.z - playerRightVec.z };
        }

        // いい感じになるように位置調整
        playerRightVec.y += playerRightVecY + 0.5f;

        break;
    }

    // 生成位置
    float offset = 0.1f;

    createPosition = playerRightVec;
    createPosition.y = (prevPosition.y > playerRightVec.y - offset - 0.01f) ? prevPosition.y : playerRightVec.y;

    // ふわふわする処理
    {
        float moveY = isMoveToUp ? 0.3f : -0.3f;

        createPosition.y += moveY * elapsedTime;

        // 折り返し判定
        if (isMoveToUp)
        {   // 上限
            if (createPosition.y >= playerRightVec.y + offset)
            {
                createPosition.y = playerRightVec.y + offset;
                isMoveToUp = false;
            }
        }
        else
        {   // 下限
            if (createPosition.y <= playerRightVec.y - offset)
            {
                createPosition.y = playerRightVec.y - offset;
                isMoveToUp = true;
            }
        }
    }

    // 位置設定
    GetTransform()->SetPosition(createPosition);

    // 回転値
    DirectX::XMFLOAT4 bookRot = GetTransform()->GetRotation();
    if (bookRot.x > DirectX::XMConvertToRadians(360.0f))bookRot.x -= DirectX::XMConvertToRadians(360.0f);
    if (bookRot.x < DirectX::XMConvertToRadians(0.0f))bookRot.x += DirectX::XMConvertToRadians(360.0f);
    if (bookRot.y > DirectX::XMConvertToRadians(360.0f))bookRot.y -= DirectX::XMConvertToRadians(360.0f);
    if (bookRot.y < DirectX::XMConvertToRadians(0.0f))bookRot.y += DirectX::XMConvertToRadians(360.0f);
    if (bookRot.z > DirectX::XMConvertToRadians(360.0f))bookRot.z -= DirectX::XMConvertToRadians(360.0f);
    if (bookRot.z < DirectX::XMConvertToRadians(0.0f))bookRot.z += DirectX::XMConvertToRadians(360.0f);
    GetTransform()->SetRotation(bookRot);
}