#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../Transform.h"

class Enemy;

class Camera
{
private: // シングルトン化
    Camera()  {}
    ~Camera() {}

public:
    // 唯一のインスタンス取得
    static Camera& Instance()
    {
        static Camera camera;
        return camera;
    }

    void Initialize();

    void Update(float elapsedTime);

    void UpdateDebug(const float& elapsedTime, DirectX::XMFLOAT2 moveVector);

    void SetPerspectiveFov(ID3D11DeviceContext* dc);

    //画面振動
    void ScreenVibrate(float volume/*振動量*/, float effectTime/*振動時間*/);
    void ScreenVibrationUpdate(float elapsedTime);

    struct view_camera
    {
        DirectX::XMFLOAT3 eye{ 0.0f,0.0f,-10.0f };
        DirectX::XMFLOAT3 focus{ 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 up{ 0.0f,1.0f,0.0f };
    }camera;

    // ImGui用関数
    void DrawDebug();
    void Reset();

    void SetTraget(Transform* t) { focusTarget = t; }

    //引数の目標座標に向かってゆっくりカメラを移動させる処理
    void EyeMoveDelayUpdate(float elapsedTime,
        const DirectX::XMFLOAT3 eyeTargetPos);
    void FocusMoveDelayUpdate(float elapsedTime,
        const DirectX::XMFLOAT3 focusTargetPos);

private: // Debug用
    float moveSpeed = 0.01f;
    float rotationSpeed = 60.0f;

public:
    Transform* GetTransform() { return &transform; }        // transform取得
    DirectX::XMMATRIX GetViewMatrix() { return V; }         // view行列取得
    DirectX::XMMATRIX GetProjectionMatrix() { return P; }   // projection行列取得

    void SetViewMatrix(DirectX::XMMATRIX v) { V = v; }
    void SetProjectionMatrix(DirectX::XMMATRIX p) { P = p; }

    const DirectX::XMFLOAT3 GetForward()
    {
        if (activeLockOn && lockOnTarget &&
            lockOnForward.x + lockOnForward.y +lockOnForward.z != 0
            )return lockOnForward;
        return transform.CalcForward();
    }
    const DirectX::XMFLOAT3 GetRight()
    {
        if (activeLockOn && lockOnTarget)
        {
            DirectX::XMVECTOR Right = DirectX::XMVector3Cross(
                DirectX::XMLoadFloat3(&lockOnForward),
                //仮の上ベクトル　右ベクトルを取るためにマイナスにしている
                DirectX::XMVECTOR(DirectX::XMVectorSet(0,-1,0,0)));
            DirectX::XMFLOAT3 right;
            DirectX::XMStoreFloat3(&right,Right);
            return right;
        }
        return transform.CalcRight();
    }

    Enemy* GetLockOnTarget() const { return lockOnTarget; }
    void SetLockOnTarget(Enemy* enemy) { lockOnTarget = enemy; }

public:
    bool activeLockOn = false;//ロックオン起動
private:
    Transform transform{};

    DirectX::XMMATRIX V{};  // view行列
    DirectX::XMMATRIX P{};  // projection行列

    //プレイヤーからカメラまでの距離
    float focalLength = 7.0f;
    float offsetY = 2.7f;
    float focusOffsetY = 1.0f;

    float maxEyeSpeed = 8.0f;
    float maxFocusSpeed = 20.0f;

    Transform* focusTarget;//注視点になるオブジェクト

    Enemy* lockOnTarget;
    //Transform* lockOnTargetTransform;//ロックオン対象の座標
    DirectX::XMFLOAT3 lockOnForward;//ロックオン時の前方向ベクトル

    DirectX::XMFLOAT3 eyePos;
    DirectX::XMFLOAT3 focusPos = {0,0,1};

    float acceleration = 3.0f;
    float focusAcceleration = 9.0f;
    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 focusVelocity;

    bool enableDebugCamera = false;

    //画面振動
    DirectX::XMFLOAT3 screenVibrationOffset{};//振動表現用の座標
    float vibrationVolume;//振動量
    float vibrationTime;//振動時間
    float vibrationTimer;//振動時間を測るためのタイマー
};
