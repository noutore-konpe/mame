#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"

#include "../Graphics/Bloom.h"
#include "../Graphics/ShadowMap.h"
#include "../Graphics/Particle.h"

#include "../Game/EnemySlime.h"
#include "../Game/EnemyAura.h"
#include "../Game/EnemyGolem.h"

#include "../Game/Stage.h"

class SceneTutorial : public Mame::Scene::BaseScene
{
public:// �萔


public:// ���N���X�ɂ���֐�
    SceneTutorial() {}
    ~SceneTutorial() override {}

    void CreateResource() override;                 // ���\�[�X����
    void Initialize()     override;                 // ������
    void Finalize()       override;                 // �I����
    void Begin()          override;                 // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()            override;                 // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��
    void DrawDebug()      override;

public:// �֐�

private:// �ϐ�
    std::unique_ptr<Stage> stageBase;
    std::unique_ptr<Stage> stageWall;

private:// �O���t�B�b�N�֌W
    std::unique_ptr<FrameBuffer> framebuffers[2];
    std::unique_ptr<FullscreenQuad> bitBlockTransfer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
    // SHADOW
    struct Shadow
    {
        const uint32_t shadowMapWidth = 2048;
        const uint32_t shadowMapHeight = 2048;
        std::unique_ptr<ShadowMap> shadowMap;
        DirectX::XMFLOAT4 lightViewFocus{ 0,0,0,1 };
        float lightViewDistance = 10.0f;
        float lightViewSize = 12.0f;
        float lightViewNearZ = 2.0f;
        float lightViewFarZ = 18.0f;
    } shadow;
    Microsoft::WRL::ComPtr<ID3D11Buffer> shadowConstantBuffer;

    // BLOOM
    std::unique_ptr<Bloom> bloomer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS;

    // FOG
    Microsoft::WRL::ComPtr<ID3D11PixelShader> fogPS;

public:
    static DirectX::XMFLOAT3 stageCenter;       // �X�e�[�W�̒��S
    static constexpr float stageRadius = 16.7f; // �X�e�[�W�p�̔��a
};

