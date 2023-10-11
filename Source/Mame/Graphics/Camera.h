#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../Transform.h"

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

    void MoveDelayUpdate(float elapsedTime);

private: // Debug用
    float moveSpeed = 0.01f;
    float rotationSpeed = 60.0f;

public:
    Transform* GetTransform() { return &transform; }        // transform取得
    DirectX::XMMATRIX GetViewMatrix() { return V; }         // view行列取得
    DirectX::XMMATRIX GetProjectionMatrix() { return P; }   // projection行列取得

    void SetViewMatrix(DirectX::XMMATRIX v) { V = v; }
    void SetProjectionMatrix(DirectX::XMMATRIX p) { P = p; }

    void SetLockOnTargetPos(Transform* transform) { lockOnTargetTransform = transform; }
    const DirectX::XMFLOAT3 GetForward()
    {
        if (activeLockOn)return lockOnForward;
        return transform.CalcForward();
    }
    const DirectX::XMFLOAT3 GetRight()
    {
        if (activeLockOn)
        {
            DirectX::XMVECTOR Right = DirectX::XMVector3Cross(
                DirectX::XMLoadFloat3(&lockOnForward),
                DirectX::XMVECTOR(DirectX::XMVectorSet(0,-1,0,0)));
            DirectX::XMFLOAT3 right;
            DirectX::XMStoreFloat3(&right,Right);
            return right;
        }
        return transform.CalcRight();
    }

    Transform* GetLockOnTarget() const { return lockOnTargetTransform; }
    void SetLockOnTarget(Transform* transform) { lockOnTargetTransform = transform; }

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

    float maxSpeed = 8.0f;

    Transform* focusTarget;//注視点になるオブジェクト

    Transform* lockOnTargetTransform;//ロックオン対象の座標
    DirectX::XMFLOAT3 lockOnForward;//ロックオン時の前方向ベクトル

    DirectX::XMFLOAT3 eyePos;
    float acceleration = 3.0f;
    DirectX::XMFLOAT3 velocity;

    bool enableDebugCamera = false;

    //画面振動
    DirectX::XMFLOAT3 screenVibrationOffset{};//振動表現用の座標
    float vibrationVolume;//振動量
    float vibrationTime;//振動時間
    float vibrationTimer;//振動時間を測るためのタイマー
};
