#pragma once
#include "../Resource/sprite.h"
#include <memory>

class UserInterface
{
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

    void RenderLv();
    void RenderWave();

private:
    std::unique_ptr<Sprite> lockOnSprite;

    std::unique_ptr<Sprite> hpSprite;           // hp
    std::unique_ptr<Sprite> keikenchiSprite;    // �o���l�Q�[�W
    std::unique_ptr<Sprite> maruSprite;         // ��
    std::unique_ptr<Sprite> backGageSprite[2];  // �Q�[�W�w�i

    std::unique_ptr<Sprite> lvSprite;   // Lv
    std::unique_ptr<Sprite> waveSprite; // Wave
    std::unique_ptr<Sprite> numSprite;  // num
    std::unique_ptr<Sprite> chonchonSprite; // :

#ifdef _DEBUG
    int lv = 0;
    int wave = 0;
#endif
};

