#include "Tutorial.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Other/Easing.h"
#include "TutorialManager.h"
#include "EnemyManager.h"
#include "EnemyAI_Tutorial.h"

// �x�[�X�`���[�g���A��
#pragma region BaseTutorial

BaseTutorial::BaseTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // �X�v���C�g����
    {
        // ���e�L�X�g�X�v���C�g�͊e�p���N���X�Ő�������

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

void BaseTutorial::Initialize()
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
        color   = { 1.0f, 1.0f, 1.0f, 0.0f };
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
        color   = { 1.0f, 1.0f, 1.0f, 0.0f };
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
        color   = { 1.0f, 1.0f, 1.0f, 0.0f };
        angle   = 0.0f;
        checkMark_->Initialize(pos, size, texSize, texPos, color, angle);
    }

}

void BaseTutorial::Update(const float elapsedTime)
{
     TutorialManager& tutorialManager = TutorialManager::Instance();

    Sprite::SpriteTransform* textT      = text_->GetSpriteTransform();
    Sprite::SpriteTransform* checkMarkT = checkMark_->GetSpriteTransform();
    Sprite::SpriteTransform* textBG_T   = textBackGround_->GetSpriteTransform();

    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);

    float textColorAlpha        = 0.0f;
    float checkMarkColorAlpha   = 0.0f;
    float textBG_ColorAlpha     = 0.0f;

    switch (step_)
    {
    case 0:
        // �C�[�W���O�^�C�}�[�X�V
        easingTimer_ += elapsedTime;

        textColorAlpha      = textT->GetColorA();
        checkMarkColorAlpha = checkMarkT->GetColorA();
        textBG_ColorAlpha   = textBG_T->GetColorA();

        {
            const bool isEndEaseIn[3] = {
                BaseTutorial::colorAlphaEaseIn(&textColorAlpha),
                BaseTutorial::colorAlphaEaseIn(&checkMarkColorAlpha),
                BaseTutorial::colorAlphaEaseIn(&textBG_ColorAlpha),
            };

            // ���ׂẴX�v���C�g���s�����ɂȂ�����
            // ���̃X�e�b�v�Ɉڂ�
            if (false == isEndEaseIn[0] &&
                false == isEndEaseIn[1] &&
                false == isEndEaseIn[2])
            {
                ++step_;

                // �C�[�W���O�^�C�}�[���Z�b�g
                easingTimer_ = 0.0f;
            }
        }

        // �X�v���C�g�s�����x�X�V
        textT->SetColorA(textColorAlpha);
        checkMarkT->SetColorA(checkMarkColorAlpha);
        textBG_T->SetColorA(textBG_ColorAlpha);

        break;
    case 1:
        // ���̃X�e�b�v�Ɉڂ��Ȃ玟�̃X�e�b�v�Ɉړ�
        if (true == MoveNextStepJudgment())
        {
            ++step_;
            break;
        }

        break;
    case 2:
        // �C�[�W���^�C�}�[�X�V
        easingTimer_ += elapsedTime;

        textColorAlpha      = textT->GetColorA();
        checkMarkColorAlpha = checkMarkT->GetColorA();
        textBG_ColorAlpha   = textBG_T->GetColorA();

        {
            const bool isEndEaseOut[3] = {
                BaseTutorial::colorAlphaEaseOut(&textColorAlpha),
                BaseTutorial::colorAlphaEaseOut(&checkMarkColorAlpha),
                BaseTutorial::colorAlphaEaseOut(&textBG_ColorAlpha),
            };

            // ���ׂẴX�v���C�g�������ɂȂ�����
            // �`���[�g���A���B���t���O�𗧂Ă�
            if (false == isEndEaseOut[0] &&
                false == isEndEaseOut[1] &&
                false == isEndEaseOut[2])
            {
                tutorialManager.CompleteTutorial();

                // �C�[�W���O�^�C�}�[���Z�b�g
                easingTimer_ = 0.0f;
            }
        }

        // �X�v���C�g�s�����x�X�V
        textT->SetColorA(textColorAlpha);
        checkMarkT->SetColorA(checkMarkColorAlpha);
        textBG_T->SetColorA(textBG_ColorAlpha);

        break;
    }

}

void BaseTutorial::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

void BaseTutorial::DrawImGui()
{
#if USE_IMGUI

    using DirectX::XMFLOAT2;

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

#endif
}

const bool BaseTutorial::colorAlphaEaseIn(float* colorAlpha, const float endTime)
{
    if (easingTimer_ > endTime) { easingTimer_ = endTime; }

    // �s�����x����
    (*colorAlpha) = Easing::InSine(easingTimer_, endTime, 1.0f, 0.0f);

    // �s�����x��1.0f�ȏ�ɂȂ�����false��Ԃ�
    if ((*colorAlpha) >= 1.0f) { return false; }

    // EaseIn���s��
    return true;
}

const bool BaseTutorial::colorAlphaEaseOut(float* colorAlpha, const float endTime)
{
    if (easingTimer_ > endTime) { easingTimer_ = endTime; }

    // �s�����x����
    (*colorAlpha) = Easing::OutSine(easingTimer_, endTime, 0.0f, 1.0f);

    // �s�����x���[���ȉ��ɂȂ�����false��Ԃ�
    if ((*colorAlpha) <= 0.0f) { return false; }

    // EaseOut���s��
    return true;
}

#pragma endregion


// �ړ��`���[�g���A��
#pragma region TutorialMove

TutorialMove::TutorialMove()
{
    Graphics& graphics = Graphics::Instance();

    // �e�L�X�g�X�v���C�g����
    text_ = std::make_unique<Sprite>(
        graphics.GetDevice(),
        L"./Resources/Image/Tutorial/Texts/Move.png"
    );

}

void TutorialMove::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialMove"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialMove::MoveNextStepJudgment()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    // �ړ����͂�����Ă�����true��Ԃ�
    if (gamePad.GetAxisLX() != 0.0f ||
        gamePad.GetAxisLY() != 0.0f)
    {
        return true;
    }

    return false;
}

#pragma endregion


// �J�����ړ��`���[�g���A��
#pragma region TutorialMoveCamera

TutorialMoveCamera::TutorialMoveCamera()
{
    Graphics& graphics = Graphics::Instance();

    // �e�L�X�g�X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/MoveCamera.png"
        );
    }

}

void TutorialMoveCamera::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialMoveCamera"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialMoveCamera::MoveNextStepJudgment()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    // �J�����ړ����͂�����Ă�����true��Ԃ�
    if (gamePad.GetAxisRY() != 0.0f ||
        gamePad.GetAxisRY() != 0.0f)
    {
        return true;
    }

    return false;
}

#pragma endregion


// ���b�N�I���`���[�g���A��
#pragma region TutorialLockOn

TutorialLockOn::TutorialLockOn()
{
    Graphics& graphics = Graphics::Instance();

    // �e�L�X�g�X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/LockOn.png"
        );
    }

}

void TutorialLockOn::Initialize()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // �x�[�X�`���[�g���A���̏������֐��Ăяo��
    BaseTutorial::Initialize();

    // �`���[�g���A���G�l�~�[�����E�������E�o�^
    {
        EnemyAI_Tutorial* tutorialEnemy = new EnemyAI_Tutorial();
        tutorialEnemy->Initialize();
        enemyManager.Register(tutorialEnemy);
    }

}

void TutorialLockOn::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialLockOn"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialLockOn::MoveNextStepJudgment()
{
    const Camera& camera = Camera::Instance();

    //// ���b�N�I�����L���ɂȂ��Ă����true��Ԃ�
    //if (true == camera.GetActiveLockOn())
    //{
    //    return true;
    //}

    return false;
}

#pragma endregion


// �U���`���[�g���A��
#pragma region TutorialAttack

TutorialAttack::TutorialAttack()
{
    Graphics& graphics = Graphics::Instance();

    // �e�L�X�g�X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/Attack.png"
        );
    }
}

void TutorialAttack::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialAttack"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialAttack::MoveNextStepJudgment()
{
    return false;
}

#pragma endregion


// ����`���[�g���A��
#pragma region TutorialAvoid

TutorialAvoid::TutorialAvoid()
{
    Graphics& graphics = Graphics::Instance();

    // �e�L�X�g�X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/Avoid.png"
        );
    }

}

void TutorialAvoid::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialAvoid"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialAvoid::MoveNextStepJudgment()
{
    return false;
}

#pragma endregion


// ���x���A�b�v�`���[�g���A��
#pragma region TutorialLevelUp

TutorialLevelUp::TutorialLevelUp()
{
    Graphics& graphics = Graphics::Instance();

    // �e�L�X�g�X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/LevelUp.png"
        );
    }

}

void TutorialLevelUp::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialLevelUp"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialLevelUp::MoveNextStepJudgment()
{
    return false;
}

#pragma endregion
