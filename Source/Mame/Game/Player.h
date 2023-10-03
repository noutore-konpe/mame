#pragma once

#include "Character.h"
#include "StateMachine.h"
#include "../Input/Input.h"

class Player : public Character
{
public: // enum関連
    // ステート
    enum class STATE
    {
        NORMAL,   // 移動、待機等
        ATTACK,   // 攻撃
        AVOID,   // 回避
        DIE,   // 死亡
    };

private: // enum関連
    // アニメーション
    enum class Animation
    {
    };

public:
    Player();
    ~Player() override;

    void Initialize() override;                     // 初期化
    void Finalize();                                // 終了化

    void Begin();                                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End();                                     // 毎フレーム一番最後に呼ばれる

    void MoveUpdate(float elapsedTime);
    void UpdateVelocity(float elapsedTime);

    void CameraControllerUpdate(float elapsedTime);
    
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // 描画処理
    
    void DrawDebug() override;  // ImGui用

    static bool InputAttack()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_X);
    }

    static bool InputDash()
    {
        return (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_RIGHT_SHOULDER);
    }

private:
    //----------------------------カメラ関係----------------------------------
    float cameraRotSpeed = 2.0f;//旋回速度
    //-----------------------------------------------------------------------

    //--------------------------移動-----------------------------------------
    float maxSpeed = 3.0f;
    float maxDashSpeed = 4.0f;
    DirectX::XMFLOAT3 velocity{};

    //カメラの向いている方向を前とした移動方向ベクトル
    DirectX::XMFLOAT3 moveVec;

    float deceleration = 5.0f;
    float acceleration = 10.0f;
    //-----------------------------------------------------------------------


    //---------------------------ステートマシン-------------------------------
    StateMachine<State<Player>> stateMachine;
    //-----------------------------------------------------------------------
};

