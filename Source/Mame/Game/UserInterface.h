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
    void BloomRender();//ƒuƒ‹[ƒ€Œø‰Ê•t‚«‚Ì•`‰æ
    void DrawDebug();

private:
    std::unique_ptr<Sprite> lockOnSprite;
};

