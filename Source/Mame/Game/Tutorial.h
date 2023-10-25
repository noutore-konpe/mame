#pragma once

#include <memory>
#include "../Resource/sprite.h"

// ベースチュートリアル
class BaseTutorial
{
public:
    BaseTutorial();
    virtual ~BaseTutorial() = default;

    virtual void Initialize();
    virtual void Update(const float elapsedTime);
    virtual void Render();
    virtual void DrawImGui();

    // 次のステップに移る判定
    virtual const bool MoveNextStepJudgment() = 0;

protected:
    const bool colorAlphaEaseIn(float* colorAlpha, const float endTime = 1.0f);  // 不透明度増加
    const bool colorAlphaEaseOut(float* colorAlpha, const float endTime = 2.0f); // 不透明度減少

    void UpdateCreateEnemy(const float elapsedTime, const int createCount, const int dropExpCount);

public:
    static constexpr float CREATE_TIME_ = 1.0f;

protected:
    std::unique_ptr<Sprite> text_;
    std::unique_ptr<Sprite> checkMark_;
    std::unique_ptr<Sprite> textBackGround_;

    float   easingTimer_    = 0.0f;
    float   createTimer_    = CREATE_TIME_; // 敵生成タイマー
    int     step_           = 0;

};


// 移動チュートリアル
class TutorialMove : public BaseTutorial
{
public:
    TutorialMove();
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;

};

// カメラ移動チュートリアル
class TutorialMoveCamera : public BaseTutorial
{
public:
    TutorialMoveCamera();
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;
};

// ロックオンチュートリアル
class TutorialLockOn : public BaseTutorial
{
public:
    TutorialLockOn();
    void Update(const float elapsedTime) override;
    void DrawImGui()  override;

private:
    const bool MoveNextStepJudgment() override;
};

// 弱攻撃チュートリアル
class TutorialLowAttack : public BaseTutorial
{
public:
    TutorialLowAttack();
    void Update(const float elapsedTime) override;
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;
};

// 強攻撃チュートリアル
class TutorialHighAttack : public BaseTutorial
{
public:
    TutorialHighAttack();
    void Update(const float elapsedTime) override;
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;

};

// 回避チュートリアル
class TutorialAvoid : public BaseTutorial
{
public:
    TutorialAvoid();
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;
};

// チュートリアルエンド
class TutorialEnd : public BaseTutorial
{
public:
    TutorialEnd();
    void Initialize() override;
    void DrawImGui() override;

private:
    const bool MoveNextStepJudgment() override;

};