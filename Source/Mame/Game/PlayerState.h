#pragma once
#include "State.h"

class Player;
namespace PlayerState
{
    //移動、待機を含めたステート
    class NormalState : public State<Player>
    {
    public:
        NormalState(Player* player) : State(player, "Normal") {}
        ~NormalState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;

    private:
        bool enableInputButton = false;//入力受付可能（回避が連続ででないようにするため）  
    };

    //攻撃
    class JabAttackState : public State<Player>
    {
    public:
        JabAttackState(Player* player) : State(player, "Jab Attack") {}
        ~JabAttackState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;

    private:
        void HitCollisionUpdate();//判定処理はPlayerでやった方がいいかも...
        void AttackUpdate(int dodgeCanselFrame,int comboCanselFrame);

    private:
        enum ATTACK_STATE
        {
            UPDATE_FRAME,//キャンセル不可
            DODGE_CANSEL_FRAME,//回避キャンセル可能
            COMBO_AND_DODGE_CANSEL_FRAME,//連撃、回避キャンセル可能
        };
        int state;

        int combo;//現在の連撃回数
        bool initialize;//コンボ毎の初期化処理フラグ

        bool collisionOn;//判定処理をするか

        const float dodgeCanselFrame1 = 10.0f;
        const float dodgeCanselFrame2 = 10.0f;
        const float dodgeCanselFrame3 = 10.0f;
        const float comboCanselFrame1 = 20.0f;
        const float comboCanselFrame2 = 20.0f;
        const float comboCanselFrame3 = 20.0f;
    };

    //強攻撃
    class HardAttackState : public State<Player>
    {
    public:
        HardAttackState(Player* player) : State(player, "Hard Attack") {}
        ~HardAttackState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //回避
    class AvoidState : public State<Player>
    {
    public:
        AvoidState(Player* player) : State(player, "Avoid") {}
        ~AvoidState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;

    private:
        float dodgeSpeed  = 20.0f;
    };

    //死亡
    class DieState : public State<Player>
    {
    public:
        DieState(Player* player) : State(player, "Die") {}
        ~DieState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //小怯み
    class SoftStaggerState : public State<Player>
    {
    public:
        SoftStaggerState(Player* player) : State(player, "Soft Stagger") {}
        ~SoftStaggerState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //大怯み(吹っ飛び)
    class HardStaggerState : public State<Player>
    {
    public:
        HardStaggerState(Player* player) : State(player, "Hard Stagger") {}
        ~HardStaggerState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;
    };

    //カウンター
    class CounterState : public State<Player>
    {
    public:
        CounterState(Player* player) : State(player, "Counter") {}
        ~CounterState() {}

        void Initialize() override;
        void Update(const float& elapsedTime) override;
        void Finalize() override;

    private:
        bool counterCompleted;//カウンター成立フラグ
        int state;

        const int startUpFrame = 10;//カウンター受付開始アニメーション
        const float receptionTime = 0.3f;//カウンター受付時間
        float timer;
    };
}