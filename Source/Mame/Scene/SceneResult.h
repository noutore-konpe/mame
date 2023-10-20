#pragma once
#include "BaseScene.h"

#include <memory>

#include "../FrameBuffer.h"
#include "../FullscreenQuad.h"

#include "../Resource/sprite.h"

#include "../Game/EnemyGolemResult.h"
#include "../Game/PlayerResult.h"

class SceneResult : public Mame::Scene::BaseScene
{
public:// �萔
    enum class Ability
    {
        Absorption,     // �z��
        AttackSpeed,    // �U���X�s�[�h
        Defense,        // �h���
        Power,          // �U����
        Book,           // �{
        Homing,         // �z�[�~���O
        Poison,         // ��
        BlackHole,      // �u���b�N�z�[��
        HpUp,           // Hp�㏸
        MoveSpeed,      // �ړ����x
        Max,            // �ő�
    };

public:
    SceneResult() {}
    ~SceneResult()       override {}

    void CreateResource()override;                  // ���\�[�X����
    void Initialize()   override;                   // ������
    void Finalize()     override;                   // �I����
    void Begin()        override;                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()          override;                   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug()    override;

private:
    void UpdateEnemyKillNumAndx(const float& elapsedTime);

    void RenderEnemyKillX();
    void RenderEnemyKillNum();
    /// <summary>
    /// �l���܂őΉ����鐔���̕`��
    /// </summary>
    /// <param name="who">�N�̃J�E���g��</param>
    /// <param name="firstPosX">1����</param>
    /// <param name="secondPosX">2����</param>
    /// <param name="thirdPosX">3����</param>
    /// <param name="fourthPosX">4����</param>
    void RenderNum(const int who, const float firstPosX, const float secondPosX, const float thirdPosX, const float fourthPosX);

    void RenderEnemyModel();

private:
    std::unique_ptr<EnemyGolemResult> enemyGolem = nullptr;
    std::unique_ptr<PlayerResult> player = nullptr;

    std::unique_ptr<Sprite> backSprite = nullptr;
    
    std::unique_ptr<Sprite> lifeTimeSprite = nullptr;
    std::unique_ptr<Sprite> waveSprite = nullptr;
    std::unique_ptr<Sprite> lvSprite = nullptr;

    std::unique_ptr<Sprite> xSprite = nullptr;
    std::unique_ptr<Sprite> numSprite = nullptr;

    std::unique_ptr<Sprite> iconSprite[static_cast<UINT>(Ability::Max)];

    struct EnemyResult
    {
        DirectX::XMFLOAT3 position = {};
        DirectX::XMFLOAT3 scale = {};
        DirectX::XMFLOAT3 rotate = {};
        DirectX::XMFLOAT4 color = {};
    }enemyResult[3];


    struct SlideStruct
    {
        float addPosX = 0.0f;
        float easingTimer = 0.0f;
    };

    SlideStruct KillX;
    SlideStruct killNum;
    bool isSlide = false;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> finalPassPS;
    Microsoft::WRL::ComPtr<ID3D11Buffer> resultConstantBuffer;
    struct ResultConstants
    {
        DirectX::XMFLOAT4 color{ 0.3f, 0.3f, 0.3f ,1.0f };
    }resultConstants;
};

