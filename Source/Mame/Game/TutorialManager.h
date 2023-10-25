#pragma once

#include <memory>
#include "Tutorial.h"

enum class TUTORIAL_STEP
{
    NO_TUTORIAL = -1,   // �f�t�H���g
    MOVE,               // �ړ�
    MOVE_CAMERA,        // �J�����ړ�
    LOCK_ON,            // ���b�N�I��
    LOW_ATTACK,         // ��U��
    HIGH_ATTACK,        // ���U��
    AVOID,              // ���
    LEVEL_UP,           // ���x���A�b�v

    COUNT,              // �`���[�g���A����
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
    void DrawImGui();

    // ���̃`���[�g���A����ݒ�(���`���[�g���A�����Ō�܂ōs���Ă�����false��Ԃ�)
    const bool SetNextTutorial();

    // �`���[�g���A���B���t���O��ON�ɂ���
    void CompleteTutorial() { tutorialCompleteFlags_ = true; }

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
    TUTORIAL_STEP tutorialStep_ = TUTORIAL_STEP::NO_TUTORIAL;

    // �`���[�g���A���B���t���O
    bool tutorialCompleteFlags_ = false;

};

