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

    void UpdateCreateEnemy(const float elapsedTime, const int createCount, const int dropExpCount);

public:
    static constexpr float CREATE_TIME_ = 1.0f;

protected:
    std::unique_ptr<Sprite> text_;
    std::unique_ptr<Sprite> checkMark_;
    std::unique_ptr<Sprite> textBackGround_;

    float   easingTimer_    = 0.0f;
    float   createTimer_    = CREATE_TIME_; // �G�����^�C�}�[
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
    void Update(const float elapsedTime) override;
    void DrawImGui()  override;

private:
    const bool MoveNextStepJudgment() override;
};

// ��U���`���[�g���A��
class TutorialLowAttack : public BaseTutorial
{
public:
    TutorialLowAttack();
    void Update(const float elapsedTime) override;
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;
};

// ���U���`���[�g���A��
class TutorialHighAttack : public BaseTutorial
{
public:
    TutorialHighAttack();
    void Update(const float elapsedTime) override;
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

// �`���[�g���A���G���h
class TutorialEnd : public BaseTutorial
{
public:
    TutorialEnd();
    void Initialize() override;
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;

};