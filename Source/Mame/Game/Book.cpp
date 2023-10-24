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
        GetStateMachine()->RegisterState(new BookState::AttackState(this));
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

    // Transform設定
    SetTransform();
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

    // 円運動Z回転値更新(ふわふわ処理更新)
    circularMotionRotationZ_ += circularMotionAddRotate_ * elapsedTime;
    if (circularMotionRotationZ_ > 360.0f) { circularMotionRotationZ_ -= 360.0f; }

    // Transform設定
    SetTransform();

    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);

    // アニメーション更新
    UpdateAnimation(elapsedTime);

    // 弾丸更新処理
    projectileManager.Update(elapsedTime);

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
    for (size_t projIndex = 0; projIndex < projCount; ++projIndex)
    {
        // 弾丸取得
        Projectile* proj = projectileManager.GetProjectile(projIndex);

        const size_t enmCount = enmManager.GetEnemyCount();
        for (size_t enmIndex = 0; enmIndex < enmCount; ++enmIndex)
        {
            // 敵取得
            Enemy* enemy = enmManager.GetEnemy(enmIndex);

            // 敵が死んでいたらcontinue
            if (true == enemy->GetIsDead()) continue;

            // 衝突フラグ
            bool isHit = false;

            const size_t enmHitColliderCount = enemy->GetHitColliderCount();
            for (size_t enmHitColliderIndex = 0; enmHitColliderIndex < enmHitColliderCount; ++enmHitColliderIndex)
            {
                // 敵の食らい判定取得
                const Character::SphereCollider enmHitCol = enemy->GetHitColliderAt(enmHitColliderIndex);

                const XMFLOAT3& projPos              = proj->GetPosition();
                const XMFLOAT3& enmHitColliderPos    = enmHitCol.position;
                const float     projRadius           = proj->GetRadius();
                const float     enmHitColliderRadius = enmHitCol.radius;

                isHit = Collision::IntersectSphereVsSphere(
                    projPos, projRadius, enmHitColliderPos, enmHitColliderRadius
                );

                // 当たってなければcontinue
                if (false == isHit) continue;

                // 敵の吹っ飛び情報を保存
                const XMFLOAT3 vec = enmHitColliderPos - projPos;
                enemy->SaveBlowOffInfo(vec, proj->GetInflictBlowOffForceLevel());

                // 敵にダメージを与える
                enemy->ApplyDamage(proj->GetAttack(),enmHitColliderPos);

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


void Book::Turn(
    const float elapsedTime,
    float vx,
    float vz,
    float turnSpeed /*Degree*/)
{
    if (vx == 0.0f && vz == 0.0f) return;

    using DirectX::XMFLOAT3;
    using DirectX::XMFLOAT4;

    const float length = sqrtf(vx * vx + vz * vz);
    //if (length < 0.001f) return;

    vx /= length;
    vz /= length;

    XMFLOAT4 rotation = GetTransform()->GetRotation();
    const float frontX = ::sinf(rotation.y);
    const float frontZ = ::cosf(rotation.y);

    const float dot = (frontX * vx) + (frontZ * vz);

    // 回転角が微小な場合は回転を行わない
    const float angle = ::acosf(dot); // ラジアン
    if (::fabsf(angle) <= 0.01f) return;

    float rot = 1.0f - dot;
    turnSpeed = ::ToRadian(turnSpeed) * elapsedTime;
    if (rot > turnSpeed) rot = turnSpeed;

    //左右判定のための外積
    const float cross = (frontZ * vx) - (frontX * vz);

    rotation.y += (cross < 0.0f) ? -rot : rot;

    GetTransform()->SetRotation(rotation);
}


// 弾生成&発射
bool Book::LaunchProjectile(const float elapsedTime, const DirectX::XMFLOAT3& vec)
{
    using DirectX::XMFLOAT3;

    launchTimer -= elapsedTime;
    if (launchTimer <= 0.0f)
    {
        const XMFLOAT3 bookPos   = GetTransform()->GetPosition();
        const XMFLOAT3 vecN      = ::XMFloat3Normalize(vec);
        const float    length    = 0.3f;
        const XMFLOAT3 launchPos = bookPos + vecN * length;

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager,PlayerManager::Instance().GetPlayer().get());
        projectile->Launch(vecN, launchPos);

        // 発射までの時間を設定
        launchTimer = launchTime;

        return true; // 発射した
    }

    return false; // 発射してない
}


void Book::SetTransform()
{
    using DirectX::XMFLOAT3;

    Transform* playerT = PlayerManager::Instance().GetPlayer()->GetTransform();

    // プレイヤーの位置
    const XMFLOAT3& plPos = playerT->GetPosition();

    // プレイヤーの右ベクトル
    const XMFLOAT3 plRightVecN = playerT->CalcRight();

    // 移動量
    static const float MOVEMENT = 0.6f;

    // プレイヤーの右ベクトル方向に伸びた値(プレイヤーの位置に足す値)
    const XMFLOAT3 addPlPos = plRightVecN * MOVEMENT;

    // 位置調整値
    static const float MODIFY_POS_Y = 1.0f; // Y位置調整値
    static const float MODIFY_POS_X = 0.5f; // X位置調整値

    // １フレーム前の位置を保存する
    //prevPosition = GetTransform()->GetPosition();

    // この本の位置
    XMFLOAT3 bookPosition = {};

    // 割り振られた番号によって位置を設定する
    switch (num)
    {
    case 1: // プレイヤーの右
        // プレイヤーの右の位置を計算
        bookPosition = plPos + addPlPos;

        // いい感じになるようにY位置調整
        bookPosition.y += MODIFY_POS_Y;

        break;
    case 2: // プレイヤーの左
        // プレイヤーの左の位置を計算
        bookPosition = plPos - addPlPos;

        // いい感じになるようにY位置調整
        bookPosition.y += MODIFY_POS_Y;

        break;
    case 3: // プレイヤーの右上
        // プレイヤーの右の位置を計算
        bookPosition = plPos + (addPlPos * MODIFY_POS_X);

        // いい感じになるようにY位置調整
        bookPosition.y += MODIFY_POS_Y + 0.5f;

        break;
    case 4: // プレイヤーの左上
        // プレイヤーの左の位置を計算
        bookPosition = plPos - (addPlPos * MODIFY_POS_X);

        // いい感じになるようにY位置調整
        bookPosition.y += MODIFY_POS_Y + 0.5f;

        break;
    }

    // ふわふわする処理
    {
        static float timer;
        //float moveY = isMoveToUp ? 0.3f : -0.3f;
#if 1
    //// 回転更新
    //circularMotionRotationZ_ += circularMotionAddRotate_ * elapsedTime;
    //if (circularMotionRotationZ_ > 360.0f) { circularMotionRotationZ_ -= 360.0f; }


    // 上下に移動させる
        DirectX::XMFLOAT3 floatingModifyPos = bookPosition;
        floatingModifyPos.y = bookPosition.y + ::cosf(circularMotionRotationZ_) * circularMotionRadius_;

        // 位置設定
        GetTransform()->SetPosition(floatingModifyPos);
#else
    // 位置設定
        GetTransform()->SetPosition(bookPosition);
#endif

        // 回転値
        DirectX::XMFLOAT4 bookRot = GetTransform()->GetRotation();
        const float radian360 = ::ToRadian(360.0f);
        if (bookRot.x > +radian360) bookRot.x += -radian360;
        else if (bookRot.x < -radian360) bookRot.x += +radian360;
        if (bookRot.y > +radian360) bookRot.y += -radian360;
        else if (bookRot.y < -radian360) bookRot.y += +radian360;
        if (bookRot.z > +radian360) bookRot.z += -radian360;
        else if (bookRot.z < -radian360) bookRot.z += +radian360;
        GetTransform()->SetRotation(bookRot);

    }
}