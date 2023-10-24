#pragma once

#include <memory>
#include "../Resource/sprite.h"

// �x�[�X�`���[�g���A��
class BaseTutorial
{
public:
    BaseTutorial() = default;
    virtual ~BaseTutorial() = default;

    virtual void Initialize() = 0;
    virtual void Update(const float elapsedTime) = 0;
    virtual void Render() = 0;

};


// �ړ��`���[�g���A��
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

// �J�����ړ��`���[�g���A��
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

// ���b�N�I���`���[�g���A��
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

// �U���`���[�g���A��
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

// ����`���[�g���A��
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

// ���x���A�b�v�`���[�g���A��
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