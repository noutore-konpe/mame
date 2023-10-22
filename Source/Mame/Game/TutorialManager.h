#pragma once

#include <memory>
#include "../Resource/sprite.h"

enum class TUTORIAL
{
    MOVE,
    ATTACK,
    AVOID,

    COUNT, // �`���[�g���A����
};

// �`���[�g���A���̃X�v���C�g�p�����[�^
struct TutorialSpriteParam
{
    // ���ʒu�͊܂߂Ȃ�
    DirectX::XMFLOAT2 size_     = {};
    DirectX::XMFLOAT2 texSize_  = {};
    DirectX::XMFLOAT2 texPos_   = {};
    DirectX::XMFLOAT4 color_    = {};
    float             angle_    = 0.0f;
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

    const bool MoveNextTutorial();    // ���̃`���[�g���A���Ɉڂ�(���`���[�g���A�����Ō�܂ōs������false��Ԃ�)

    // n�Ԗڂ̃`���[�g���A���B���t���O��ON�ɂ���
    void CompleteTutorialAt(const TUTORIAL& index) { tutorialCompleteFlag_[static_cast<int>(index)]; }

public:
    [[nodiscard]] const int GetCurrentTutorialIndex() const { return currentTutorialIndex_; }

public:
    static constexpr int TUTORIAL_COUNT_ = static_cast<int>(TUTORIAL::COUNT); // �`���[�g���A���̐�

private:
    std::unique_ptr<Sprite> text_;              // �e�L�X�g
    std::unique_ptr<Sprite> checkMark_;         // �`�F�b�N�}�[�N
    std::unique_ptr<Sprite> textBackGround_;    // �e�L�X�g�w�i

    int currentTutorialIndex_ = 0;              // ���݂̃`���[�g���A���ԍ�

    // �`���[�g���A���B���t���O
    bool tutorialCompleteFlag_[static_cast<int>(TUTORIAL::COUNT)] = { false };

private:
    // �e�X�v���C�g�p�����[�^
    TutorialSpriteParam textParam_ = {
        { 10.0f, 10.0f },
        { 100.0f, 100.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0.0f,
    };
    TutorialSpriteParam checkMarkParam_ = {
        { 10.0f, 10.0f },
        { 100.0f, 100.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0.0f,
    };
    TutorialSpriteParam textBackGroundParam_ = {
        { 10.0f, 10.0f },
        { 100.0f, 100.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0.0f,
    };

};

