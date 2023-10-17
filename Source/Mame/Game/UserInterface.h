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
    void BloomRender();//ブルーム効果付きの描画
    void DrawDebug();

private:
    std::unique_ptr<Sprite> lockOnSprite;

    std::unique_ptr<Sprite> hpSprite;           // hp
    std::unique_ptr<Sprite> keikenchiSprite;    // 経験値ゲージ
    std::unique_ptr<Sprite> maruSprite;         // 丸
    std::unique_ptr<Sprite> backGageSprite[2];        // ゲージ背景
};

