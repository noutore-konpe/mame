#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../Transform.h"

class Camera
{
private: // �V���O���g����
    Camera()  {}
    ~Camera() {}

public:
    // �B��̃C���X�^���X�擾
    static Camera& Instance()
    {
        static Camera camera;
        return camera;
    }

    void Initialize();

    void Update(float elapsedTime);

    void UpdateDebug(const float& elapsedTime, DirectX::XMFLOAT2 moveVector);

    void SetPerspectiveFov(ID3D11DeviceContext* dc);

    //��ʐU��
    void ScreenVibrate(float volume/*�U����*/, float effectTime/*�U������*/);
    void ScreenVibrationUpdate(float elapsedTime);
    
    struct view_camera
    {
        DirectX::XMFLOAT3 eye{ 0.0f,0.0f,-10.0f };
        DirectX::XMFLOAT3 focus{ 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 up{ 0.0f,1.0f,0.0f };
    }camera;

    // ImGui�p�֐�
    void DrawDebug();
    void Reset();

    void SetTraget(Transform* t) { focusTarget = t; }

    void MoveDelayUpdate(float elapsedTime);

private: // Debug�p
    float moveSpeed = 0.01f;
    float rotationSpeed = 60.0f;

public:
    Transform* GetTransform() { return &transform; }        // transform�擾
    DirectX::XMMATRIX GetViewMatrix() { return V; }         // view�s��擾
    DirectX::XMMATRIX GetProjectionMatrix() { return P; }   // projection�s��擾

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
    bool activeLockOn = false;//���b�N�I���N��
private:
    Transform transform{};

    DirectX::XMMATRIX V{};  // view�s��
    DirectX::XMMATRIX P{};  // projection�s��

    //�v���C���[����J�����܂ł̋���
    float focalLength = 7.0f;
    float offsetY = 2.7f;
    float focusOffsetY = 1.0f;

    float maxSpeed = 8.0f;

    Transform* focusTarget;//�����_�ɂȂ�I�u�W�F�N�g

    Transform* lockOnTargetTransform;//���b�N�I���Ώۂ̍��W
    DirectX::XMFLOAT3 lockOnForward;//���b�N�I�����̑O�����x�N�g��

    DirectX::XMFLOAT3 eyePos;
    float acceleration = 3.0f;
    DirectX::XMFLOAT3 velocity;

    bool enableDebugCamera = false;

    //��ʐU��
    DirectX::XMFLOAT3 screenVibrationOffset{};//�U���\���p�̍��W
    float vibrationVolume;//�U����
    float vibrationTime;//�U������
    float vibrationTimer;//�U�����Ԃ𑪂邽�߂̃^�C�}�[
};
