#pragma once

#include <memory>
#include "Tutorial.h"

enum class TUTORIAL_STEP
{
    MOVE,           // �ړ�
    MOVE_CAMERA,    // �J�����ړ�
    LOCK_ON,        // ���b�N�I��
    ATTACK,         // �U��
    AVOID,          // ���
    LEVEL_UP,       // ���x���A�b�v

    COUNT,          // �`���[�g���A����
};

class TutorialManager
{
private:
    TutorialManager()  = default;
    ~TutorialManager() = default;

public:
    static TutorialManager& Instance()
    {
        static TutorialManager instance;
        return instance;
    }

    void Initialize();
    const bool Update(const float elapsedTime); // true�F���s�� false�F�I��
    void Render();

    // ���̃`���[�g���A����ݒ�(���`���[�g���A�����Ō�܂ōs���Ă�����false��Ԃ�)
    const bool SetNextTutorial();

    // n�Ԗڂ̃`���[�g���A���B���t���O��ON�ɂ���
    void CompleteTutorialAt(const TUTORIAL_STEP step) { tutorialCompleteFlags_[static_cast<int>(step)] = true; }

public:
    // ���݂̃`���[�g���A���ԍ��擾
    [[nodiscard]] const int GetTutorialIndex() const { return static_cast<int>(tutorialStep_); }

public:
    // �`���[�g���A���̐�
    static constexpr int TUTORIAL_COUNT_ = static_cast<int>(TUTORIAL_STEP::COUNT);

private:
    // �`���[�g���A��
    std::unique_ptr<BaseTutorial> tutorial_;

    // ���݂̃`���[�g���A���X�e�b�v
    TUTORIAL_STEP tutorialStep_ = TUTORIAL_STEP::MOVE;

    // �`���[�g���A���B���t���O
    bool tutorialCompleteFlags_[TUTORIAL_COUNT_] = {false};

};

