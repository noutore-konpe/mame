#include "Tutorial.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Other/Easing.h"

#include "TutorialManager.h"
#include "EnemyManager.h"
#include "EnemyAI_Tutorial.h"
#include "PlayerManager.h"
#include "Player.h"
#include "WaveManager.h"

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
    const XMFLOAT2 parentPos = { 840.0f, 215.0f };
    {
        texSize = textBackGround_->GetSpriteTransform()->GetTexSize();
        scale   = { 1.0f, 1.0f };
        //size    = { texSize.x * scale.x, texSize.y * scale.y };
        size    = { 400.0f * scale.x, 200.0f * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 0.0f };
        angle   = 0.0f;
        textBackGround_->Initialize(parentPos, size, texSize, texPos, color, angle);
    }


    // �e�L�X�g������
    {
        // �e�̈ʒu����ɂ��Ĉʒu��ݒ肷��
        pos.x   = parentPos.x + (-4.0f);
        pos.y   = parentPos.y + (+10.0f);
        texSize = text_->GetSpriteTransform()->GetTexSize();
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
        texSize = checkMark_->GetSpriteTransform()->GetTexSize();
        scale   = { 0.3f, 0.3f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        checkMark_->Initialize(pos, size, texSize, texPos, color, angle);

        // �f�B�]���u�����ݒ�
        checkMark_->GetSpriteDissolve()->SetDissolveValue(1.0f);

    }

    createTimer_ = 0.0f;
    easingTimer_ = 0.0f;
    step_        = 0;

}

void BaseTutorial::Update(const float elapsedTime)
{
     TutorialManager& tutorialManager = TutorialManager::Instance();

    Sprite::SpriteTransform* textT      = text_->GetSpriteTransform();
    Sprite::SpriteTransform* checkMarkT = checkMark_->GetSpriteTransform();
    Sprite::SpriteTransform* textBG_T   = textBackGround_->GetSpriteTransform();

    // �f�B�]���u�X�V�̌Ăяo���͂���������ōX�V���s��
    //text_->Update(elapsedTime);
    //checkMark_->Update(elapsedTime);
    //textBackGround_->Update(elapsedTime);

    float textColorAlpha        = 0.0f;
    float textBG_ColorAlpha     = 0.0f;

    switch (step_)
    {
    case 0:
        // �C�[�W���O�^�C�}�[�X�V
        easingTimer_ += elapsedTime;

        textColorAlpha      = textT->GetColorA();
        textBG_ColorAlpha   = textBG_T->GetColorA();

        {
            const bool isEndEaseIn[2] = {
                BaseTutorial::colorAlphaEaseIn(&textColorAlpha),
                BaseTutorial::colorAlphaEaseIn(&textBG_ColorAlpha),
            };

            // ���ׂẴX�v���C�g���s�����ɂȂ�����
            // ���̃X�e�b�v�Ɉڂ�
            if (false == isEndEaseIn[0] &&
                false == isEndEaseIn[1]   )
            {
                ++step_;

                // �C�[�W���O�^�C�}�[���Z�b�g
                easingTimer_ = 0.0f;
            }
        }

        // �X�v���C�g�s�����x�X�V
        textT->SetColorA(textColorAlpha);
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
        // �`�F�b�N�}�[�N�̃f�B�]���u�C�����I������玟�̃X�e�b�v�Ɉړ�
        {
            Sprite::SpriteDissolve* checkMarkDissolve = checkMark_->GetSpriteDissolve();
            checkMarkDissolve->AddDissolveValue(-elapsedTime);
            if (checkMarkDissolve->GetDissolveValue() <= (-0.1f))
            {
                checkMarkDissolve->SetDissolveValue(-0.1f);
                ++step_;
                break;
            }
        }

        break;
    case 3: // �ҋ@
        // �^�C�}�[�X�V(�C�[�W���O�^�C�}�[��ҋ@�^�C�}�[����Ɏg�p)
        easingTimer_ += elapsedTime;

        // ���΂炭�Ԃ��J�����玟�̃X�e�b�v�Ɉړ�
        if (easingTimer_ >= 2.0)
        {
            // �C�[�W���O�^�C�}�[���Z�b�g
            easingTimer_ = 0.0f;
            ++step_;
            break;
        }

        break;
    case 4:
        // �C�[�W���O�^�C�}�[�X�V
        easingTimer_ += elapsedTime;

        // �`�F�b�N�}�[�N�̃f�B�]���u�A�E�g�X�V
        {
            Sprite::SpriteDissolve* checkMarkDissolve = checkMark_->GetSpriteDissolve();
            if (checkMarkDissolve->GetDissolveValue() < 1.0f)
            {
                checkMarkDissolve->AddDissolveValue(elapsedTime);
            }
        }

        textColorAlpha      = textT->GetColorA();
        textBG_ColorAlpha   = textBG_T->GetColorA();

        {
            const bool isEndEaseOut[2] = {
                BaseTutorial::colorAlphaEaseOut(&textColorAlpha),
                BaseTutorial::colorAlphaEaseOut(&textBG_ColorAlpha),
            };

            // ���ׂẴX�v���C�g�������ɂȂ�����
            // �`���[�g���A���B���t���O�𗧂Ă�
            if (false == isEndEaseOut[0] &&
                false == isEndEaseOut[1]   )
            {
                tutorialManager.CompleteTutorial();

                // �C�[�W���O�^�C�}�[���Z�b�g
                easingTimer_ = 0.0f;
            }
        }

        // �X�v���C�g�s�����x�X�V
        textT->SetColorA(textColorAlpha);
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
        textBackGround_->DrawDebug();

        // �ʒu
        {
            // ���̃X�v���C�g�ʒu��������
            XMFLOAT2 variationPos = textBackGroundT->GetPos();
            const XMFLOAT2 oldPos = variationPos;
            ImGui::SliderFloat2("variationPos", &variationPos.x, 0.0f, POS_LIMIT);
            textBackGroundT->SetPos(variationPos);

            // ���̃X�v���C�g���ꏏ�Ɉړ�������
            const XMFLOAT2 addChlidPos = {
                (variationPos.x - oldPos.x), (variationPos.y - oldPos.y)
            };
            textT->AddPosX(addChlidPos.x);
            textT->AddPosY(addChlidPos.y);
            checkMarkT->AddPosX(addChlidPos.x);
            checkMarkT->AddPosY(addChlidPos.y);
        }

        // �X�P�[��
        {
            // scale��size�ɔ��f������
            const XMFLOAT2& texSize = textBackGroundT->GetTexSize();
            float scaleX    = textBackGroundT->GetSize().x / textBackGroundT->GetTexSize().x;
            float scaleY    = textBackGroundT->GetSize().y / textBackGroundT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleX�̕ϓ��ʂ�scaleY�ɂ����f������
            const float variationScaleX = scaleX - oldScaleX;
            scaleY += variationScaleX;
            textBackGroundT->SetSizeX(texSize.x * scaleX);
            textBackGroundT->SetSizeY(texSize.y * scaleY);

            // ��_��������̂Ŏ��s
            //// ���̃X�v���C�g���ꏏ�ɃX�P�[�����O������
            //{
            //    const float textScaleX = textT->GetSize().x / textT->GetTexSize().x;
            //    const float textScaleY = textT->GetSize().y / textT->GetTexSize().y;
            //    textT->SetSizeX(textT->GetTexSize().x * (textScaleX + variationScaleX));
            //    textT->SetSizeY(textT->GetTexSize().y * (textScaleY + variationScaleX));
            //
            //    const float checkMarkScaleX = checkMarkT->GetSize().x / checkMarkT->GetTexSize().x;
            //    const float checkMarkScaleY = checkMarkT->GetSize().y / checkMarkT->GetTexSize().y;
            //    checkMarkT->SetSizeX(checkMarkT->GetTexSize().x * (checkMarkScaleX + variationScaleX));
            //    checkMarkT->SetSizeY(checkMarkT->GetTexSize().y * (checkMarkScaleY + variationScaleX));
            //}
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Text"))
    {
        textBackGround_->DrawDebug();

        // �e�̈ʒu����ǂꂭ�炢����Ă��邩�̃����p
        const XMFLOAT2& pos = textT->GetPos();
        const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
        XMFLOAT2 variationPos = {
            pos.x - textBackGroundPos.x,
            pos.y - textBackGroundPos.y,
        };
        ImGui::InputFloat2("variationPos", &variationPos.x);

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
        // �e�̈ʒu����ǂꂭ�炢����Ă��邩�̃����p
        const XMFLOAT2& pos = checkMarkT->GetPos();
        const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
        XMFLOAT2 variationPos = {
            pos.x - textBackGroundPos.x,
            pos.y - textBackGroundPos.y,
        };
        ImGui::InputFloat2("variationPos", &variationPos.x);

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

        float dissolveValue = checkMark_->GetSpriteDissolve()->GetDissolveValue();
        ImGui::SliderFloat("dissolveValue", &dissolveValue, -0.1f, 1.0f);
        checkMark_->GetSpriteDissolve()->SetDissolveValue(dissolveValue);

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

void BaseTutorial::UpdateCreateEnemy(
    const float elapsedTime,
    const int createCount = 1,
    const int dropExpCount = 0)
{
    using DirectX::XMFLOAT3;

    EnemyManager& enemyManager = EnemyManager::Instance();

    // �w�肵����������������悤�ɂ���
    if (enemyManager.GetEnemyCount() >= createCount)
    {
        // �����^�C�}�[���Z�b�g
        createTimer_ = 0.0f;

        return;
    }

    // �����^�C�}�[�X�V
    createTimer_ = (std::min)(CREATE_TIME_, createTimer_ + elapsedTime);

    // �������ԂɒB���Ă��Ȃ����return
    if (createTimer_ < CREATE_TIME_) return;

    // �G����
    {
        EnemyAI_Tutorial* tutorialEnemy = new EnemyAI_Tutorial();
        tutorialEnemy->Initialize();

        // �����_���ȃQ�[�g�ʒu���琶��
        const XMFLOAT3 createPos = ::GetGatewayPosition(-1);
        tutorialEnemy->SetPosition(createPos);

        // ���G����
        tutorialEnemy->SetIsInvincible(false);

        // �h���b�v�o���l�ݒ�
        tutorialEnemy->SetDropExpCount(dropExpCount);

        enemyManager.Register(tutorialEnemy);
    }

    // �����^�C�}�[���Z�b�g
    createTimer_ = 0.0f;

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
    const Mouse&   mouse   = Input::Instance().GetMouse();

    // �}�E�X�̃J�����ړ����͂�����Ă�����true��Ԃ�
    if (mouse.GetPositionX() != mouse.GetOldPositionX() ||
        mouse.GetPositionY() != mouse.GetOldPositionY())
    {
        return true;
    }

    // �L�[�{�[�h�̃J�����ړ����͂�����Ă�����true��Ԃ�
    if (gamePad.GetAxisRX() != 0.0f ||
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

void TutorialLockOn::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // �G�����X�V
    UpdateCreateEnemy(elapsedTime);

    BaseTutorial::Update(elapsedTime);
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

    // ���b�N�I�����L���ɂȂ��Ă����true��Ԃ�
    if (true == camera.GetActiveLockOn())
    {
        return true;
    }

    return false;
}

#pragma endregion


// ��U���`���[�g���A��
#pragma region TutorialLowAttack

TutorialLowAttack::TutorialLowAttack()
{
    Graphics& graphics = Graphics::Instance();

    // �e�L�X�g�X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/LowAttack.png"
        );
    }
}

void TutorialLowAttack::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // �G�����X�V
    UpdateCreateEnemy(elapsedTime);

    BaseTutorial::Update(elapsedTime);
}

void TutorialLowAttack::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialLowAttack"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialLowAttack::MoveNextStepJudgment()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    if (true == Player::InputJabAttack()) { return true; }

    return false;
}

#pragma endregion

// ���U���`���[�g���A��
#pragma region TutorialHighAttack

TutorialHighAttack::TutorialHighAttack()
{
    Graphics& graphics = Graphics::Instance();

    // �e�L�X�g�X�v���C�g����
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/HighAttack.png"
        );
    }
}

void TutorialHighAttack::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // �G�����X�V
    UpdateCreateEnemy(elapsedTime);

    BaseTutorial::Update(elapsedTime);
}

void TutorialHighAttack::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialHighAttack"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialHighAttack::MoveNextStepJudgment()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    if (true == Player::InputHardAttack()) { return true; }

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
    const GamePad& gamePad = Input::Instance().GetGamePad();

    if (true == Player::InputAvoid()) { return true; }

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
            //L"./Resources/Image/Tutorial/Texts/LevelUp.png"
            L"./Resources/Image/Tutorial/Texts/LockOn.png"
        );
    }

}

void TutorialLevelUp::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // �G�����X�V
    const int createCount = 3;
    const int dropExpCount = 8;
    UpdateCreateEnemy(elapsedTime, createCount, dropExpCount);

    BaseTutorial::Update(elapsedTime);

}

void TutorialLevelUp::Render()
{
    PlayerManager& playerManager = PlayerManager::Instance();

    //// �X�L���J�[�h�I�𒆂͕`�悵�Ȃ��悤�ɂ���
    //Player* player = playerManager.GetPlayer().get();
    //if (true == player->isSelectingSkill) { return; }

    BaseTutorial::Render();
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
    PlayerManager& playerManager = PlayerManager::Instance();

    // ���x�����オ������true��Ԃ�
    Player* player = playerManager.GetPlayer().get();
    if (player->GetLevel() > 1) { return true; }

    return false;
}

#pragma endregion
