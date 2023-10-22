#include "TutorialManager.h"
#include "../Graphics/Graphics.h"

// �e�L�X�g�摜�p�X�̔z��
static constexpr wchar_t* const TUTORIAL_TEXTS_[static_cast<int>(TUTORIAL::COUNT)] = {
    L"./Resources/UI/Text0.png",
    L"./Resources/UI/Text0.png",
    L"./Resources/UI/Text0.png",
};

// �e�L�X�g�ʒu�̔z��
static constexpr DirectX::XMFLOAT2 TUTORIAL_TEXT_POSITIONS_[static_cast<int>(TUTORIAL::COUNT)] = {
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
};


void TutorialManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // ���݂̃`���[�g���A���ԍ�������
    currentTutorialIndex_ = 0;

    // �X�v���C�g����
    {
        // �e�L�X�g����
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            TUTORIAL_TEXTS_[currentTutorialIndex_]
        );

        // �`�F�b�N�}�[�N����
        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/UI/CheckMark.png"
        );

        // �e�L�X�g�w�i����
        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/UI/TextBackGround.png"
        );
    }

    // �p�����[�^������
    {
        text_->Initialize(
            TUTORIAL_TEXT_POSITIONS_[currentTutorialIndex_],
            textParam_.size_,
            textParam_.texSize_,
            textParam_.texPos_,
            textParam_.color_,
            textParam_.angle_
        );

        checkMark_->Initialize(
            text_->GetSpriteTransform()->GetPos(),
            checkMarkParam_.size_,
            checkMarkParam_.texSize_,
            checkMarkParam_.texPos_,
            checkMarkParam_.color_,
            checkMarkParam_.angle_
        );

        textBackGround_->Initialize(
            text_->GetSpriteTransform()->GetPos(),
            textBackGroundParam_.size_,
            textBackGroundParam_.texSize_,
            textBackGroundParam_.texPos_,
            textBackGroundParam_.color_,
            textBackGroundParam_.angle_
        );

        // �`���[�g���A���B���t���O���Z�b�g
        for (int i = 0; i < TUTORIAL_COUNT_; ++i)
        {
            tutorialCompleteFlag_[i] = false;
        }

    }

}

const bool TutorialManager::Update(const float elapsedTime)
{
    // ���݂̃`���[�g���A���̒B���t���O�������Ă�����
    // ���̃`���[�g���A���Ɉڂ�
    if (true == tutorialCompleteFlag_[currentTutorialIndex_])
    {
        // �`���[�g���A���I���t���O���擾
        const bool tutorialEndFlag = MoveNextTutorial(); // ���̃`���[�g���A���Ɉڂ�

        // �`���[�g���A�����Ō�܂ōs���Ă�����I������
        if (true == tutorialEndFlag)
        {
            // �`���[�g���A���I��
            return false;
        }

        // �`���[�g���A�����s��
        return true;
    }


    // �`���[�g���A�����s��
    return true;
}

void TutorialManager::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

const bool TutorialManager::MoveNextTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // �`���[�g���A���ԍ����Z
    ++currentTutorialIndex_;

    // �`���[�g���A�����Ō�܂ōs������`���[�g���A���I��
    if (currentTutorialIndex_ >= TUTORIAL_COUNT_)
    {
        return false;
    }

    // �e�L�X�g�Đ���
    text_ = std::make_unique<Sprite>(
        graphics.GetDevice(),
        TUTORIAL_TEXTS_[currentTutorialIndex_]
    );

    // �e�L�X�g������
    text_->Initialize(
        TUTORIAL_TEXT_POSITIONS_[currentTutorialIndex_],
        textParam_.size_,
        textParam_.texSize_,
        textParam_.texPos_,
        textParam_.color_,
        textParam_.angle_
    );

    return true;
}
