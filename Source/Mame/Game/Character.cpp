#include "Character.h"

#include "../Mame/Graphics/Graphics.h"
#include "../Mame/AudioManager.h"

#include "Terrain/TerrainManager.h"
#include "PlayerManager.h"

Character::Character()
{
    Graphics& graphics = Graphics::Instance();

    create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/wireframe.cso", pixel_shaders.GetAddressOf());

    geometricAABB_ = std::make_unique<GeometricAABB>(graphics.GetDevice(), defaultMin_, defaultMax_);
}


void Character::Render(const float& /*elapsedTime*/)
{
    Graphics& graphics = Graphics::Instance();

    // world行列更新
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (&model->keyframe)
    {
        model->skinned_meshes->render(graphics.GetDeviceContext(), transform, modelColor, &model->keyframe);
    }
    else
    {
        model->skinned_meshes->render(graphics.GetDeviceContext(), transform, modelColor, nullptr);
    }

#if _DEBUG
    // RS番号
    {
        // 0 ソリッド・後ろカリング
        // 1 ワイヤーフレーム・後ろカリング
        // 2 ワイヤーフレーム・カリングなし
        // 3 ソリッド・カリングなし
    }

    // ラスタライザ設定(ソリッド・カリングなし)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 3, 0, 0);

    // 回転なしワールド行列の作成
    NO_CONST DirectX::XMFLOAT4X4 noRotationTransform = {};
    {
        const DirectX::XMFLOAT3  scale      = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
        const DirectX::XMFLOAT3& position   = GetTransform()->GetPosition();
        const DirectX::XMMATRIX  S          = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
        const DirectX::XMMATRIX  T          = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMStoreFloat4x4(&noRotationTransform, S * T);
    }
    
    // AABB描画
    //geometricAABB_->render(graphics.GetDeviceContext(), noRotationTransform, materialColor);

    // ラスタライザ再設定(ソリッド・後ろカリング)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 0, 0, 0);

#endif // _DEBUG
}


void Character::DrawDebug()
{
    // ImGui描画
    GetTransform()->DrawDebug();
}


// ダメージを与える
bool Character::ApplyDamage(const int& damage, const float& invincibleTime)
{
    // ダメージが0の場合は健康状態を変更する必要がない
    if (damage <= 0) return false;

    // 死亡している場合は健康状態を変更しない
    if (health <= 0) return false;

    // 無敵時間が残っていたら健康状態を変更しない
    if (invincibleTimer > 0.0f) return false;

    // 無敵モードなら健康状態を変更しない
    if (isInvincible) return false;

    // ダメージ処理
    const int damagedHealth = health - damage;
    health = (damagedHealth > 0) ? damagedHealth : 0;

    // 無敵時間を設定
    invincibleTimer = invincibleTime;

    // 死亡通知
    if (health <= 0) OnDead();
    // ダメージ通知
    else OnDamaged();

    // 健康状態が変更した場合はtrueを返す
    return true;
}


// 移動処理
void Character::Move(
    const float& vx, 
    const float& moveSpeed)
{
    // 移動方向ベクトルを設定
    moveVecX_ = vx;

    // 最大速度設定
    maxMoveSpeed = moveSpeed; // elapsedTimeを乗算しない
}


// 旋回処理
bool Character::Turn(
    const    float& elapsedTime,
    NO_CONST float vx,  
    NO_CONST float turnSpeed)
{
    // 進行ベクトルがゼロベクトルの場合は処理する必要なし
    if (vx == 0.0f) return false;

    NO_CONST DirectX::XMFLOAT4 rotation = GetTransform()->GetRotation();

    turnSpeed *= elapsedTime;

    // 進行ベクトルを単位ベクトル化
    const float vLength = fabsf(vx);
    if (vLength < 0.001f) return false;

    // 単位ベクトル化
    vx /= vLength;

    // 自身の回転値から前方向を求める
    const float frontX = sinf(rotation.y);

    // 回転角を求めるため、2つの単位ベクトルの内積計算する
    const float dot = (frontX * vx) /*+ (frontZ * vz)*/;

    // 回転角が微小な場合は回転を行わない
    const float angle = acosf(dot); // ラジアン
    //if (fabsf(angle) <= 0.001f) return;
    if (fabsf(angle) <= 0.01f) return false;

    // 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
    // 小さいほど1.0に近づくという性質を利用して回転速度を調整する  
    NO_CONST float rot = 1.0f - dot;   // 補正値                    
    if (rot > turnSpeed) rot = turnSpeed;

    // 左右判定を行うために2つの単位ベクトルの外積を計算する
    const float frontZ = cosf(rotation.y);
    const float cross  = (frontZ * vx) /*- (frontX * vz)*/;

    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    // 左右判定を行うことによって左右回転を選択する
    rotation.y += (cross < 0.0f) ? -rot : rot;
    
    GetTransform()->SetRotation(rotation);

    return true;
}


// ジャンプ処理
void Character::Jump(const float& jumpSpeed)
{
    // 上方向の力を設定
    velocity.y = jumpSpeed;

    if (saveTerrain_) saveTerrain_ = nullptr; // 保存した地形情報を消去
}


// 速力更新処理
void Character::UpdateVelocity(const float& elapsedTime)
{
    // 経過フレーム（経過時間が変動した場合でも正常に計算されるようにする）
    const float elapsedFrame = 60.0f * elapsedTime;

    // 垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);

    // 垂直移動更新処理
    UpdateVerticalMove(elapsedTime);

    // 水平速力更新処理
    UpdateHorizontalVelocity(elapsedFrame);

    // 水平移動更新処理
    UpdateHorizontalMove(elapsedTime);
}


// AABB更新処理
void Character::UpdateAABB()
{
    const DirectX::XMFLOAT3& position = GetTransform()->GetPosition();

    aabb_.min = position + defaultMin_;
    aabb_.max = position + defaultMax_;
}


// 無敵時間更新処理
void Character::UpdateInvincibleTimer(const float& elapsedTime)
{
    //　無敵時間がなければ飛ばす
    if (invincibleTimer <= 0.0f) return;

    // 無敵時間を減らす
    invincibleTimer -= elapsedTime;
}


// 垂直速力更新処理
void Character::UpdateVerticalVelocity(const float& elapsedFrame)
{
    // 重力処理
    velocity.y += gravity * elapsedFrame;
    const float gravityMax = -18.0f;
    if (velocity.y < gravityMax) velocity.y = gravityMax;
}

// 垂直移動更新処理
void Character::UpdateVerticalMove(const float& elapsedTime)
{
    NO_CONST Transform* transform = GetTransform();

    // 垂直方向の移動量
    const float my = velocity.y * elapsedTime;

    // 落下中
    if (my < 0.0f)
    {
        // 垂直落下処理
        VerticalFall(my);
    }
    // 上昇中
    else if (my > 0.0f)
    {
        // 垂直上昇処理
        VerticalRise(my);
    }
}

#define USE_TERRAIN_MOVE_POSITION_MODIFY    // 動く地形に乗った時の位置修正オンオフ

// 垂直落下処理
void Character::VerticalFall(const float& fallSpeed)
{
    // 当たっているかどうか
    NO_CONST bool isHitY = false;

    // 地形に当たっているか衝突判定処理を行う
    {
        NO_CONST TerrainManager& terrainManager = TerrainManager::Instance();
        const int terrainCount = terrainManager.GetTerrainCount();
        for (int i = 0; i < terrainCount; ++i)
        {
            NO_CONST Terrain* terrain = terrainManager.GetTerrain(i);

            if (characterType_ == CharacterType::Player && !terrain->isCollisionPlayer_) continue; // プレイヤーと判定しない地形なら飛ばす
            if (characterType_ == CharacterType::Enemy  && !terrain->isCollisionEnemy_)  continue; // 敵と判定しない地形なら飛ばす

            if (Collision::IntersectAABBVsAABB(this->aabb_, terrain->aabb_))
            {
                // 上下左右にそれぞれ重なっている値を求める（エネミーの頭からプレイヤーの足元までの距離）
                // 符号がマイナスでもプラスの距離が求められるように絶対値にする
                const float overlapUp       = fabsf(aabb_.min.y - terrain->aabb_.max.y); // 上からめり込んでいる
                const float overlapBottom   = fabsf(aabb_.max.y - terrain->aabb_.min.y); // 下からめり込んでいる
                const float overlapRight    = fabsf(aabb_.min.x - terrain->aabb_.max.x); // 右からめり込んでいる
                const float overlapLeft     = fabsf(aabb_.max.x - terrain->aabb_.min.x); // 左からめり込んでいる
                // 一番小さい(重なっていない)値を求める(
                const float overlapY        = (std::min)(overlapUp, overlapBottom);
                const float overlapX        = (std::min)(overlapRight, overlapLeft);

                // X軸の方が重なりが小さい(めりこんでいる)ならcontinue
                if (overlapX < overlapY) continue;

                // 上への重なりがなければ位置を修正しない
                if (overlapUp == 0.0f)
                {
                    isHitY     = true; // 当たっている         
                    velocity.y = 0.0f; // Y速度をリセット


                    // バウンス中ならバウンスさせる
                    // バウンス回数をループ文で消費させないためにbreakしておく
                    if (isBounce)
                    {
                        OnBounce();
                    }
#ifdef USE_TERRAIN_MOVE_POSITION_MODIFY
                    else if (terrain->terrainType_ == Terrain::Type::Move)
                    {
                        saveTerrain_ = terrain;
                    }
#endif
                    // 着地した地形のAABBを保存
                    if (terrain->terrainType_ == Terrain::Type::Normal)
                    {
                        lastLandingTerrainAABB_ = terrain->aabb_;
                        saveTerrain_ = nullptr;
                    }            

                    // 地形の乗られた時の処理を行う
                    if (this == PlayerManager::Instance().GetPlayer().get())
                    {
                        terrain->OnRiding();    
                    }

                    continue;
                }

                // 下より上からの重なりの方が小さければ上からめり込んでいると判断し、
                // 重なっている分だけ押し戻す
                // ※上からのめり込みを優先して<=にしている
                if (overlapUp <= overlapBottom)
                {
                    isHitY      = true; // 当たっている   
                    velocity.y  = 0.0f; // Y速度をリセット

                    // 重なっている分だけ位置を修正
                    GetTransform()->AddPositionY(overlapUp);

                    // 押し戻し後のAABBの最小座標と最大座標を更新
                    UpdateAABB();

                    // バウンス中は跳ねさせる
                    if (isBounce)
                    {
                        OnBounce();
                    }
                    else
                    {
                        // 着地した
                        if (!isGround_) OnLanding();
                        isGround_ = true;

#ifdef USE_TERRAIN_MOVE_POSITION_MODIFY
                        if (terrain->terrainType_ == Terrain::Type::Move)
                        {
                            saveTerrain_ = terrain;
                        }
#endif
#undef USE_TERRAIN_MOVE_POSITION_MODIFY
                    }

                    // 着地した地形のAABBを保存
                    if (terrain->terrainType_ == Terrain::Type::Normal)
                    {
                        lastLandingTerrainAABB_ = terrain->aabb_;
                        saveTerrain_ = nullptr;
                    }

                    if (this == PlayerManager::Instance().GetPlayer().get())
                    {
                        terrain->OnRiding();    // 地形の乗られた時の処理を行う                        
                    }
                }
            }
        }
    }

    // 地形に当たっていなければ落下させる
    if (!isHitY)
    {
        GetTransform()->AddPositionY(fallSpeed);
        isGround_ = false;

        // それなりに下に落ちたら落下死・落下ミスしたときの処理を行う
        if (GetTransform()->GetPosition().y < -15.0f)
        {
            OnFallDead(); 
        }
        // 画面から見えなくなるくらいまで落ちたらX速度を0にする
        else if (GetTransform()->GetPosition().y < -3.0f)
        {
            if (this == PlayerManager::Instance().GetPlayer().get())
            {
                AudioManager::Instance().StopPlayerMoveSE();
            }

            velocity.x = 0.0f;
        }
    }
}

// 垂直上昇処理
void Character::VerticalRise(const float& riseSpeed)
{
    // 先に上昇速度を位置に加算しておく(地面と衝突判定させないため)
    GetTransform()->AddPositionY(riseSpeed);

    // AABB更新
    UpdateAABB(); 

    // 空中にいる
    isGround_ = false;

    // 地形に当たっているか衝突判定処理を行う
    {
        NO_CONST TerrainManager& terrainManager = TerrainManager::Instance();
        const int terrainCount = terrainManager.GetTerrainCount();
        for (int i = 0; i < terrainCount; ++i)
        {
            const Terrain* terrain = terrainManager.GetTerrain(i);

            if (characterType_ == CharacterType::Player && !terrain->isCollisionPlayer_) continue; // プレイヤーと判定しない地形なら飛ばす
            if (characterType_ == CharacterType::Enemy  && !terrain->isCollisionEnemy_)  continue; // 敵と判定しない地形なら飛ばす

            if (Collision::IntersectAABBVsAABB(aabb_, terrain->aabb_))
            {
                // 上下左右のそれぞれ重なっている値を求める
                // 符号がマイナスでもプラスの距離が求められるように絶対値にする
                const float overlapUp       = fabsf(aabb_.min.y - terrain->aabb_.max.y); // 上からめり込んでいる
                const float overlapBottom   = fabsf(aabb_.max.y - terrain->aabb_.min.y); // 下からめり込んでいる
                const float overlapRight    = fabsf(aabb_.min.x - terrain->aabb_.max.x); // 右からめり込んでいる
                const float overlapLeft     = fabsf(aabb_.max.x - terrain->aabb_.min.x); // 左からめり込んでいる
                // 一番小さい(重なっていない)値を求める
                const float overlapY        = (std::min)(overlapUp, overlapBottom);
                const float overlapX        = (std::min)(overlapRight, overlapLeft);

                // X軸の方が重なりが小さい(めりこんでいる)ならcontinue
                if (overlapX < overlapY) continue;

                // 上より下からの重なりの方が小さければ下からめり込んでいると判断し、
                // 重なっている分だけ押し戻す
                if (overlapBottom < overlapUp)
                {
                    velocity.y = 0.0f; // Y速度をリセット

                    // 重なっている分だけ位置を修正
                    GetTransform()->AddPositionY(-overlapBottom);

                    // 押し戻し後のAABBの最小座標と最大座標を更新
                    UpdateAABB();

                    break;
                }
            }
        }
    }
}


// 水平速力更新処理
void Character::UpdateHorizontalVelocity(const float& elapsedFrame)
{
    const float dist = sqrtf(velocity.x * velocity.x);

    // XZ平面の速力を減速する
    if (dist > 0.0f)
    {
        // 摩擦力
        float friction = this->friction_ * elapsedFrame;
        // 空中にいるときは摩擦力を減らす
        if (!isGround_) friction *= airControl;

        // 摩擦による横方向の減速処理
        if (dist > friction)
        {
            // 単位ベクトル化
            const float vx = velocity.x / dist;

            velocity.x -= vx * friction;
        }
        // 横方向の速力が摩擦力以下になったら速力を無効化
        else
        {
            velocity.x = 0.0f;
        }
    }


    // ダッシュしていたらダッシュ処理を行う
    if (isDash)
    {
        OnDash();
    }
    // X軸の速力を加速する
    else if (dist <= maxMoveSpeed)
    {
        // 移動ベクトルがゼロベクトルでないなら加速する
        const float moveVecDist = sqrtf(moveVecX_ * moveVecX_);
        if (moveVecDist > 0.0f)
        {
            // 加速力
            float acceleration = this->acceleration_ * elapsedFrame;

            // 空中にいるときは加速力を減らす
            if (!isGround_) acceleration *= airControl;

            // 移動ベクトルによる加速処理
            velocity.x += moveVecX_ * acceleration;

            // 最大速度制限
            const float dist = sqrtf(velocity.x * velocity.x);
            if (dist > maxMoveSpeed)
            {
                velocity.x = moveVecX_ * maxMoveSpeed;
            }

            //// 下り坂でガタガタしないようにする
            //if (isGround && slopeRate > 0.0f) // 着地時はスロープ対応
            //{
            //    velocity.y -= dist * slopeRate * elapsedFrame;
            //}
        }
    }

    // 移動ベクトルをリセット
    moveVecX_ = 0.0f;
}

// 水平移動更新処理
void Character::UpdateHorizontalMove(const float& elapsedTime)
{
    // X速度がゼロでも当たり判定が機能するように消している

    // 水平速力量計算
    //const float velocityLengthX = sqrtf(velocity.x * velocity.x);

    //if (velocityLengthX > 0.0f)
    {
        // 水平移動値
        const float mx = velocity.x * elapsedTime;

        HorizontalRightLeft(mx);
    }

}


// 水平移動処理
void Character::HorizontalRightLeft(NO_CONST float horizontalSpeed)
{
    // 当たっているかどうか
    NO_CONST bool isHitX = false;

    // 地形に当たっているか衝突判定処理を行う
    {
        NO_CONST TerrainManager& terrainManager = TerrainManager::Instance();
        const int terrainCount = terrainManager.GetTerrainCount();
        for (int i = 0; i < terrainCount; ++i)
        {
            NO_CONST Terrain* terrain = terrainManager.GetTerrain(i);

            if (characterType_ == CharacterType::Player && !terrain->isCollisionPlayer_) continue; // プレイヤーと判定しない地形なら飛ばす
            if (characterType_ == CharacterType::Enemy  && !terrain->isCollisionEnemy_)  continue; // 敵と判定しない地形なら飛ばす

            if (Collision::IntersectAABBVsAABB(aabb_, terrain->aabb_))
            {
                // 上下左右のそれぞれ重なっている値を求める
                // 符号がマイナスでもプラスの距離が求められるように絶対値にする
                const float overlapUp       = fabsf(aabb_.min.y - terrain->aabb_.max.y); // 上からめり込んでいる
                const float overlapBottom   = fabsf(aabb_.max.y - terrain->aabb_.min.y); // 下からめり込んでいる
                const float overlapRight    = fabsf(aabb_.min.x - terrain->aabb_.max.x); // 右からめり込んでいる
                const float overlapLeft     = fabsf(aabb_.max.x - terrain->aabb_.min.x); // 左からめり込んでいる
                // 一番重なっていない値を求める
                const float overlapY        = (std::min)(overlapUp, overlapBottom);
                const float overlapX        = (std::min)(overlapRight, overlapLeft);

                // Y軸の方が重なりが小さい(めりこんでいる)ならcontinue
                if (overlapY < overlapX) continue;

                // 横への重なりがなければ修正しない
                if (overlapX == 0.0f) continue;

                // バウンス中なら壁に当たった時にX速度を反転させる
                if (isBounce)
                {                  
                    horizontalSpeed = -horizontalSpeed; // 今回の速度を反転
                    velocity.x      = -velocity.x;      // 跳ね返って空中に浮いてるときのX速度も反転させる
                    saveMoveVecX_    = -saveMoveVecX_;  // 向いている方向を逆にさせる

                    AudioManager& audioManager = AudioManager::Instance();
                    audioManager.StopSE(SE::PL_Bounce);                 // バウンスSE停止
                    audioManager.PlaySE(SE::PL_Bounce, false, true);    // バウンスSE再生
                }
                else
                {
                    velocity.x  = 0.0f;  // X速度をリセット
                    isHitX      = true;  // 当たっている   
                }

                // 左よりより右からの重なりの方が小さければ右からめり込んでいて、
                // 逆に右より左からの重なりの方が小さければ左からめり込んでいると判断し、
                // 重なっている分だけ位置を押し戻す
                // ※右からのめり込みを優先して<=にしている
                if (overlapRight <= overlapLeft)
                {
                    GetTransform()->AddPositionX(overlapRight);
                }
                else if (overlapLeft < overlapRight)
                {
                    GetTransform()->AddPositionX(-overlapLeft);
                }

                // 押し戻し後のAABBの最小座標と最大座標を更新
                UpdateAABB();

                // 壁に当たった時の処理
                if (!isHitWall_)
                {
                    saveWall_ = terrain; // 当たった壁のAABBを保存
                    OnHitWall();
                }
                isHitWall_ = true;

                break;
            }
        }
    }
    
    // 地形に衝突していなければ位置にX速度を加算
    if (!isHitX || isBounce)
    {
        // 移動処理
        GetTransform()->AddPositionX(horizontalSpeed);
        isHitWall_ = false;
    }
}


void Character::SetAABB(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
    // min・maxの再設定
    defaultMin_ = min;
    defaultMax_ = max;

    // 当たり判定のAABB描画を再設定
    ID3D11Device* device = Graphics::Instance().GetDevice();
    geometricAABB_ = std::make_unique<GeometricAABB>(device, defaultMin_, defaultMax_);
}
