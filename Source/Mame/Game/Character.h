#pragma once

#include "../Graphics/Model.h"

//#ifdef USE_IMGUI
//#include "../../External/imgui/imgui.h"
//#include "../../External/imgui/imgui_internal.h"
//#include "../../External/imgui/imgui_impl_dx11.h"
//#include "../../External/imgui/imgui_impl_win32.h"
//#endif

#include "../Resource/GeometricPrimitive.h"
#include "../Resource/sprite.h"

#include <DirectXMath.h>

#include "Common.h"

#include "Collision.h"

#include "Terrain/Terrain.h"

enum class CharacterType
{
    None,
    Player,
    Enemy,
};

class Character
{
public:
    Character();
    virtual ~Character() {}

    virtual void Render(const float& elapsedTime);  // 描画処理
    virtual void DrawDebug();                       // デバッグ描画

    void UpdateAABB();                              // AABB更新処理


public:
    // ダメージを与える
    virtual bool ApplyDamage(
        const int& damage, 
        const float& invincibleTime
    );

protected:
    // 移動処理
    void Move(const float& vx, const float& moveSpeed);

    // 旋回処理
    bool Turn(
        const float& elapsedTime, 
        NO_CONST float vx, 
        NO_CONST float turnSpeed
    );

    // ジャンプ処理
    void Jump(const float& jumpSpeed);

protected:
    void UpdateVelocity(const float& elapsedTime);              // 速力処理更新処理
    void UpdateInvincibleTimer(const float& elapsedTime);       // 無敵時間更新処理

protected:
    virtual void OnLanding()  {}                             // 着地したときに呼ばれる  
    virtual void OnDash()     {}                             // ダッシュしているときに呼ばれる
    virtual void OnBounce()   {}                             // バウンスするときに呼ばれる
    virtual void OnDamaged()  {}                             // ダメージを受けたときに呼ばれる  
    virtual void OnDead()     {}                             // 死亡したときに呼ばれる
    virtual void OnFallDead() {}                             // 落下死・落下ミスしたときに呼ばれる
    virtual void OnHitWall()  {}                             // 壁に当たった時の処理

private: 
    void UpdateVerticalVelocity(const float& elapsedFrame);     // 垂直速力更新処理
    void UpdateVerticalMove(const float& elapsedTime);          // 垂直移動更新処理

    void VerticalFall(const float& fallSpeed);                  // 垂直落下処理(コードが長くなったので落下時の処理をこちらに移動)
    void VerticalRise(const float& riseSpeed);                  // 垂直上昇処理(コードが長くなったので上昇時の処理をこちらに移動)

    void UpdateHorizontalVelocity(const float& elapsedFrame);   // 水平速力更新処理
    void UpdateHorizontalMove(const float& elapsedTime);        // 水平移動更新処理

    void HorizontalRightLeft(NO_CONST float horizontalSpeed);     // 水平移動処理(コードが長くなったので水平移動時の処理をこちらに移動)


public: // アニメーション処理関数関連（ショートカット関数）

    // アニメーション再生設定
    void PlayAnimation(
        const int& index,
        const bool& loop,
        const float& speed = 1.0f,
        const float& blendSeconds = 1.0f)
    {
        model->PlayAnimation(index, loop, speed, blendSeconds);
    }

    // アニメーション更新処理
    void UpdateAnimation(const float& elapsedTime) { model->UpdateAnimation(elapsedTime); }

    // アニメーションが再生中かどうか
    bool IsPlayAnimation() const { return model->IsPlayAnimation(); }

    // アニメーション再生速度設定（途中で再生速度を変えたいときなどに）
    void SetAnimationSpeed(const float& speed) { model->SetAnimationSpeed(speed); }

public: // 取得・設定関数関連

    // トランスフォーム取得
    Transform* GetTransform() const { return model->GetTransform(); }

    // カラー設定
    const DirectX::XMFLOAT4& SetMaterialColor() const { return materialColor; }
    void SetMaterialColor(const DirectX::XMFLOAT4& color) { materialColor = color; }

    // 速度の設定
    void SetVelocityX(const float velocityX) { velocity.x = velocityX; }
    void SetVelocityY(const float velocityY) { velocity.y = velocityY; }
    void SetVelocityZ(const float velocityZ) { velocity.z = velocityZ; }

    // モデルカラーの取得・設定
    const DirectX::XMFLOAT4& GetModelColor() const { return modelColor; }
    const float GetModelColorAlpha(const float colorAlpha) { modelColor.w = colorAlpha; }
    void SetModelColorAlpha(const float colorAlpha) { modelColor.w = colorAlpha; }

    // 保存した移動方向ベクトルの取得
    const float GetSaveMoveVecX() const { return saveMoveVecX_; }

    // 移動速度の取得・設定
    const float GetMoveSpeed() const { return moveSpeed_; }
    void SetMoveSpeed(const float speed) { moveSpeed_ = speed; }    
    
    // 加速度の取得・設定
    const float GetAcceleration() const { return acceleration_; }
    void SetAcceleration(const float acceleration) { acceleration_ = acceleration; }    
    
    // 摩擦の取得・設定
    const float GetFriction() const { return friction_; }
    void SetFriction(const float friction) { friction_ = friction; }

    // 旋回速度の取得・設定
    const float GetTurnSpeed() const { return turnSpeed_; }
    void SetTurnSpeed(const float turnSpeed) { turnSpeed_ = turnSpeed; }

    // ジャンプ速度の取得・設定
    const float GetJumpSpeed() const { return jumpSpeed_; }
    void SetJumpSpeed(const float jumpSpeed) { jumpSpeed_ = jumpSpeed; }


    // 体力の取得
    const int GetHealth() const { return health; }
    void SetHealth(const int hp) { health = hp; }

    // 地面についているかどうかの取得・設定
    const bool& GetIsGround() const { return isGround_; }
    void SetIsGround(const bool isGround) { isGround_ = isGround; }

    // 無敵かどうかの取得・設定
    const bool GetIsInvincible() const { return isInvincible; }
    void SetIsInvincible(const bool invincible) { isInvincible = invincible; }

    // AABB設定(当たり判定のサイズを途中で変えたいときなどに)
    void SetAABB(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);

public:
    std::unique_ptr<Model> model    =  nullptr;                     // モデル

    std::unique_ptr<GeometricPrimitive> geometricAABB_ = nullptr;   // 当たり判定AABB描画用ジオメトリックプリミティブ
    Collision::AABB aabb_                   = {};                   // 当たり判定AABB
    Collision::AABB lastLandingTerrainAABB_ = {};                   // 最後に着地した地形のAABB（落下ミスの復活時に使用）
    Terrain*        saveTerrain_            = nullptr;              // 地形を保存するポインタ
    Terrain*        saveWall_               = nullptr;              // 壁のAABBを保存するAABB

    CharacterType   characterType_          = CharacterType::None;

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders;

    DirectX::XMFLOAT4 modelColor    =   { 1.0f, 1.0f, 1.0f, 1.0f }; // モデルカラー
    DirectX::XMFLOAT4 materialColor =   { 1.0f, 0.0f, 0.0f, 0.4f }; // マテリアルカラー

    DirectX::XMFLOAT3 velocity      =   { 0,0,0 };                  // 速度
         
    float       stepOffset          =   1.0f;                       // 位置補正(Y位置がキャラクターの中心になるように調整)
                                        
    float       moveVecX_           =   0.0f;                       // 移動ベクトルX
    float       saveMoveVecX_        =   1.0f;                       // 移動ベクトルを保存するベクトルX（最初は右を向かせておく）

    float       defaultAcceleration =   1.0f;                       // 加速力の初期値
    float       acceleration_       =   defaultAcceleration;        // 加速力
    float       defaultGravity      =  -1.0f;                       // 重力初期値
    float       gravity             =   defaultGravity;             // 重力
    float       defaultFriction     =   0.5f;                       // 摩擦力の初期値
    float       friction_           =   0.5f;                       // 摩擦力
    float       airControl          =   0.3f;                       // 空中制御

    float       defaultMoveSpeed    =  5.0f;                        // 移動速度初期値
    float       moveSpeed_          =  defaultMoveSpeed;            // 移動速度(最大移動速度に代入される)
    float       turnSpeed_          =  ToRadian(900.0f);            // 旋回速度(180.f * 5)
    float       jumpSpeed_           = 10.0f;                       // ジャンプ速度
    float       maxMoveSpeed        =  5.0f;                        // 最大移動速度

    float       slopeRate           =   1.0f;                       // 傾斜率
                                        
    float       invincibleTimer     =   0.0f;                       // 無敵時間

    float       defaultJumpTime     =   0.3f;                       // ジャンプ時間
    float       jumpTimer           =   0.0f;                       // ジャンプタイマー

    int         health              =   3;                          // 体力
    int         jumpCount           =   0;                          // ジャンプ回数
    int         jumpLimit           =   1;                          // 最大ジャンプ回数
                                       
    bool        isGround_            =   false;                      // 地面についているか
    bool        isHitWall_          =   false;                      // 壁に当たっているか
    bool        isBounce            =   false;                      // バウンスさせるか
    bool        isDash              =   false;                      // ダッシュしているか
    bool        isInvincible        =   false;                      // 無敵かどうか(ボスに使う)

private:
    // 大きさの変更をするときに当たり判定のAABB描画の大きさも変更させるためにprivateにしている
    DirectX::XMFLOAT3 defaultMin_   = { -0.4f, -0.0f, -0.4f };       // 当たり判定のminデフォルト値
    DirectX::XMFLOAT3 defaultMax_   = { +0.4f, +0.8f, +0.4f };       // 当たり判定のmaxデフォルト値
};

