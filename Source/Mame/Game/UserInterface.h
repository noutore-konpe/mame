#pragma once
#include "../Resource/sprite.h"
#include <memory>

class UserInterface
{
public:
    enum class STATE
    {
        Idle,       // �ҋ@
        CreateWhite,// ���w�i
        FadeIn,     // �t�F�[�h�C��
        Display,    // �\��
        FadeOut,    // �t�F�[�h�A�E�g
        DeleteWhite,// ���w�i
    };

private:
    UserInterface() {}
    ~UserInterface() {}

public:
    static UserInterface& Instance()
    {
        static UserInterface instance;
        return instance;
    }

    void Initialize();
    void Update(float elapsedTime);
    void Render();
    void BloomRender();//�u���[�����ʕt���̕`��
    void DrawDebug();

public:// �ݒ�E�擾
    bool GetLockOnInitialize() { return isLockOnInitialize; }
    void SetLockOnInitialize() { isLockOnInitialize = true; }

    void SetWaveSlideSprite() { isWaveSlideSprite = true; }

private:
    void UpdateLockOnSprite(const float& elapsedTime);
    void UpdateWaveSprite(const float& elapsedTime);
    void UpdateHpExpGauge();

    void RenderWaveSlide();
    void RenderLv();
    void RenderWave();

private:
    std::unique_ptr<Sprite> lockOnSprite;
    std::unique_ptr<Sprite> lockOnMaruSprite;

    std::unique_ptr<Sprite> hpSprite;           // hp
    std::unique_ptr<Sprite> keikenchiSprite;    // �o���l�Q�[�W
    std::unique_ptr<Sprite> maruSprite;         // ��
    std::unique_ptr<Sprite> backGageSprite[2];  // �Q�[�W�w�i

    std::unique_ptr<Sprite> lvSprite;   // Lv
    std::unique_ptr<Sprite> waveSprite; // Wave
    std::unique_ptr<Sprite> numSprite;  // num
    std::unique_ptr<Sprite> chonchonSprite; // :

    std::unique_ptr<Sprite> whiteSprite;        // ���摜
    std::unique_ptr<Sprite> waveSlideSprite;    // ����Ă����摜(Wave)
    std::unique_ptr<Sprite> numSlideSprite;     // ����Ă����摜(����)
    DirectX::XMFLOAT2 numPosition[4] = {};    // 

    bool isLockOnInitialize = false;
    float lockOnTimer = 0.0f;

    bool isWaveSlideSprite = false; // ��ʐ^�񒆂ɏo������
    int slideState = 0;             // �X���C�h�p
    float easingTimer = 0.0f;       // easing�p

    float hpSpriteSizeX;//�T�C�Y�̏����l��ێ����Ƃ���������
    float expSpriteSizeX;
};

