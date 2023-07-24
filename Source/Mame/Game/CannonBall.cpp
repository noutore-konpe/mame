#include "CannonBall.h"
#include "CannonBallManager.h"

#include "../Mame/Graphics/Graphics.h"
#include "PlayerManager.h"

int   CannonBall::nameNum             = 0;
float CannonBall::renderLengthXLimit_ = 40.0f;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="newPosition">大砲の位置</param>
/// <param name="directionZ">砲弾のすすむ方向Z</param>
CannonBall::CannonBall(
    NO_CONST DirectX::XMFLOAT3 newPosition, 
    const float& directionZ, 
    NO_CONST CannonBallManager* manager)
    : cannonBallManager(manager)
    , directionZ(directionZ)
{
    cannonBallManager->Register(this);

    Graphics& graphics = Graphics::Instance();

    // model読み込み
    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_cannon_bullet.fbx", true);

    // 初期座標を大砲の位置から少し前にする
    {
        offsetZ = (directionZ > 0) ? offsetZ : -offsetZ;
        newPosition.z += offsetZ;
        GetTransform()->SetPosition(newPosition);
    }

    // imgui名前かぶり起きないように...
    {
        name = "Boss" + std::to_string(nameNum);
        SetName(name.c_str());
        ++nameNum;
    }
}

// デストラクタ
CannonBall::~CannonBall()
{
}

// 初期化
void CannonBall::Initialize()
{
}

// 終了化
void CannonBall::Finalize()
{
}

// Updateの前に呼ばれる
void CannonBall::Begin()
{
}

// 更新処理
void CannonBall::Update(const float& elapsedTime)
{
    // 寿命処理
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0)
    {
        // 自分を消去
        Destroy();
        return;
    }

    float speed = (directionZ > 0) ? moveSpeed_ * elapsedTime : -moveSpeed_ * elapsedTime;

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    pos.z += speed;
    GetTransform()->SetPosition(pos);

    UpdateAABB();    // AABB更新処理
}

// Updateの後に呼ばれる
void CannonBall::End()
{
}

// 描画処理
void CannonBall::Render(const float& elapsedTime)
{
    // DrawCollを少なくするためにplayerから近いものだけ描画する
    {
        const float& enemyPosX  = GetTransform()->GetPosition().x;
        const float& playerPosX = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float  lengthX    = fabsf(enemyPosX - playerPosX);

        if (lengthX > renderLengthXLimit_) return;
    }

    Character::Render(elapsedTime);
}

// imgui
void CannonBall::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    Character::DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}

// 破棄
void CannonBall::Destroy()
{
    cannonBallManager->Remove(this);
}
