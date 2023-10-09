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

    void Update();

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

private: // Debug�p
    float moveSpeed = 0.01f;
    float rotationSpeed = 60.0f;

public:
    Transform* GetTransform() { return &transform; }        // transform�擾
    DirectX::XMMATRIX GetViewMatrix() { return V; }         // view�s��擾
    DirectX::XMMATRIX GetProjectionMatrix() { return P; }   // projection�s��擾

private:
    Transform transform{};

    DirectX::XMMATRIX V{};  // view�s��
    DirectX::XMMATRIX P{};  // projection�s��

    //�v���C���[����J�����܂ł̋���
    float focalLength = 7.0f;
    float offsetY = 2.7f;
    float focusOffsetY = 1.0f;

    Transform* focusTarget;//�����_�ɂȂ�I�u�W�F�N�g

    DirectX::XMFLOAT3 lockOnTargetPos;//���b�N�I���Ώۂ̍��W
    bool activeLockOn = false;//���b�N�I���N��

    bool enableDebugCamera = false;

    //��ʐU��
    DirectX::XMFLOAT3 screenVibrationOffset{};//�U���\���p�̍��W
    float vibrationVolume;//�U����
    float vibrationTime;//�U������
    float vibrationTimer;//�U�����Ԃ𑪂邽�߂̃^�C�}�[
};
