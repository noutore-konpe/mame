#include "Enemy.h"

#include "../../Taki174/FunctionXMFloat3.h"
#include "EnemyManager.h"
#include "WaveManager.h"
#include "PlayerManager.h"

// 方向マーカーを使用するか
#define USE_DIRECTION_MARKER (1)

Enemy::Enemy()
{
    Graphics& graphics = Graphics::Instance();

    // 方向マーカー生成
#if USE_DIRECTION_MARKER
    directionMarker_ = std::make_unique<Sprite>(
        graphics.GetDevice(),
        L"./Resources/Image/UI/DirectionMarker.png"
    );
#endif

}

Enemy::~Enemy()
{
    // ウェーブで管理されていた敵なら残りのウェーブエネミーカウントを１減らす
    if (true == isWaveEnemy_)
    {
        WaveManager::Instance().ReduceRemainingEnemyCounter();
    }
}

void Enemy::Initialize()
{
    Character::Initialize();

    // 方向マーカー初期設定
    InitializeDirectionMarker();
}

void Enemy::Update(const float& elapsedTime)
{
    Character::Update(elapsedTime);

    // 方向マーカー更新
    UpdateDirectionMarker(elapsedTime);
}

void Enemy::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader);
}

void Enemy::RenderDirectionMarker()
{
    if (nullptr == directionMarker_) { return; }

    Camera& camera = Camera::Instance();

    // ロックオン中は描画しない（仮）
    if (true == camera.GetActiveLockOn()) { return; }

    directionMarker_->Render();
}

void Enemy::InitializeDirectionMarker()
{
    if (nullptr == directionMarker_) { return; }

    using DirectX::XMFLOAT4;
    Sprite::SpriteTransform* directionMarkerT = directionMarker_->GetSpriteTransform();

    // 赤色に設定
    const XMFLOAT4 color = { 1.0f, 0.25f, 0.0f, 1.0f };
    directionMarkerT->SetColor(color);

    // サイズ調整
    directionMarkerT->SetSizeX(directionMarkerT->GetSizeX() * 0.1f);
    directionMarkerT->SetSizeY(directionMarkerT->GetSizeY() * 0.1f);

}

void Enemy::UpdateDirectionMarker(const float elapsedTime)
{
    if (nullptr == directionMarker_) { return; }

    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT3;

    Player* player = PlayerManager::Instance().GetPlayer().get();
    Camera& camera = Camera::Instance();
    Sprite::SpriteTransform* directionMarkerT = directionMarker_->GetSpriteTransform();

    const XMFLOAT3& plPos = player->GetTransform()->GetPosition();
    const XMFLOAT3& enemyPos = this->GetTransform()->GetPosition();
    const XMFLOAT2  screenPlayerPos = Sprite::ConvertToScreenPos(plPos);
    const float     vecX = enemyPos.x - plPos.x;
    const float     vecZ = enemyPos.z - plPos.z;
    const float     lengthXZ_Sq = (vecX * vecX + vecZ * vecZ);

    const float cameraRotationY = camera.GetTransform()->GetRotationY();
    //float angleRadian = {
    //    ::atan2f(vecX, vecZ) +
    //    ((true == camera.GetActiveLockOn())
    //     ? /*要改善*/(-cameraRotationY)
    //     : (-cameraRotationY))
    //};
    float angleRadian = ::atan2f(vecX, vecZ) + (-cameraRotationY);
    if      (angleRadian < 0.0f)   angleRadian += ::ToRadian(360.0f);
    else if (angleRadian > 360.0f) angleRadian += ::ToRadian(-360.0f);

    // 位置更新
    {
        static constexpr float RADIUS = 120.0f;
        //const XMFLOAT2 centerPos =
        //    (true == camera.GetActiveLockOn())
        //    ? Sprite::ConvertToScreenPos(camera.GetLockOnTarget()->GetPosition())
        //    : screenPlayerPos;
        const XMFLOAT2 centerPos = screenPlayerPos;
        directionMarkerT->SetPosX(screenPlayerPos.x + ::sinf(angleRadian) * RADIUS);
        directionMarkerT->SetPosY(screenPlayerPos.y + (-::cosf(angleRadian)) * RADIUS);

        // 中心に位置調整
        directionMarkerT->SetSpriteCenterPos(directionMarkerT->GetPos());
    }

    // 角度更新
    {
        const float angleDegree = DirectX::XMConvertToDegrees(angleRadian);
        directionMarkerT->SetAngle(angleDegree);
    }

    //　不透明度更新
    {
        // マーカーを表示する距離
        static constexpr float DISP_LENGTH = 15.0f;
        static constexpr float DISP_LENGTH_SQ = DISP_LENGTH * DISP_LENGTH;

        // プレイヤーと敵との距離で不透明度を調節する
        const float colorA = {
            (lengthXZ_Sq <= DISP_LENGTH_SQ)
            ? 1.0f - (lengthXZ_Sq / DISP_LENGTH_SQ)
            : 0.0f
        };

        directionMarkerT->SetColorA(colorA);
    }

}

// 吹っ飛ばし情報の保存
void Enemy::SaveBlowOffInfo(
    const DirectX::XMFLOAT3& blowOffVec,
    const BLOW_OFF_FORCE_LEVEL& blowOffForceLevel)
{
    blowOffVec_        = blowOffVec;
    blowOffForceLevel_ = blowOffForceLevel;
}

void Enemy::Destroy()
{
    // 自分を消去
    EnemyManager::Instance().Remove(this);
}

// 検索する子ノードが存在するか検索する
const bool Enemy::IsExistChildNode(const std::string& findNodeName)
{
    std::vector<NodeBase*>* Nodes = behaviorTree_->GetRoot()->GetChildren();

    for (size_t i = 0; i < Nodes->size(); ++i)
    {
        // 探しているノードと名前が一致したらtrueを返す
        if (findNodeName == Nodes->at(i)->GetName())
        {
            return true;
        }
    }

    return false;
}

void Enemy::AttackCollisionOnPlayer(const float damage)
{
    DirectX::XMFLOAT3 pos;
    if (EnemyManager::AttackCollisionPlayerToEnemy(this,pos))
    {
        PlayerManager::Instance().GetPlayer()->ApplyDamage(damage,pos,Player::HitReaction::SOFT,this,0.2f);
    }
}

#undef USE_DIRECTION_MARKER