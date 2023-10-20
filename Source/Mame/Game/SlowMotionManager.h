#pragma once

class SlowMotionManager
{
private:
    enum class STEP
    {
        INIT,
        EASE_IN,
        SLOW_MOTION,
        EASE_OUT,
    };

private:
    SlowMotionManager()  = default;
    ~SlowMotionManager() = default;

public:
    static SlowMotionManager& Instance()
    {
        static SlowMotionManager instance;
        return instance;
    }

    void Initialize();
    void Update(const float elapsedTime);
    void DrawDebug();

    /// <summary>
    /// スローモーション実行
    /// </summary>
    /// <param name="time:スローモーション時間"></param>
    /// <param name="percentage:スローモーションの割合(0.0f ~ 1.0f)(値が小さいほどスローになる)"></param>
    /// <param name="easeOutTime:スローモーションに入るまでの所要時間"></param>
    /// <param name="easeInTime:スローモーションが終わりきるまでの所要時間"></param>
    void ExecuteSlowMotion(
        const float time        = 1.5f,
        const float percentage  = 0.5f,
        const float easeOutTime = 0.0f,
        const float easeInTime  = 1.0f
    );

public:
    // 戻り値の破棄警告
    [[nodiscard]] const bool GetSlowMotionFlag() const { return slowMotionFlag_; }
    [[nodiscard]] const float GetCurrentPercentage() const { return currentPercentage_; }

private:
    STEP    step_                   = STEP::INIT;       // ステップ
    STEP    nextStep_               = STEP::EASE_IN;    // 次のステップ
    float   timer_                  = 0.0f;             // スローモーションタイマー
    float   endTime_                = 0.0f;             // スローモーション終了時間
    float   currentPercentage_      = 1.0f;             // 現在のスローモーションの割合(0.0f ~ 1.0f)(値が小さいほどスローになる)
    float   targetPercentage_       = 1.0f;             // 目標のスローモーションの割合(0.0f ~ 1.0f)(値が小さいほどスローになる)
    float   easeOutTime_            = 0.0f;             // スローモーションに入りきるまでの時間
    float   easeInTime_             = 0.0f;             // スローモーションが終わりきるまでの時間
    bool    slowMotionFlag_         = false;            // スローモーションフラグ

};

