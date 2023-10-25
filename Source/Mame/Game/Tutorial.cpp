#include "Tutorial.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "TutorialManager.h"

// �ړ��`���[�g���A��
#pragma region TutorialMove

TutorialMove::TutorialMove()
{
    Graphics& graphics = Graphics::Instance();

    // �X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/Move.png"
        );

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/CheckMark.png",
            "./Resources/Shader/sprite_dissolve_ps.cso" // �f�B�]���u������
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/textBackGround.png"
        );
    }

}

void TutorialMove::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = {};
    XMFLOAT2 texSize = {};
    XMFLOAT2 scale   = {};
    XMFLOAT2 size    = {};
    XMFLOAT2 texPos  = {};
    XMFLOAT4 color   = {};
    float    angle   = {};

    // �e�L�X�g�w�i������
    // �e�̈ʒu�i���e�L�X�g�w�i�̈ʒu��e�̈ʒu�ɂ���j
    const XMFLOAT2 parentPos = { 735.0f, 215.0f };
    {
        texSize = { 520.0f, 300.0f };
        scale   = { 1.0f, 1.0f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        textBackGround_->Initialize(parentPos, size, texSize, texPos, color, angle);
    }

    // �e�L�X�g������
    {
        // �e�̈ʒu����ɂ��Ĉʒu��ݒ肷��
        pos.x   = parentPos.x + (57.0f);
        pos.y   = parentPos.y + (51.0f);
        texSize = { 400.0f, 200.0f };
        scale   = { 1.0f, 1.0f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        text_->Initialize(pos, size, texSize, texPos, color, angle);
    }

    // �`�F�b�N�}�[�N������
    {
        // �e�̈ʒu����ɂ��Ĉʒu��ݒ肷��
        pos.x   = parentPos.x + (-20.0f);
        pos.y   = parentPos.y + (-30.0f);
        texSize = { 512.0f, 350.0f };
        scale   = { 0.3f, 0.3f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        checkMark_->Initialize(pos, size, texSize, texPos, color, angle);
    }

}

void TutorialMove::Update(const float elapsedTime)
{
    TutorialManager& tutorialManager = TutorialManager::Instance();
    const GamePad& gamePad = Input::Instance().GetGamePad();

    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);

    // �ړ����͂�����Ă�����
    // �ړ��`���[�g���A���B���t���O�𗧂Ă�
    if (gamePad.GetAxisLX() != 0.0f ||
        gamePad.GetAxisLY() != 0.0f)
    {
        tutorialManager.CompleteTutorialAt(TUTORIAL_STEP::MOVE);
    }

}

void TutorialMove::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

void TutorialMove::DrawImGui()
{
#if USE_IMGUI

    using DirectX::XMFLOAT2;

    if (ImGui::BeginMenu("TutorialMove"))
    {
        Sprite::SpriteTransform* textBackGroundT = textBackGround_->GetSpriteTransform();
        Sprite::SpriteTransform* textT           = text_->GetSpriteTransform();
        Sprite::SpriteTransform* checkMarkT      = checkMark_->GetSpriteTransform();

        static constexpr float POS_LIMIT   = 1280.0f;
        static constexpr float SCALE_LIMIT = 20.0f;

        if (ImGui::BeginMenu("TextBackGround"))
        {
            XMFLOAT2 pos = textBackGroundT->GetPos();
            const XMFLOAT2 oldPos = pos;
            ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
            textBackGroundT->SetPos(pos);

            // ���̃X�v���C�g���ꏏ�Ɉړ�������
            const XMFLOAT2 addChlidPos = {
                (pos.x - oldPos.x), (pos.y - oldPos.y)
            };
            textT->AddPosX(addChlidPos.x);
            textT->AddPosY(addChlidPos.y);
            checkMarkT->AddPosX(addChlidPos.x);
            checkMarkT->AddPosY(addChlidPos.y);


            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = textBackGroundT->GetTexSize();
            float scaleX    = textBackGroundT->GetSize().x / textBackGroundT->GetTexSize().x;
            float scaleY    = textBackGroundT->GetSize().y / textBackGroundT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            scaleY += scaleX - oldScaleX;
            textBackGroundT->SetSizeX(texSize.x * scaleX);
            textBackGroundT->SetSizeY(texSize.y * scaleY);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Text"))
        {
            XMFLOAT2 pos = textT->GetPos();
            ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
            textT->SetPos(pos);

            // �e�̈ʒu����ǂꂭ�炢����Ă��邩�̃����p
            const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
            XMFLOAT2 textVariation = {
                pos.x - textBackGroundPos.x,
                pos.y - textBackGroundPos.y,
            };
            ImGui::InputFloat2("textVariation", &textVariation.x);

            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = textT->GetTexSize();
            float scaleX    = textT->GetSize().x / textT->GetTexSize().x;
            float scaleY    = textT->GetSize().y / textT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            scaleY += scaleX - oldScaleX;
            textT->SetSizeX(texSize.x * scaleX);
            textT->SetSizeY(texSize.y * scaleY);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("CheckMark"))
        {
            XMFLOAT2 pos = checkMarkT->GetPos();
            ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
            checkMarkT->SetPos(pos);

            // �e�̈ʒu����ǂꂭ�炢����Ă��邩�̃����p
            const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
            XMFLOAT2 checkMarkVariation = {
                pos.x - textBackGroundPos.x,
                pos.y - textBackGroundPos.y,
            };
            ImGui::InputFloat2("checkMarkVariation", &checkMarkVariation.x);

            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = checkMarkT->GetTexSize();
            float scaleX    = checkMarkT->GetSize().x / checkMarkT->GetTexSize().x;
            float scaleY    = checkMarkT->GetSize().y / checkMarkT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            scaleY += scaleX - oldScaleX;
            checkMarkT->SetSizeX(texSize.x * scaleX);
            checkMarkT->SetSizeY(texSize.y * scaleY);

            static float dissolveValue = 1.0f;
            ImGui::SliderFloat("dissolveValue", &dissolveValue, -1.0f, 1.0f);
            text_->GetSpriteDissolve()->SetDissolveValue(dissolveValue);


            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

#endif
}

#pragma endregion


// �J�����ړ��`���[�g���A��
#pragma region TutorialMoveCamera

TutorialMoveCamera::TutorialMoveCamera()
{
    Graphics& graphics = Graphics::Instance();

    // �X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/MoveCamera.png"
        );

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/CheckMark.png",
            "./Resources/Shader/sprite_dissolve_ps.cso" // �f�B�]���u������
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/textBackGround.png"
        );
    }

}

void TutorialMoveCamera::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = {};
    XMFLOAT2 texSize = {};
    XMFLOAT2 scale   = {};
    XMFLOAT2 size    = {};
    XMFLOAT2 texPos  = {};
    XMFLOAT4 color   = {};
    float    angle   = {};

    // �e�L�X�g�w�i������
    // �e�̈ʒu�i���e�L�X�g�w�i�̈ʒu��e�̈ʒu�ɂ���j
    const XMFLOAT2 parentPos = { 735.0f, 215.0f };
    {
        texSize = { 520.0f, 300.0f };
        scale   = { 1.0f, 1.0f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        textBackGround_->Initialize(parentPos, size, texSize, texPos, color, angle);
    }

    // �e�L�X�g������
    {
        // �e�̈ʒu����ɂ��Ĉʒu��ݒ肷��
        pos.x   = parentPos.x + (57.0f);
        pos.y   = parentPos.y + (51.0f);
        texSize = { 400.0f, 200.0f };
        scale   = { 1.0f, 1.0f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        text_->Initialize(pos, size, texSize, texPos, color, angle);
    }

    // �`�F�b�N�}�[�N������
    {
        // �e�̈ʒu����ɂ��Ĉʒu��ݒ肷��
        pos.x   = parentPos.x + (-20.0f);
        pos.y   = parentPos.y + (-30.0f);
        texSize = { 512.0f, 350.0f };
        scale   = { 0.3f, 0.3f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        checkMark_->Initialize(pos, size, texSize, texPos, color, angle);
    }

}

void TutorialMoveCamera::Update(const float elapsedTime)
{
    TutorialManager& tutorialManager = TutorialManager::Instance();
    const GamePad& gamePad = Input::Instance().GetGamePad();

    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);

    // �J�����ړ����͂�����Ă�����
    // �J�����ړ��`���[�g���A���B���t���O�𗧂Ă�
    if (gamePad.GetAxisRX() != 0.0f ||
        gamePad.GetAxisRY() != 0.0f)
    {
        tutorialManager.CompleteTutorialAt(TUTORIAL_STEP::MOVE_CAMERA);
    }

}

void TutorialMoveCamera::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

void TutorialMoveCamera::DrawImGui()
{
#if USE_IMGUI

    using DirectX::XMFLOAT2;

    if (ImGui::BeginMenu("TutorialMoveCamera"))
    {
        Sprite::SpriteTransform* textBackGroundT = textBackGround_->GetSpriteTransform();
        Sprite::SpriteTransform* textT           = text_->GetSpriteTransform();
        Sprite::SpriteTransform* checkMarkT      = checkMark_->GetSpriteTransform();

        static constexpr float POS_LIMIT   = 1280.0f;
        static constexpr float SCALE_LIMIT = 20.0f;

        if (ImGui::BeginMenu("TextBackGround"))
        {
            XMFLOAT2 pos = textBackGroundT->GetPos();
            const XMFLOAT2 oldPos = pos;
            ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
            textBackGroundT->SetPos(pos);

            // ���̃X�v���C�g���ꏏ�Ɉړ�������
            const XMFLOAT2 addChlidPos = {
                (pos.x - oldPos.x), (pos.y - oldPos.y)
            };
            textT->AddPosX(addChlidPos.x);
            textT->AddPosY(addChlidPos.y);
            checkMarkT->AddPosX(addChlidPos.x);
            checkMarkT->AddPosY(addChlidPos.y);


            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = textBackGroundT->GetTexSize();
            float scaleX    = textBackGroundT->GetSize().x / textBackGroundT->GetTexSize().x;
            float scaleY    = textBackGroundT->GetSize().y / textBackGroundT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            scaleY += scaleX - oldScaleX;
            textBackGroundT->SetSizeX(texSize.x * scaleX);
            textBackGroundT->SetSizeY(texSize.y * scaleY);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Text"))
        {
            XMFLOAT2 pos = textT->GetPos();
            ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
            textT->SetPos(pos);

            // �e�̈ʒu����ǂꂭ�炢����Ă��邩�̃����p
            const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
            XMFLOAT2 textVariation = {
                pos.x - textBackGroundPos.x,
                pos.y - textBackGroundPos.y,
            };
            ImGui::InputFloat2("textVariation", &textVariation.x);

            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = textT->GetTexSize();
            float scaleX    = textT->GetSize().x / textT->GetTexSize().x;
            float scaleY    = textT->GetSize().y / textT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            scaleY += scaleX - oldScaleX;
            textT->SetSizeX(texSize.x * scaleX);
            textT->SetSizeY(texSize.y * scaleY);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("CheckMark"))
        {
            XMFLOAT2 pos = checkMarkT->GetPos();
            ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
            checkMarkT->SetPos(pos);

            // �e�̈ʒu����ǂꂭ�炢����Ă��邩�̃����p
            const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
            XMFLOAT2 checkMarkVariation = {
                pos.x - textBackGroundPos.x,
                pos.y - textBackGroundPos.y,
            };
            ImGui::InputFloat2("checkMarkVariation", &checkMarkVariation.x);

            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = checkMarkT->GetTexSize();
            float scaleX    = checkMarkT->GetSize().x / checkMarkT->GetTexSize().x;
            float scaleY    = checkMarkT->GetSize().y / checkMarkT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            scaleY += scaleX - oldScaleX;
            checkMarkT->SetSizeX(texSize.x * scaleX);
            checkMarkT->SetSizeY(texSize.y * scaleY);

            static float dissolveValue = 1.0f;
            ImGui::SliderFloat("dissolveValue", &dissolveValue, -1.0f, 1.0f);
            text_->GetSpriteDissolve()->SetDissolveValue(dissolveValue);


            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

#endif
}

#pragma endregion


// ���b�N�I���`���[�g���A��
#pragma region TutorialLockOn

TutorialLockOn::TutorialLockOn()
{
    Graphics& graphics = Graphics::Instance();

    // �X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/MoveCamera.png"
        );

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/CheckMark.png",
            "./Resources/Shader/sprite_dissolve_ps.cso" // �f�B�]���u������
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/textBackGround.png"
        );
    }

}

void TutorialLockOn::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = {};
    XMFLOAT2 texSize = {};
    XMFLOAT2 scale   = {};
    XMFLOAT2 size    = {};
    XMFLOAT2 texPos  = {};
    XMFLOAT4 color   = {};
    float    angle   = {};

    // �e�L�X�g�w�i������
    // �e�̈ʒu�i���e�L�X�g�w�i�̈ʒu��e�̈ʒu�ɂ���j
    const XMFLOAT2 parentPos = { 735.0f, 215.0f };
    {
        texSize = { 520.0f, 300.0f };
        scale   = { 1.0f, 1.0f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        textBackGround_->Initialize(parentPos, size, texSize, texPos, color, angle);
    }

    // �e�L�X�g������
    {
        // �e�̈ʒu����ɂ��Ĉʒu��ݒ肷��
        pos.x   = parentPos.x + (57.0f);
        pos.y   = parentPos.y + (51.0f);
        texSize = { 400.0f, 200.0f };
        scale   = { 1.0f, 1.0f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        text_->Initialize(pos, size, texSize, texPos, color, angle);
    }

    // �`�F�b�N�}�[�N������
    {
        // �e�̈ʒu����ɂ��Ĉʒu��ݒ肷��
        pos.x   = parentPos.x + (-20.0f);
        pos.y   = parentPos.y + (-30.0f);
        texSize = { 512.0f, 350.0f };
        scale   = { 0.3f, 0.3f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        checkMark_->Initialize(pos, size, texSize, texPos, color, angle);
    }

}

void TutorialLockOn::Update(const float elapsedTime)
{
    TutorialManager& tutorialManager = TutorialManager::Instance();
    const GamePad& gamePad = Input::Instance().GetGamePad();

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

void TutorialLockOn::DrawImGui()
{
#if USE_IMGUI

    using DirectX::XMFLOAT2;

    if (ImGui::BeginMenu("TutorialLockOn"))
    {
        Sprite::SpriteTransform* textBackGroundT = textBackGround_->GetSpriteTransform();
        Sprite::SpriteTransform* textT           = text_->GetSpriteTransform();
        Sprite::SpriteTransform* checkMarkT      = checkMark_->GetSpriteTransform();

        static constexpr float POS_LIMIT   = 1280.0f;
        static constexpr float SCALE_LIMIT = 20.0f;

        if (ImGui::BeginMenu("TextBackGround"))
        {
            XMFLOAT2 pos = textBackGroundT->GetPos();
            const XMFLOAT2 oldPos = pos;
            ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
            textBackGroundT->SetPos(pos);

            // ���̃X�v���C�g���ꏏ�Ɉړ�������
            const XMFLOAT2 addChlidPos = {
                (pos.x - oldPos.x), (pos.y - oldPos.y)
            };
            textT->AddPosX(addChlidPos.x);
            textT->AddPosY(addChlidPos.y);
            checkMarkT->AddPosX(addChlidPos.x);
            checkMarkT->AddPosY(addChlidPos.y);


            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = textBackGroundT->GetTexSize();
            float scaleX    = textBackGroundT->GetSize().x / textBackGroundT->GetTexSize().x;
            float scaleY    = textBackGroundT->GetSize().y / textBackGroundT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            scaleY += scaleX - oldScaleX;
            textBackGroundT->SetSizeX(texSize.x * scaleX);
            textBackGroundT->SetSizeY(texSize.y * scaleY);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Text"))
        {
            XMFLOAT2 pos = textT->GetPos();
            ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
            textT->SetPos(pos);

            // �e�̈ʒu����ǂꂭ�炢����Ă��邩�̃����p
            const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
            XMFLOAT2 textVariation = {
                pos.x - textBackGroundPos.x,
                pos.y - textBackGroundPos.y,
            };
            ImGui::InputFloat2("textVariation", &textVariation.x);

            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = textT->GetTexSize();
            float scaleX    = textT->GetSize().x / textT->GetTexSize().x;
            float scaleY    = textT->GetSize().y / textT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            scaleY += scaleX - oldScaleX;
            textT->SetSizeX(texSize.x * scaleX);
            textT->SetSizeY(texSize.y * scaleY);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("CheckMark"))
        {
            XMFLOAT2 pos = checkMarkT->GetPos();
            ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
            checkMarkT->SetPos(pos);

            // �e�̈ʒu����ǂꂭ�炢����Ă��邩�̃����p
            const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
            XMFLOAT2 checkMarkVariation = {
                pos.x - textBackGroundPos.x,
                pos.y - textBackGroundPos.y,
            };
            ImGui::InputFloat2("checkMarkVariation", &checkMarkVariation.x);

            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = checkMarkT->GetTexSize();
            float scaleX    = checkMarkT->GetSize().x / checkMarkT->GetTexSize().x;
            float scaleY    = checkMarkT->GetSize().y / checkMarkT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            scaleY += scaleX - oldScaleX;
            checkMarkT->SetSizeX(texSize.x * scaleX);
            checkMarkT->SetSizeY(texSize.y * scaleY);

            static float dissolveValue = 1.0f;
            ImGui::SliderFloat("dissolveValue", &dissolveValue, -1.0f, 1.0f);
            text_->GetSpriteDissolve()->SetDissolveValue(dissolveValue);


            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

#endif
}

#pragma endregion


// �U���`���[�g���A��
#pragma region TutorialAttack

TutorialAttack::TutorialAttack()
{
    Graphics& graphics = Graphics::Instance();

    // �X�v���C�g����
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

void TutorialAttack::DrawImGui()
{
}

#pragma endregion


// ����`���[�g���A��
#pragma region TutorialAvoid

TutorialAvoid::TutorialAvoid()
{
    Graphics& graphics = Graphics::Instance();

    // �X�v���C�g����
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

void TutorialAvoid::DrawImGui()
{
}

#pragma endregion


// ���x���A�b�v�`���[�g���A��
#pragma region TutorialLevelUp

TutorialLevelUp::TutorialLevelUp()
{
    Graphics& graphics = Graphics::Instance();

    // �X�v���C�g����
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

void TutorialLevelUp::DrawImGui()
{
}

#pragma endregion

