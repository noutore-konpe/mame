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
    /// �X���[���[�V�������s
    /// </summary>
    /// <param name="time:�X���[���[�V��������"></param>
    /// <param name="percentage:�X���[���[�V�����̊���(0.0f ~ 1.0f)(�l���������قǃX���[�ɂȂ�)"></param>
    /// <param name="easeOutTime:�X���[���[�V�����ɓ���܂ł̏��v����"></param>
    /// <param name="easeInTime:�X���[���[�V�������I��肫��܂ł̏��v����"></param>
    void ExecuteSlowMotion(
        const float time        = 1.5f,
        const float percentage  = 0.5f,
        const float easeOutTime = 0.0f,
        const float easeInTime  = 1.0f
    );

public:
    // �߂�l�̔j���x��
    [[nodiscard]] const bool GetSlowMotionFlag() const { return slowMotionFlag_; }
    [[nodiscard]] const float GetCurrentPercentage() const { return currentPercentage_; }

private:
    STEP    step_                   = STEP::INIT;       // �X�e�b�v
    STEP    nextStep_               = STEP::EASE_IN;    // ���̃X�e�b�v
    float   timer_                  = 0.0f;             // �X���[���[�V�����^�C�}�[
    float   endTime_                = 0.0f;             // �X���[���[�V�����I������
    float   currentPercentage_      = 1.0f;             // ���݂̃X���[���[�V�����̊���(0.0f ~ 1.0f)(�l���������قǃX���[�ɂȂ�)
    float   targetPercentage_       = 1.0f;             // �ڕW�̃X���[���[�V�����̊���(0.0f ~ 1.0f)(�l���������قǃX���[�ɂȂ�)
    float   easeOutTime_            = 0.0f;             // �X���[���[�V�����ɓ��肫��܂ł̎���
    float   easeInTime_             = 0.0f;             // �X���[���[�V�������I��肫��܂ł̎���
    bool    slowMotionFlag_         = false;            // �X���[���[�V�����t���O

};

