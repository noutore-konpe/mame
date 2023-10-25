#pragma once

#include <memory>
#include "../Resource/sprite.h"

// ベースチュートリアル
class BaseTutorial
{
public:
    BaseTutorial() = default;
    virtual ~BaseTutorial() = default;

    virtual void Initialize() = 0;
    virtual void Update(const float elapsedTime) = 0;
    virtual void Render() = 0;

};


// 移動チュートリアル
class TutorialMove : public BaseTutorial
{
public:
    TutorialMove();
    ~TutorialMove() override = default;

    void Initialize() override;
    void Update(const float elapsedTime) override;
    void Render() override;

private:
    std::unique_ptr<Sprite> text_;
    std::unique_ptr<Sprite> checkMark_;
    std::unique_ptr<Sprite> textBackGround_;

};

// カメラ移動チュートリアル
class TutorialMoveCamera : public BaseTutorial
{
public:
    TutorialMoveCamera();
    ~TutorialMoveCamera() override = default;

    void Initialize() override;
    void Update(const float elapsedTime) override;
    void Render() override;

private:
    std::unique_ptr<Sprite> text_;
    std::unique_ptr<Sprite> checkMark_;
    std::unique_ptr<Sprite> textBackGround_;

};

// ロックオンチュートリアル
class TutorialLockOn : public BaseTutorial
{
public:
    TutorialLockOn();
    ~TutorialLockOn() override = default;

    void Initialize() override;
    void Update(const float elapsedTime) override;
    void Render() override;

private:
    std::unique_ptr<Sprite> text_;
    std::unique_ptr<Sprite> checkMark_;
    std::unique_ptr<Sprite> textBackGround_;

};

// 攻撃チュートリアル
class TutorialAttack : public BaseTutorial
{
public:
    TutorialAttack();
    ~TutorialAttack() override = default;

    void Initialize() override;
    void Update(const float elapsedTime) override;
    void Render() override;

private:
    std::unique_ptr<Sprite> text_;
    std::unique_ptr<Sprite> checkMark_;
    std::unique_ptr<Sprite> textBackGround_;

};

// 回避チュートリアル
class TutorialAvoid : public BaseTutorial
{
public:
    TutorialAvoid();
    ~TutorialAvoid() override = default;

    void Initialize() override;
    void Update(const float elapsedTime) override;
    void Render() override;

private:
    std::unique_ptr<Sprite> text_;
    std::unique_ptr<Sprite> checkMark_;
    std::unique_ptr<Sprite> textBackGround_;

};

// レベルアップチュートリアル
class TutorialLevelUp : public BaseTutorial
{
public:
    TutorialLevelUp();
    ~TutorialLevelUp() override = default;

    void Initialize() override;
    void Update(const float elapsedTime) override;
    void Render() override;

private:
    std::unique_ptr<Sprite> text_;
    std::unique_ptr<Sprite> checkMark_;
    std::unique_ptr<Sprite> textBackGround_;

};