#include "Tutorial.h"
#include "../Graphics/Graphics.h"

// 移動チュートリアル
#pragma region TutorialMove

TutorialMove::TutorialMove()
{
    Graphics& graphics = Graphics::Instance();

    // スプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );
    }

}

void TutorialMove::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = { 0.0f, 0.0f };
    XMFLOAT2 size    = { 1.0f, 1.0f };
    XMFLOAT2 texSize = { 10.0f, 10.0f };
    XMFLOAT2 texPos  = { 0.0f, 0.0f };
    XMFLOAT4 color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    float    angle   = 0.0f;
    text_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    checkMark_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    textBackGround_->Initialize(pos, size, texSize, texPos, color, angle);

}

void TutorialMove::Update(const float elapsedTime)
{
    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);
}

void TutorialMove::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

#pragma endregion


// カメラ移動チュートリアル
#pragma region TutorialMoveCamera

TutorialMoveCamera::TutorialMoveCamera()
{
    Graphics& graphics = Graphics::Instance();

    // スプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );
    }

}

void TutorialMoveCamera::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = { 0.0f, 0.0f };
    XMFLOAT2 size    = { 1.0f, 1.0f };
    XMFLOAT2 texSize = { 10.0f, 10.0f };
    XMFLOAT2 texPos  = { 0.0f, 0.0f };
    XMFLOAT4 color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    float    angle   = 0.0f;
    text_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    checkMark_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    textBackGround_->Initialize(pos, size, texSize, texPos, color, angle);

}

void TutorialMoveCamera::Update(const float elapsedTime)
{
    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);
}

void TutorialMoveCamera::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

#pragma endregion


// ロックオンチュートリアル
#pragma region TutorialLockOn

TutorialLockOn::TutorialLockOn()
{
    Graphics& graphics = Graphics::Instance();

    // スプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );
    }

}

void TutorialLockOn::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = { 0.0f, 0.0f };
    XMFLOAT2 size    = { 1.0f, 1.0f };
    XMFLOAT2 texSize = { 10.0f, 10.0f };
    XMFLOAT2 texPos  = { 0.0f, 0.0f };
    XMFLOAT4 color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    float    angle   = 0.0f;
    text_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    checkMark_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    textBackGround_->Initialize(pos, size, texSize, texPos, color, angle);

}

void TutorialLockOn::Update(const float elapsedTime)
{
    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);
}

void TutorialLockOn::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

#pragma endregion


// 攻撃チュートリアル
#pragma region TutorialAttack

TutorialAttack::TutorialAttack()
{
    Graphics& graphics = Graphics::Instance();

    // スプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );
    }

}

void TutorialAttack::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = { 0.0f, 0.0f };
    XMFLOAT2 size    = { 1.0f, 1.0f };
    XMFLOAT2 texSize = { 10.0f, 10.0f };
    XMFLOAT2 texPos  = { 0.0f, 0.0f };
    XMFLOAT4 color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    float    angle   = 0.0f;
    text_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    checkMark_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    textBackGround_->Initialize(pos, size, texSize, texPos, color, angle);

}

void TutorialAttack::Update(const float elapsedTime)
{
    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);
}

void TutorialAttack::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

#pragma endregion


// 回避チュートリアル
#pragma region TutorialAvoid

TutorialAvoid::TutorialAvoid()
{
    Graphics& graphics = Graphics::Instance();

    // スプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );
    }

}

void TutorialAvoid::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = { 0.0f, 0.0f };
    XMFLOAT2 size    = { 1.0f, 1.0f };
    XMFLOAT2 texSize = { 10.0f, 10.0f };
    XMFLOAT2 texPos  = { 0.0f, 0.0f };
    XMFLOAT4 color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    float    angle   = 0.0f;
    text_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    checkMark_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    textBackGround_->Initialize(pos, size, texSize, texPos, color, angle);

}

void TutorialAvoid::Update(const float elapsedTime)
{
    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);
}

void TutorialAvoid::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

#pragma endregion


// レベルアップチュートリアル
#pragma region TutorialLevelUp

TutorialLevelUp::TutorialLevelUp()
{
    Graphics& graphics = Graphics::Instance();

    // スプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/UI/TutorialText/Text1.png"
        );
    }

}

void TutorialLevelUp::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = { 0.0f, 0.0f };
    XMFLOAT2 size    = { 1.0f, 1.0f };
    XMFLOAT2 texSize = { 10.0f, 10.0f };
    XMFLOAT2 texPos  = { 0.0f, 0.0f };
    XMFLOAT4 color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    float    angle   = 0.0f;
    text_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    checkMark_->Initialize(pos, size, texSize, texPos, color, angle);

    pos     = { 0.0f, 0.0f };
    size    = { 1.0f, 1.0f };
    texSize = { 10.0f, 10.0f };
    texPos  = { 0.0f, 0.0f };
    color   = { 1.0f, 1.0f, 1.0f, 1.0f };
    angle   = 0.0f;
    textBackGround_->Initialize(pos, size, texSize, texPos, color, angle);

}

void TutorialLevelUp::Update(const float elapsedTime)
{
    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);
}

void TutorialLevelUp::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

#pragma endregion

