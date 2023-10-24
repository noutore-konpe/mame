#pragma once

#include <memory>
#include "../Resource/sprite.h"

class BaseTutorial
{
public:
    BaseTutorial() = default;
    virtual ~BaseTutorial() = default;

    virtual void Initialize() = 0;
    virtual void Update(const float elapsedTime) = 0;
    virtual void Render() = 0;

};

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