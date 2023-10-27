#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../Transform.h"

class Enemy;

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

public:
    enum class STATE
    {
        Wait,
        Move0,
        Move1,
        Move2,
        Move3,
        Move4,
        Move5,
    };

    void TitleInitialize();
    void TitleUpdate(const float& elapsedTime);
    void TitleSetPerspectiveFov(ID3D11DeviceContext* dc);

    void ResultInitialize();
    void ResultSetPerSpectiveFov(ID3D11DeviceContext* dc);

    int titleState = 0;
    bool isFocusCenter = false;
    float easingTimer = 0.0f;
    float angle = 0.0f;
    float length = 27.0f;

public:
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

    //�����̖ڕW���W�Ɍ������Ă������J�������ړ������鏈��
    void EyeMoveDelayUpdate(float elapsedTime,
        const DirectX::XMFLOAT3 eyeTargetPos);
    void FocusMoveDelayUpdate(float elapsedTime,
        const DirectX::XMFLOAT3 focusTargetPos);

private: // Debug�p
    float moveSpeed = 0.01f;
    float rotationSpeed = 60.0f;

public:
    Transform* GetTransform() { return &transform; }        // transform�擾
    DirectX::XMMATRIX GetViewMatrix() { return V; }         // view�s��擾
    DirectX::XMMATRIX GetProjectionMatrix() { return P; }   // projection�s��擾

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
        if (activeLockOn && lockOnTarget &&
            lockOnForward.x + lockOnForward.y + lockOnForward.z != 0)
        {
            DirectX::XMVECTOR Right = DirectX::XMVector3Cross(
                DirectX::XMLoadFloat3(&lockOnForward),
                //���̏�x�N�g���@�E�x�N�g������邽�߂Ƀ}�C�i�X�ɂ��Ă���
                DirectX::XMVECTOR(DirectX::XMVectorSet(0,-1,0,0)));
            DirectX::XMFLOAT3 right;
            DirectX::XMStoreFloat3(&right,Right);
            return right;
        }
        return transform.CalcRight();
    }

    Enemy* GetLockOnTarget() const { return lockOnTarget; }
    void SetLockOnTarget(Enemy* enemy) { lockOnTarget = enemy; }

    [[nodiscard]] const bool GetActiveLockOn() const { return activeLockOn; }

    void CancelLockOn();//���b�N�I������

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

    float maxEyeSpeed = 15.0f;
    float maxFocusSpeed = 20.0f;

    Transform* focusTarget;//�����_�ɂȂ�I�u�W�F�N�g

    Enemy* lockOnTarget;
    //Transform* lockOnTargetTransform;//���b�N�I���Ώۂ̍��W
    DirectX::XMFLOAT3 lockOnForward;//���b�N�I�����̑O�����x�N�g��

    DirectX::XMFLOAT3 eyePos;
    DirectX::XMFLOAT3 focusPos = {0,0,1};

    float acceleration = 12.0f;
    float focusAcceleration = 9.0f;
    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 focusVelocity;

    bool enableDebugCamera = false;

    //��ʐU��
    DirectX::XMFLOAT3 screenVibrationOffset{};//�U���\���p�̍��W
    float vibrationVolume;//�U����
    float vibrationTime;//�U�����ԃG�~�b�^�[
    float vibrationTimer;//�U�����Ԃ𑪂邽�߂̃^�C�}�[

    //����p
    float fov;
    bool isChangingFov;
    float fovTimer;
    float fovTime;
    float initFov;
    float holdFov;
    float resultFov;

    view_camera gameCamera;

private:
    //�C�[�W���O���̎���p�ύX�X�V
    void UpdateFov(float elapsedTime);
public:
    /// <summary>
    /// ����p���Ȃ߂炩�ɕύX����
    /// </summary>
    /// <param name="fov">�ύX��̎���p</param>
    /// <param name="time">�ύX�ɗv���鎞��</param>
    void ChangeFov(float fov, float time);

    //����p�����Ƃɖ߂�
    void RestoreFov(float time);

    //�}�E�X���g�p���Ă��邩
    bool useMouse = false;
private:
};
