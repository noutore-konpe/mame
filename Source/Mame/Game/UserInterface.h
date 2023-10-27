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

    static const int iconMax = 15;

    DirectX::XMFLOAT2 skillNumPos[iconMax] =
    {
        { 1240, 60 },
        { 1180, 60 },
        { 1120, 60 },
        { 1060, 60 },
        { 1000, 60 }, // 5
        {  940, 60 },
        {  880, 60 },
        {  820, 60 },
        {  760, 60 },
        {  700, 60 }, // 10
        {  640, 60 },
        {  580, 60 },
        {  520, 60 },
        {  460, 60 },
        {  400, 60 }, // 15
    };

    DirectX::XMFLOAT2 skillXPos[iconMax] =
    {
        { 1228, 72 },
        { 1168, 72 },
        { 1108, 72 },
        { 1048, 72 },
        {  988, 72 }, // 5
        {  928, 72 },
        {  868, 72 },
        {  808, 72 },
        {  748, 72 },
        {  688, 72 }, // 10
        {  628, 72 },
        {  568, 72 },
        {  508, 72 },
        {  448, 72 },
        {  388, 72 }, // 15
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

    void RenderNum(const int who, const float firstPosX, const float secondPosX, const float thirdPosX, const float fourthPosX);
    void RenderSkillNum();
    void RenderSkillX();


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

    std::unique_ptr<Sprite> skillNumSprite;
    std::unique_ptr<Sprite> skillxSprite;

    bool isLockOnInitialize = false;
    float lockOnTimer = 0.0f;

    bool isWaveSlideSprite = false; // ��ʐ^�񒆂ɏo������
    int slideState = 0;             // �X���C�h�p
    float easingTimer = 0.0f;       // easing�p

    float hpSpriteSizeX;//�T�C�Y�̏����l��ێ����Ƃ���������
    float expSpriteSizeX;
};

