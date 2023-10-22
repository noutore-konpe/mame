#pragma once

#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Game/LoadingPlayer.h"
#include "../Resource/sprite.h"

class SceneLoading : public Mame::Scene::BaseScene
{
public:
    SceneLoading(BaseScene* nextScene);
    ~SceneLoading() override {}

    void CreateResource()override;                  // ���\�[�X����
    void Initialize()   override;                   // ������
    void Finalize()     override;                   // �I����
    void Begin()        override;                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()          override;                   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug()    override;

private:
    // ���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene);

private:
    BaseScene*      nextScene   = nullptr;
    std::thread*    thread      = nullptr;

private:
    std::unique_ptr<LoadingPlayer> loadingPlayer = nullptr;
    std::unique_ptr<Sprite> titleLogo = nullptr;

    Microsoft::WRL::ComPtr<ID3D11Buffer> playerConstant;
    struct PlayerConstants
    {
        DirectX::XMFLOAT4 color;
    }playerConstants;

    bool isChangeScene = false;
    float easingTimer = 0.0f;

    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
};

