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
public: // enum�֘A
    // �X�e�[�g
    enum STATE
    {
        NORMAL,   // �ړ��A�ҋ@��
        ATTACK_JAB,   // ��U��
        ATTACK_HARD,   // ���U��
        AVOID,   // ���
        DIE,   // ���S
        STAGGER_SOFT,   // ������
        STAGGER_HARD,   // �勯��
        COUNTER,   // �J�E���^�[
    };

    // �A�j���[�V����
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
        HardStagger,//������сA���S
    };


public:
    Player();
    ~Player() override;

    void Initialize() override;                     // ������
    void Finalize();                                // �I����

    void Begin();                                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float elapsedTime) override; // �X�V����
    void End();                                     // ���t���[����ԍŌ�ɌĂ΂��


    DamageResult ApplyDamage(float damage, const DirectX::XMFLOAT3 hitPosition, Character* attacker = nullptr, float invincibleTime = 0)override;

    void MoveUpdate(float elapsedTime, float ax, float ay);
    void UpdateVelocity(float elapsedTime, float ax, float ay);

    //���͂��J�������猩���x�N�g���ɕύX��moveVec�ϐ��ɓ����֐�
    void MoveVecUpdate(float ax, float ay);

    void AvoidUpdate(float elapsedTime);

    void ModelRotZUpdate(float elapsedTime);

    void CameraControllerUpdate(float elapsedTime);

    void Render(const float scale, ID3D11PixelShader* psShader = nullptr) override; // �`�揈��

    

    void DrawDebug() override;  // ImGui�p

    void PlayAnimation(Animation index, bool loop, float speed = 1.0f) { Character::PlayAnimation(static_cast<int>(index), loop, speed); }

    //�ėp�֐�
    void PlayWalkAnimation() { PlayBlendAnimation(Idle, Dash, true); }
    void PlaySwordWalkAnimation() { swordModel->PlayBlendAnimation(Idle, Dash, true); }

    void AttackSteppingUpdate(float elapsedTime);//�U���ԍۂ̓��ݍ��ݏ���

    void OnDamaged()override;
    void OnDead(DamageResult result)override;

    void ChangeState(int newState) { stateMachine->ChangeState(newState); }


    void ResetSteppingTimer() { steppingTimer = 0; }

    //���x���A�b�v�����ׂĂ̍X�V�������~�߂ăX�L���I�����o������
    void SelectSkillUpdate(float elapsedTime);

    BaseSkill* Lottery();//�J�[�h�𒊑I
    void DrawCards();//�R�̃X�L���J�[�h�𒊑I�@drawingSkillCards�Ɋi�[

    //���b�N�I�����Ă���^�[�Q�b�g�؂�ւ� 1�Ȃ�E���̓G�փ��b�N -1�Ȃ獶�����b�N
    bool ChangeLockOnTarget(float ax);

    void LockOnUpdate();

    void LockOnInitialize();

    void BlownUpdate(float elapsedTime);//������эX�V����
    void Blow(DirectX::XMFLOAT3 blowVec/*������ԕ���*/);//�����򂵂���Ƃ��ɌĂ�

    void ActiveCounter();

    void PlayLaserEffect();

private:
    float blowTime = 1.0f;
    float blowTimer;
    float blowSpeed = 9.0f;
    DirectX::XMFLOAT3 blowVec;



public:
    //���͊֐�
    static bool InputJabAttack()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_X);
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

    const int GetLevel() const { return level; }

    AbilityManager* GetAbilityManager() { return &abilityManager_; }

    const float GetLifeTimer() { return lifeTimer; };

    Effect* GetLaserEffect() { return laserEffect.get(); }

    //---------------------------�X�L��-------------------------------
   

    //-----------------------------------------------------------------
public:
    bool isCounter;//�J�E���^�[��t���Ԃ��H
    bool counterCompleted;//�J�E���^�[�����t���O

    const BLOW_OFF_FORCE_LEVEL& GetInflictBlowOffForceLevel() const { return inflictBlowOffForceLevel_; }
    void SetInflictBlowOffForceLevel(const BLOW_OFF_FORCE_LEVEL& inflictBlowOffForceLevel) { inflictBlowOffForceLevel_ = inflictBlowOffForceLevel; }


private:
    void LevelUpdate();

    //�n�`�����̍��W�擾
    DirectX::XMFLOAT3 CollidedPosition(const DirectX::XMFLOAT3 pos);

public:
    bool isSelectingSkill;//�\�͂̑I�����o�����̃t���O

    static constexpr float InitAcceleration = 10.0f;

    float actualRotValue;//��𒆁A���ۂɉ�]������Z�l

    bool isActiveAttackFrame;
private:
    //----------------------------�V�F�[�_�[----------------------------------
    Microsoft::WRL::ComPtr<ID3D11PixelShader> playerPS;
    //----------------------------------------------------------------------

    //----------------------------�J�����֌W----------------------------------
    float cameraRotSpeed = 2.0f;//���񑬓x
    //-----------------------------------------------------------------------

    //--------------------------�ړ�-----------------------------------------
    float maxEyeSpeed;
    //float maxDashSpeed = 4.0f;
    DirectX::XMFLOAT3 velocity{};

    //�J�����̌����Ă��������O�Ƃ����ړ������x�N�g��
    DirectX::XMFLOAT3 moveVec;

    float deceleration;
    float acceleration;
    float dodgeAcceleration = 30.0f;

    float rotTimer = 0;
    //-----------------------------------------------------------------------

    //--------------------------���x��-----------------------------------------
    int level;
    float curExp;//���݂̃��x���ɏオ���Ă���̌o���l
    float totalExp;//���܂ł̌o���l�̑���
    float levelUpExp;//���x�����グ��̂ɕK�v�Ȍo���l
    //-----------------------------------------------------------------------

    //---------------------------�X�e�[�g�}�V��-------------------------------
    std::unique_ptr<StateMachine<State<Player>>> stateMachine;
    //-----------------------------------------------------------------------


    //---------------------------�X�L��-------------------------------
    std::vector<BaseSkill*>* skillArray;

    BaseSkill* drawingSkillCards[3];
    int drawDirectionState;//�J�[�h�h���[���o�X�e�[�g
    float drawDirectionTime = 0.5f;//�J�[�h����ʏォ�痎���Ă��Ď~�܂�܂ł̎���
    float drawDirectionTimer = 0;//���o�Ɏg���p�̃^�C�}�[
    int selectCard = 0;

    //---------------------------�U���֌W-------------------------------
    float baseAttackPower;//��b�U���́i����Ƀ��[�V�����{�����|���Čv�Z����j

    float attackSpeed;//�U�����x

    //���ݍ��ݏ����̓C�[�W���O���g���čs��
    //���ݍ��ݒ��͎��@�̉�]���\�ɂ���
    const float steppingSpeed = 5.0f;//�U���ԍۂ̓��ݍ��ݍō����x
    const float steppingTime = 0.2f;//���ݍ��ݎ���
    float steppingTimer = 0;

public://getter���̂߂�ǂ�����
    float jabMotionAtkMuls[3];
    //-----------------------------------------------------------------------
private:
    //----------------------------���---------------------------------------
    float maxDodgeSpeed;//��𒆂̈ړ����x
    //-----------------------------------------------------------------------


    //-----------------------------��----------------------------------------
    std::unique_ptr<Model> swordModel;
    //-----------------------------------------------------------------------

    //--------------------------------��炢�A�U������------------------------------------
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

    float swordScale;//���̑傫���ɍ��킹�Ĕ���̑傫�����ς���

    float swordColliderRadius;//���̔���̑傫��
    int swordColliderNum;//����̐�

    void ColliderPosUpdate(const float& scale);//�e�W���C���g�ɔ�������鏈��

    //--------------------------------------------------------------------------------

    // �G�ɗ^���鐁����΂��͂̓x�����i���e�U���̊J�n���ɐݒ肵�Ă����j
    BLOW_OFF_FORCE_LEVEL inflictBlowOffForceLevel_ = BLOW_OFF_FORCE_LEVEL::NONE;


    // �A�r���e�B�}�l�[�W���[(��)
    AbilityManager abilityManager_ = {};


    //��������
    float lifeTimer;

    //---------------------------------Effect---------------------------------------------
    std::unique_ptr<Effect> laserEffect;
    //------------------------------------------------------------------------------------
};

