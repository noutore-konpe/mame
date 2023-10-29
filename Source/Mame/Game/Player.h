#pragma once

#include "Character.h"
#include "StateMachine.h"
#include "../Input/Input.h"

#include "BaseSkill.h"
#include "AttackSkill.h"
#include "ParametricSkill.h"

#include "AbilityManager.h"

#include "../Graphics/Effect.h"

class Player : public Character
{
public: // enum関連
    // ステート
    enum STATE
    {
        NORMAL,   // 移動、待機等
        ATTACK_JAB,   // 弱攻撃
        ATTACK_HARD,   // 強攻撃
        AVOID,   // 回避
        DIE,   // 死亡
        STAGGER_SOFT,   // 小怯み
        STAGGER_HARD,   // 大怯み
        COUNTER,   // カウンター
    };

    // アニメーション
    enum Animation
    {
        Idle,
        Dash,
        Jab_1,
        Jab_2,
        Jab_3,
        Avoid,
        Counter,
        CounterAttack,
        SoftStagger,
        HardStagger,//吹っ飛び、死亡
        StandUp,
        HardAttack,
        EnemyWalk
    };


public:
    Player();
    ~Player() override;

    void Initialize() override;                     // 初期化
    void Finalize();                                // 終了化

    void Begin();                                   // 毎フレーム一番最初に呼ばれる
    void Update(const float elapsedTime) override; // 更新処理
    void End();                                     // 毎フレーム一番最後に呼ばれる

    enum HitReaction
    {
        NONE,
        SOFT,
        HARD
    };

    DamageResult ApplyDamage(float damage, const DirectX::XMFLOAT3 hitPosition,Character* attacker = nullptr,float invincibleTime = 0, bool ignoreDefence = false, DirectX::XMFLOAT4 color = { 1,0,0,1 })override;
    DamageResult ApplyDamage(float damage, const DirectX::XMFLOAT3 hitPosition, const HitReaction reaction, Character* attacker = nullptr, float invincibleTime = 0, bool ignoreDefence = false);

    void MoveUpdate(float elapsedTime, float ax, float ay);
    void UpdateVelocity(float elapsedTime, float ax, float ay);

    //入力をカメラから見たベクトルに変更しmoveVec変数に入れる関数
    void MoveVecUpdate(float ax, float ay);

    DirectX::XMFLOAT2 ConvertToCameraMoveVec(float ax,float ay);

    void AvoidUpdate(float elapsedTime);

    void ModelRotZUpdate(float elapsedTime);

    void CameraControllerUpdate(float elapsedTime);

    void Render(const float scale, ID3D11PixelShader* psShader = nullptr) override; // 描画処理



    void DrawDebug() override;  // ImGui用

    void PlayAnimation(Animation index, bool loop, float speed = 1.0f) { Character::PlayAnimation(static_cast<int>(index), loop, speed); }

    //汎用関数
    void PlayWalkAnimation() { PlayBlendAnimation(Idle, Dash, true); }
    void PlaySwordWalkAnimation() { swordModel->PlayBlendAnimation(Idle, Dash, true); }

    void AttackSteppingUpdate(float elapsedTime);//攻撃間際の踏み込み処理

    void OnDamaged()override;
    void OnDead(DamageResult result)override;

    void ChangeState(int newState);


    void ResetSteppingTimer() { steppingTimer = 0; }

    //レベルアップ時すべての更新処理を止めてスキル選択演出をする
    void SelectSkillUpdate(float elapsedTime);

    BaseSkill* Lottery();//カードを抽選
    void DrawCards();//３つのスキルカードを抽選　drawingSkillCardsに格納

    //ロックオンしているターゲット切り替え 1なら右側の敵へロック -1なら左側ロック
    bool ChangeLockOnTarget(float ax);

    void LockOnUpdate();

    void LockOnInitialize();

    const bool BlownUpdate(float elapsedTime);//吹っ飛び更新処理(吹っ飛び中true)
    void Blow(DirectX::XMFLOAT3 blowVec/*吹き飛ぶ方向*/);//吹っ飛しするときに呼ぶ

    void ActiveCounter();

    void PlayLaserEffect();

    void TurnNearEnemy(float radius,float elapsedTime);



private:
    float blowTime = 1.0f;
    float blowTimer;
    float blowSpeed = 9.0f;
    DirectX::XMFLOAT3 blowVec;



public:
    //入力関数
    static bool InputJabAttack()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_X) ||
            (Input::Instance().GetMouse().GetButtonDown() & Mouse::BTN_LEFT);
    }

    static bool InputHardAttack()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_Y) ||
            (Input::Instance().GetMouse().GetButtonDown() & Mouse::BTN_RIGHT);
    }

    static bool InputDash()
    {
        return (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_RIGHT_SHOULDER);
    }

    static bool InputAvoid()
    {
        return (Input::Instance().GetGamePad().GetTriggerR() > 0.5f);
    }

    static bool InputDecide()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_A);
    }

    static bool InputLockOn()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_THUMB);
    }

    static bool InputCounter()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_LEFT_SHOULDER);
    }

    //getter,setter
    void AddMaxSpeed(const float spd) { maxEyeSpeed += spd; }
    void AddMaxDodgeSpeed(const float spd) { maxDodgeSpeed += spd; }

    void AddBaseAttackPower(const float power) { baseAttackPower += power; }

    const float GetAttackSpeed() const { return attackSpeed; }
    void SetAttackSpeed(const float spd) { attackSpeed = spd; }
    void AddAttackSpeed(const float spd) { attackSpeed += spd; }

    void SetAcceleration(const float accel) { acceleration = accel; }

    StateMachine<State<Player>>* GetStateMachine() { return stateMachine.get(); }

    void SetVelocity(const DirectX::XMFLOAT3 velo) { velocity = velo; }

    Model* GetSword() { return swordModel.get(); }


    void AddExp(const float exp) { curExp += exp; totalExp += exp; }
    const float GetCurExp() const { return curExp; }
    const float GetTotalExp() const { return totalExp; }
    const float GetLevelUpExp() const { return levelUpExp; }

    void ApplyExp(const float exp)
    {
        AddExp(exp);
        auto pos = GetTransform()->GetPosition();
        pos.y += 0.7f;
        expEffect->Play(pos,DirectX::XMFLOAT3(1,1,1),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,0.4f));//エフェクト再生
    }

    const int GetLevel() const { return level; }

    AbilityManager* GetAbilityManager() { return &abilityManager_; }

    const float GetLifeTimer() { return lifeTimer; };

    Effect* GetLaserEffect() { return laserEffect.get(); }

    const float GetBasePower() const { return baseAttackPower; }
    //---------------------------スキル-------------------------------


    //-----------------------------------------------------------------
public:
    bool isCounter;//カウンター受付時間か？
    bool counterCompleted;//カウンター成立フラグ
    float counterDamage;//カウンターをした攻撃のダメージ

    const BLOW_OFF_FORCE_LEVEL& GetInflictBlowOffForceLevel() const { return inflictBlowOffForceLevel_; }
    void SetInflictBlowOffForceLevel(const BLOW_OFF_FORCE_LEVEL& inflictBlowOffForceLevel) { inflictBlowOffForceLevel_ = inflictBlowOffForceLevel; }


    void LevelUpdate();

    //地形判定後の座標取得
    DirectX::XMFLOAT3 CollidedPosition(const DirectX::XMFLOAT3 pos);

public:
    bool isSelectingSkill;//能力の選択演出中かのフラグ

    static constexpr float InitAcceleration = 10.0f;

    float actualRotValue;//回避中、実際に回転させるZ値

    bool isActiveAttackFrame;
private:
    //----------------------------シェーダー----------------------------------
    Microsoft::WRL::ComPtr<ID3D11PixelShader> playerPS;
    //----------------------------------------------------------------------

    //----------------------------カメラ関係----------------------------------
    float cameraRotSpeed = 2.0f;//旋回速度
    float cameraRotSpeedMouseX = 0.1f;
    float cameraRotSpeedMouseY = 0.18f;
    //-----------------------------------------------------------------------

    //--------------------------移動-----------------------------------------
    float maxEyeSpeed;
    //float maxDashSpeed = 4.0f;
    DirectX::XMFLOAT3 velocity{};

    //カメラの向いている方向を前とした移動方向ベクトル
    DirectX::XMFLOAT3 moveVec;

    float deceleration;
    float acceleration;
    float dodgeAcceleration = 30.0f;

    float rotTimer = 0;
    //-----------------------------------------------------------------------

    //--------------------------レベル-----------------------------------------
    int level;
    float curExp;//現在のレベルに上がってからの経験値
    float totalExp;//今までの経験値の総量
    float levelUpExp;//レベルを上げるのに必要な経験値
    //-----------------------------------------------------------------------

    //---------------------------ステートマシン-------------------------------
    std::unique_ptr<StateMachine<State<Player>>> stateMachine;
    //-----------------------------------------------------------------------


    //---------------------------スキル-------------------------------
    std::vector<BaseSkill*>* skillArray;

    BaseSkill* drawingSkillCards[3];
    int drawDirectionState;//カードドロー演出ステート
    float drawDirectionTime = 0.5f;//カードが画面上から落ちてきて止まるまでの時間
    float drawDirectionTimer = 0;//演出に使う用のタイマー
    int selectCard = 0;

    //---------------------------攻撃関係-------------------------------
    float baseAttackPower;//基礎攻撃力（これにモーション倍率を掛けて計算する）

    float attackSpeed;//攻撃速度

    //踏み込み処理はイージングを使って行う
    //踏み込み中は自機の回転も可能にする
    const float steppingSpeed = 5.0f;//攻撃間際の踏み込み最高速度
    const float steppingTime = 0.2f;//踏み込み時間
    float steppingTimer = 0;

public://getter作るのめんどいだけ
    float jabMotionAtkMuls[3];
    float hardAtkMuls;
    float counterMuls;

    float poisonSlipDamage;
    float poisonEffectTime;
    //-----------------------------------------------------------------------
private:
    //----------------------------回避---------------------------------------
    float maxDodgeSpeed;//回避中の移動速度
    //-----------------------------------------------------------------------


    //-----------------------------剣----------------------------------------
    std::unique_ptr<Model> swordModel;
    //-----------------------------------------------------------------------

    //--------------------------------喰らい、攻撃判定------------------------------------
    //std::unique_ptr<Model> stageDebugSphere;

    enum class HitColName
    {
        NECK,
        HIP,
        R_LEG,
        L_LEG,
        R_LEG_END,
        L_LEG_END,
        R_ELBOW,
        L_ELBOW,
        END
    };

    float swordScale;//剣の大きさに合わせて判定の大きさも変える

    float swordColliderRadius;//剣の判定の大きさ
    int swordColliderNum;//判定の数

    void ColliderPosUpdate(const float& scale);//各ジョイントに判定をつける処理

    //--------------------------------------------------------------------------------

    // 敵に与える吹っ飛ばす力の度合い（※各攻撃の開始時に設定しておく）
    BLOW_OFF_FORCE_LEVEL inflictBlowOffForceLevel_ = BLOW_OFF_FORCE_LEVEL::NONE;


    // アビリティマネージャー(仮)
    AbilityManager abilityManager_ = {};


    //生存時間
    float lifeTimer;

    //---------------------------------Effect---------------------------------------------
    std::unique_ptr<Effect> laserEffect;
    std::unique_ptr<Effect> expEffect;
    Effekseer::Handle expHandle;
    //------------------------------------------------------------------------------------

    //--- result ---
public:
    void SetIsResult() { isResult = true; }
    bool GetIsResult() { return isResult; }

private:
    bool isResult = false;
};

