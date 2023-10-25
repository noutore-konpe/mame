#pragma once

#include <memory>
#include "../Resource/sprite.h"

// �x�[�X�`���[�g���A��
class BaseTutorial
{
public:
    BaseTutorial();
    virtual ~BaseTutorial() = default;

    virtual void Initialize();
    virtual void Update(const float elapsedTime);
    virtual void Render();
    virtual void DrawImGui();

    // ���̃X�e�b�v�Ɉڂ锻��
    virtual const bool MoveNextStepJudgment() = 0;

protected:
    const bool colorAlphaEaseIn(float* colorAlpha, const float endTime = 1.0f);  // �s�����x����
    const bool colorAlphaEaseOut(float* colorAlpha, const float endTime = 2.0f); // �s�����x����

protected:
    std::unique_ptr<Sprite> text_;
    std::unique_ptr<Sprite> checkMark_;
    std::unique_ptr<Sprite> textBackGround_;

    float   easingTimer_    = 0.0f;
    int     step_           = 0;

};


// �ړ��`���[�g���A��
class TutorialMove : public BaseTutorial
{
public:
    TutorialMove();
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;

};

// �J�����ړ��`���[�g���A��
class TutorialMoveCamera : public BaseTutorial
{
public:
    TutorialMoveCamera();
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;
};

// ���b�N�I���`���[�g���A��
class TutorialLockOn : public BaseTutorial
{
public:
    TutorialLockOn();
    void Initialize() override;
    void DrawImGui()  override;

private:
    const bool MoveNextStepJudgment() override;
};

// �U���`���[�g���A��
class TutorialAttack : public BaseTutorial
{
public:
    TutorialAttack();
    void Initialize() override;
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;
};

// ����`���[�g���A��
class TutorialAvoid : public BaseTutorial
{
public:
    TutorialAvoid();
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;
};

// ���x���A�b�v�`���[�g���A��
class TutorialLevelUp : public BaseTutorial
{
public:
    TutorialLevelUp();
    void Update(const float elapsedTime) override;
    void Render() override;
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;

    void UpdateCreateEnemy(const float elapsedTime);

private:
    static constexpr float CREATE_TIME_ = 1.0f;

private:
    float createTimer_ = 0.0f; // �G�����^�C�}�[

};