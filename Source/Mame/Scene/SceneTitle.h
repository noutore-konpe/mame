#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"
#include "../Graphics/ShadowMap.h"
#include "../Graphics/Bloom.h"
#include "../Graphics/Particle.h"

#include "../Game/Stage.h"

class SceneTitle : public Mame::Scene::BaseScene
{
public:
    SceneTitle() {}
    ~SceneTitle()       override {}

    void CreateResource()override;                  // ���\�[�X����
    void Initialize()   override;                   // ������
    void Finalize()     override;                   // �I����
    void Begin()        override;                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()          override;                   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug()    override;

private:
    std::unique_ptr<Stage> stageBase;
    std::unique_ptr<Stage> stageWall;

    std::unique_ptr<Sprite> backSprite;
    std::unique_ptr<Sprite> emmaSprite;
    std::unique_ptr<Sprite> pressSprite;

    // �ϐ�
    bool isAlphaDown = false;
    float pressTimer = 0.0f;

private:
    std::unique_ptr<FrameBuffer> framebuffers[3];
    std::unique_ptr<FullscreenQuad> bitBlockTransfer;

    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> finalPassPS;


    // BLOOM
    std::unique_ptr<Bloom> bloomer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS;

    // FOG
    Microsoft::WRL::ComPtr<ID3D11PixelShader> fogPS;

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
};

