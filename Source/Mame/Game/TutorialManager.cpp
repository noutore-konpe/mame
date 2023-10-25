#include "TutorialManager.h"
#include "../../Taki174/Common.h"
#include "../Graphics/Graphics.h"

void TutorialManager::Initialize()
{
    // �`���[�g���A���X�e�b�v������
    tutorialStep_ = TUTORIAL_STEP::NO_TUTORIAL;

    // ���̃`���[�g���A���ݒ�
    SetNextTutorial();

    // �`���[�g���A���B���t���O���Z�b�g
    tutorialCompleteFlags_ = false;

}

const bool TutorialManager::Update(const float elapsedTime)
{
    // �`���[�g���A���B���t���O�������Ă�����
    // ���̃`���[�g���A���Ɉڂ�
    if (true == tutorialCompleteFlags_)
    {
        // ���̃`���[�g���A���Ɉڂ�
        const bool tutorialExecuteFlag = SetNextTutorial();

        // �`���[�g���A�����Ō�܂ōs���Ă�����I������
        if (false == tutorialExecuteFlag)
        {
            // �`���[�g���A���I��
            return false;
        }

        // �`���[�g���A���B���t���O�����Z�b�g
        tutorialCompleteFlags_ = false;

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

void TutorialManager::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialManager"))
    {
        // �`���[�g���A��ImGui�`��
        tutorial_->DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialManager::SetNextTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // �`���[�g���A���X�e�b�v���Z
    ::IncrementEnumClass(&tutorialStep_);

    // �`���[�g���A�����Ō�܂ōs���Ă�����`���[�g���A���I��
    if (tutorialStep_ >= TUTORIAL_STEP::COUNT)
    {
        return false;
    }

    // �`���[�g���A���������
    switch (tutorialStep_)
    {
    case TUTORIAL_STEP::MOVE:        tutorial_ = std::make_unique<TutorialMove>();       break;
    case TUTORIAL_STEP::MOVE_CAMERA: tutorial_ = std::make_unique<TutorialMoveCamera>(); break;
    case TUTORIAL_STEP::LOCK_ON:     tutorial_ = std::make_unique<TutorialLockOn>();     break;
    case TUTORIAL_STEP::LOW_ATTACK:  tutorial_ = std::make_unique<TutorialLowAttack>();  break;
    case TUTORIAL_STEP::HIGH_ATTACK: tutorial_ = std::make_unique<TutorialHighAttack>(); break;
    case TUTORIAL_STEP::AVOID:       tutorial_ = std::make_unique<TutorialAvoid>();      break;
    case TUTORIAL_STEP::LEVEL_UP:    tutorial_ = std::make_unique<TutorialLevelUp>();    break;
    }

    // �`���[�g���A��������
    tutorial_->Initialize();

    return true;
}
