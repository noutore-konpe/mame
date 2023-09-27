#pragma once

#include "Character.h"
#include "StateMachine.h"

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

    void CameraControllerUpdate(float elapsedTime);
    
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // 描画処理
    
    void DrawDebug() override;  // ImGui用

private:
    //----------------------------カメラ関係----------------------------------
    float cameraRotSpeed = 2.0f;//旋回速度
    //-----------------------------------------------------------------------

    //--------------------------移動-----------------------------------------
    float moveSpeed = 3.0f;
    DirectX::XMFLOAT3 velocity{};
    //-----------------------------------------------------------------------


    //---------------------------ステートマシン-------------------------------
    StateMachine<State<Player>> stateMachine;
    //-----------------------------------------------------------------------
};

