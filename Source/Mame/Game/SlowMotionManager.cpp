#include "SlowMotionManager.h"

#include <utility>
#include "../../imgui/imgui.h"
#include "../Other/Easing.h"

void SlowMotionManager::Initialize()
{
    // パラメータ初期化
    step_               = STEP::INIT;
    nextStep_           = STEP::EASE_IN;
    timer_              = 0.0f;
    endTime_            = 0.0f;
    currentPercentage_  = 1.0f;
    targetPercentage_   = 1.0f;
    easeOutTime_        = 0.0f;
    easeInTime_         = 0.0f;
    slowMotionFlag_     = false; // スローモーションフラグリセット
}

void SlowMotionManager::Update(const float elapsedTime)
{
    // スローモーションフラグが立っていなければreturn
    if (false == slowMotionFlag_) return;

    switch (step_)
    {
    case STEP::INIT:
        timer_ = 0.0f;      // タイマーリセット
        step_  = nextStep_; // 次のステップに移る

        break;
    case STEP::EASE_IN:
        // タイマー更新
        timer_ = (std::min)(easeOutTime_, timer_ + elapsedTime);

        // イージング処理(totalTimeがゼロ以下だとバグるので修正しておく)
        currentPercentage_ = {
            (easeOutTime_ > 0.0f)
            ? Easing::OutSine(timer_, easeOutTime_, targetPercentage_, 1.0f)
            : targetPercentage_
        };

        // 目標のスローモーションの割合になったら次のステップに移る
        if (static_cast<const float>(currentPercentage_) == targetPercentage_)
        {
            nextStep_ = STEP::SLOW_MOTION;  // 次のステップを設定
            step_     = STEP::INIT;         // 初期化処理を経由する
            break;
        }

        break;
    case STEP::SLOW_MOTION:
        // タイマー更新
        timer_ = (std::min)(endTime_, timer_ + elapsedTime);

        // スローモーション終了時間になったら次のステップに移る
        if (static_cast<const float>(timer_) == endTime_)
        {
            nextStep_ = STEP::EASE_OUT; // 次のステップを設定
            step_     = STEP::INIT;     // 初期化処理を経由する
            break;
        }

        break;
    case STEP::EASE_OUT:
        // タイマー更新
        timer_ = (std::min)(easeInTime_, timer_ + elapsedTime);

        // イージング処理(totalTimeがゼロ以下だとバグるので修正しておく)
        currentPercentage_ = {
            (easeInTime_ > 0.0f)
            ? Easing::InSine(timer_, easeInTime_, 1.0f, targetPercentage_)
            : 1.0f
        };

        // スローモーションの割合が戻り切ったらスローモーションを終了する
        if (1.0f == currentPercentage_)
        {
            nextStep_           = STEP::EASE_IN; // ステップをリセット
            step_               = STEP::INIT;    // 次のステップをリセット
            slowMotionFlag_     = false;         // スローモーションフラグをリセット
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
        // ステップ
        int step = static_cast<int>(step_);
        ImGui::InputInt("step", &step);

        // タイマー
        float timer = timer_;
        ImGui::InputFloat("time_", &timer);

        // パーセンテージ
        float currentPercentage = currentPercentage_;
        ImGui::InputFloat("currentPercentage", &currentPercentage);

        // スローモーションフラグ
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
    // すでにスローモーションが実行中ならreturn
    if (true == slowMotionFlag_) return;

    // パラメータ設定
    endTime_            = time;
    targetPercentage_   = percentage;
    easeOutTime_        = easeOutTime;
    easeInTime_         = easeInTime;

    // スローモーションフラグを立てる
    slowMotionFlag_     = true;

}
