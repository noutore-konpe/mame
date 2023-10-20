#include "SlowMotionManager.h"

#include <utility>
#include "../../imgui/imgui.h"
#include "../Other/Easing.h"

void SlowMotionManager::Initialize()
{
    // �p�����[�^������
    step_               = STEP::INIT;
    nextStep_           = STEP::EASE_IN;
    timer_              = 0.0f;
    endTime_            = 0.0f;
    currentPercentage_  = 1.0f;
    targetPercentage_   = 1.0f;
    easeOutTime_        = 0.0f;
    easeInTime_         = 0.0f;
    slowMotionFlag_     = false; // �X���[���[�V�����t���O���Z�b�g
}

void SlowMotionManager::Update(const float elapsedTime)
{
    // �X���[���[�V�����t���O�������Ă��Ȃ����return
    if (false == slowMotionFlag_) return;

    switch (step_)
    {
    case STEP::INIT:
        timer_ = 0.0f;      // �^�C�}�[���Z�b�g
        step_  = nextStep_; // ���̃X�e�b�v�Ɉڂ�

        break;
    case STEP::EASE_IN:
        // �^�C�}�[�X�V
        timer_ = (std::min)(easeOutTime_, timer_ + elapsedTime);

        // �C�[�W���O����(totalTime���[���ȉ����ƃo�O��̂ŏC�����Ă���)
        currentPercentage_ = {
            (easeOutTime_ > 0.0f)
            ? Easing::OutSine(timer_, easeOutTime_, targetPercentage_, 1.0f)
            : targetPercentage_
        };

        // �ڕW�̃X���[���[�V�����̊����ɂȂ����玟�̃X�e�b�v�Ɉڂ�
        if (static_cast<const float>(currentPercentage_) == targetPercentage_)
        {
            nextStep_ = STEP::SLOW_MOTION;  // ���̃X�e�b�v��ݒ�
            step_     = STEP::INIT;         // �������������o�R����
            break;
        }

        break;
    case STEP::SLOW_MOTION:
        // �^�C�}�[�X�V
        timer_ = (std::min)(endTime_, timer_ + elapsedTime);

        // �X���[���[�V�����I�����ԂɂȂ����玟�̃X�e�b�v�Ɉڂ�
        if (static_cast<const float>(timer_) == endTime_)
        {
            nextStep_ = STEP::EASE_OUT; // ���̃X�e�b�v��ݒ�
            step_     = STEP::INIT;     // �������������o�R����
            break;
        }

        break;
    case STEP::EASE_OUT:
        // �^�C�}�[�X�V
        timer_ = (std::min)(easeInTime_, timer_ + elapsedTime);

        // �C�[�W���O����(totalTime���[���ȉ����ƃo�O��̂ŏC�����Ă���)
        currentPercentage_ = {
            (easeInTime_ > 0.0f)
            ? Easing::InSine(timer_, easeInTime_, 1.0f, targetPercentage_)
            : 1.0f
        };

        // �X���[���[�V�����̊������߂�؂�����X���[���[�V�������I������
        if (1.0f == currentPercentage_)
        {
            nextStep_           = STEP::EASE_IN; // �X�e�b�v�����Z�b�g
            step_               = STEP::INIT;    // ���̃X�e�b�v�����Z�b�g
            slowMotionFlag_     = false;         // �X���[���[�V�����t���O�����Z�b�g
            break;
        }

        break;
    }

}

void SlowMotionManager::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu("SlowMotion"))
    {
        // �X�e�b�v
        int step = static_cast<int>(step_);
        ImGui::InputInt("step", &step);

        // �^�C�}�[
        float timer = timer_;
        ImGui::InputFloat("time_", &timer);

        // �p�[�Z���e�[�W
        float currentPercentage = currentPercentage_;
        ImGui::InputFloat("currentPercentage", &currentPercentage);

        // �X���[���[�V�����t���O
        bool slowMotionFlag = slowMotionFlag_;
        ImGui::Checkbox("slowMotionFlag", &slowMotionFlag);

        ImGui::EndMenu();
    }

#endif
}

void SlowMotionManager::ExecuteSlowMotion(
    const float time,
    const float percentage,
    const float easeOutTime,
    const float easeInTime)
{
    // ���łɃX���[���[�V���������s���Ȃ�return
    if (true == slowMotionFlag_) return;

    // �p�����[�^�ݒ�
    endTime_            = time;
    targetPercentage_   = percentage;
    easeOutTime_        = easeOutTime;
    easeInTime_         = easeInTime;

    // �X���[���[�V�����t���O�𗧂Ă�
    slowMotionFlag_     = true;

}
