#include "TutorialManager.h"
#include "../Graphics/Graphics.h"

void TutorialManager::Initialize()
{
    // �`���[�g���A���X�e�b�v������
    tutorialstep_ = TUTORIAL_STEP::MOVE;

    // ���̃`���[�g���A���ݒ�
    SetNextTutorial();

    // �`���[�g���A���B���t���O���Z�b�g
    for (bool& flag : tutorialCompleteFlags_) { flag = false; }

}

const bool TutorialManager::Update(const float elapsedTime)
{
    // ���݂̃`���[�g���A���̒B���t���O�������Ă�����
    // ���̃`���[�g���A���Ɉڂ�
    if (true == tutorialCompleteFlags_[GetTutorialIndex()])
    {
        // ���̃`���[�g���A���Ɉڂ�
        const bool tutorialExecuteFlag = SetNextTutorial();

        // �`���[�g���A�����Ō�܂ōs���Ă�����I������
        if (false == tutorialExecuteFlag)
        {
            // �`���[�g���A���I��
            return false;
        }

        // �`���[�g���A�����s��
        return true;
    }

    // �`���[�g���A���X�V
    tutorial_->Update(elapsedTime);

    // �`���[�g���A�����s��
    return true;
}

void TutorialManager::Render()
{
    // �`���[�g���A���`��
    tutorial_->Render();
}

const bool TutorialManager::SetNextTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // �`���[�g���A�����Ō�܂ōs���Ă�����`���[�g���A���I��
    if (tutorialstep_ >= TUTORIAL_STEP::COUNT)
    {
        return false;
    }

    // �`���[�g���A������
    switch (tutorialstep_)
    {
    case TUTORIAL_STEP::MOVE: tutorial_ = std::make_unique<TutorialMove>(); break;
    }

    // �`���[�g���A��������
    tutorial_->Initialize();

    // �`���[�g���A���X�e�b�v���Z
    tutorialstep_ = static_cast<TUTORIAL_STEP>(static_cast<int>(tutorialstep_) + 1);

    return true;
}
