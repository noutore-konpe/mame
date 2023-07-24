#include "EnemyCannon.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"

#include "PlayerManager.h"

int EnemyCannon::nameNum = 0;

// コンストラクタ
EnemyCannon::EnemyCannon()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_cannon.fbx", true);

    // ステートマシンをセット
    stateMachine.reset(new StateMachine);

    GetStateMachine()->RegisterState(new CANNON::IdleState(this));
    GetStateMachine()->RegisterState(new CANNON::AttackState(this));

    GetStateMachine()->SetState(static_cast<int>(CANNON::STATE::Idle));


    // imgui名前かぶりが起きないように...
    name = "EnemyCannon" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// デストラクタ
EnemyCannon::~EnemyCannon()
{
}

// 初期化
void EnemyCannon::Initialize()
{
}

// 終了化
void EnemyCannon::Finalize()
{
}

// Updateの前に呼ばれる
void EnemyCannon::Begin()
{
}

// 更新処理
void EnemyCannon::Update(const float& elapsedTime)
{
    // cannonBallManager
    cannonBallManager.Update(elapsedTime);

    if (stateMachine) GetStateMachine()->Update(elapsedTime);

    // AABB更新処理
    UpdateAABB();

    CollisionCannonBallVsPlayer(); // エネミー弾丸とプレイヤーの衝突処理
}

// Updateの後に呼ばれる
void EnemyCannon::End()
{
}

// 描画処理
void EnemyCannon::Render(const float& elapsedTime)
{
    Enemy::Render(elapsedTime);

    // cannonBallManager
    cannonBallManager.Render(elapsedTime);

}

// debug用
void EnemyCannon::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    model->GetTransform()->DrawDebug();

    if (stateMachine) GetStateMachine()->DrawDebug();

    cannonBallManager.DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}


void EnemyCannon::CollisionCannonBallVsPlayer()
{
    const int cannonBallCount = cannonBallManager.GetCannonBallCount();

    bool isHit = false;

    for (int i = 0; i < cannonBallCount; ++i)
    {
        CannonBall* cannonBall = cannonBallManager.GetCannonBall(i);

        const Collision::AABB& playerAABB = PlayerManager::Instance().GetPlayer()->aabb_;
        if (Collision::IntersectAABBVsAABB(cannonBall->aabb_, playerAABB))
        {
            isHit = true;
            // AABB1を押し出す
            //if (outPosition.max.x - outPosition.min.x < outPosition.max.y - outPosition.min.y)
            //{
            //    if (outPosition.max.x - box1.min.x < box1.max.x - outPosition.min.x)
            //    {
            //        box1.max.x = outPosition.min.x;
            //        GetTransform()->AddPosition(DirectX::XMFLOAT3(outPosition.min.x, 0, 0));
            //    }
            //    else
            //    {
            //        box1.min.x = outPosition.max.x;
            //        GetTransform()->AddPosition(DirectX::XMFLOAT3(outPosition.max.x, 0, 0));
            //    }
            //}
            //else
            //{
            //    if (outPosition.max.y - box1.min.y < box1.max.y - outPosition.min.y)
            //    {
            //        //box1.max.y = outPosition.min.y;
            //        GetTransform()->AddPosition(DirectX::XMFLOAT3(0, outPosition.min.y, 0));
            //    }
            //    else
            //    {
            //        //box1.min.y = outPosition.max.y;
            //        GetTransform()->AddPosition(DirectX::XMFLOAT3(0, outPosition.max.y, 0));
            //    }
            //}
            //if (outPosition.max.z - box1.min.z < box1.max.z - outPosition.min.z)
            //    box1.max.z = outPosition.min.z;
            //else
            //    box1.min.z = outPosition.max.z;   
        }
    }
}
