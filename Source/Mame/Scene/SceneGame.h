#pragma once
#include "BaseScene.h"

#include <memory>

#include "../../Taki174/TextRenderer.h"

#include "../Resource/sprite.h"

#include "../Graphics/Effect.h"
#include "../Graphics/Bloom.h"
#include "../Graphics/ShadowMap.h"
#include "../Graphics/Particle.h"

#include "../Game/Stage.h"

#include "../Game/EnemySlime.h"
#include "../Game/EnemyAura.h"
#include "../Game/EnemyGolem.h"



class SceneGame : public Mame::Scene::BaseScene
{
public: // ���N���X�ɂ���֐�
    SceneGame() {}
    ~SceneGame() override {}

    void CreateResource() override;                 // ���\�[�X����
    void Initialize()     override;                 // ������
    void Finalize()       override;                 // �I����
    void Begin()          override;                 // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()            override;                 // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��
    void DrawDebug()      override;

public: // �֐�
    void DebugCreateEnemyFromGateway(); // �Q�[�g����G�𐶐�����֐�(�f�o�b�O�p)

    void UpdateWhiteSprite(const float& elapsedTime);

public: // �ݒ�E�擾

private: // �ϐ����X
    // stage
    std::unique_ptr<Stage> stageBase;
    std::unique_ptr<Stage> stageWall;

    std::unique_ptr<Sprite> whiteSprite;

    

    // effect
    Effect* effect[4];
    //std::unique_ptr<Effect> effect[4];

    bool isParticleInitialize = false;
    bool isDebugCamera = false;
    bool isWhiteSpriteRender = false;
    bool isBlackSpriteRender = false;
    float whiteSpriteTimer = 0.0f;
    float blackSpriteTimer = 0.0f;

    std::unique_ptr<EnemyGolem> enemyGolem;


private: // GPU�p�萔�E�ϐ�
    std::unique_ptr<FrameBuffer> framebuffers[3];
    std::unique_ptr<FullscreenQuad> bitBlockTransfer;

    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;

    // SHADOW
    struct Shadow
    {
        const uint32_t shadowMapWidth = 2048;
        const uint32_t shadowMapHeight = 2048;
        //const uint32_t shadowMapWidth = 4096;
        //const uint32_t shadowMapHeight = 4096;
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

    // bokeh
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bokehPS;

    // PARTICLE
    std::unique_ptr<Particles> particles;
    bool integrateParticles = true;

    // EMISSIVE
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture[2];
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleTexture;

    // PostEffect
    Microsoft::WRL::ComPtr<ID3D11PixelShader> postEffectPS;

    // PS Shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> sagePS; // �����̃V�F�[�_�[

public:
    static DirectX::XMFLOAT3 stageCenter;               // �X�e�[�W�̒��S
    static constexpr float   stageRadius    = 16.7f;    // �X�e�[�W�p�̔��a
    static constexpr int     GATEWAY_COUNT_ = 5;        // �Q�[�g�̐�

public: // debug�p
#ifdef _DEBUG
    static bool isDebugRender;
#endif // _DEBUG

    static bool isDispCollision_; // �����蔻��`��

    std::unique_ptr<TextRenderer> text_;

};

