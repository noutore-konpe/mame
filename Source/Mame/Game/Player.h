#pragma once

#include "Character.h"
#include "StateMachine.h"
#include "../Input/Input.h"

#include "BaseSkill.h"
#include "AttackSkill.h"
#include "ParametricSkill.h"

#include "AbilityManager.h"

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
        //Jab_3,
        Avoid,
        Counter,
        CounterAttack,
        SoftStagger
    };


public:
    Player();
    ~Player() override;

    void Initialize() override;                     // ������
    void Finalize();                                // �I����

    void Begin();                                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float elapsedTime) override; // �X�V����
    void End();                                     // ���t���[����ԍŌ�ɌĂ΂��

    void MoveUpdate(float elapsedTime, float ax, float ay);
    void UpdateVelocity(float elapsedTime, float ax, float ay);

    //���͂��J�������猩���x�N�g���ɕύX��moveVec�ϐ��ɓ����֐�
    void MoveVecUpdate(float ax, float ay);

    void AvoidUpdate(float elapsedTime);

    void ModelRotZUpdate(float elapsedTime);

    void CameraControllerUpdate(float elapsedTime);

    void Render(const float scale, ID3D11PixelShader* psShader = nullptr) override; // �`�揈��

    void SkillImagesRender();

    void DrawDebug() override;  // ImGui�p

    void PlayAnimation(Animation index, bool loop, float speed = 1.0f) { Character::PlayAnimation(static_cast<int>(index), loop, speed); }

    //�ėp�֐�
    void PlayWalkAnimation() { PlayBlendAnimation(Idle, Dash, true); }
    void PlaySwordWalkAnimation() { swordModel->PlayBlendAnimation(Idle, Dash, true); }

    void AttackSteppingUpdate(float elapsedTime);//�U���ԍۂ̓��ݍ��ݏ���

    void OnDamaged()override;
    void OnDead()override;

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

    //getter,setter
    void AddMaxSpeed(const float spd) { maxEyeSpeed += spd; }

    void AddBaseAttackPower(const float power) { baseAttackPower += power; }

    const float GetAttackSpeed() const { return attackSpeed; }
    void SetAttackSpeed(const float spd) { attackSpeed = spd; }
    void AddAttackSpeed(const float spd) { attackSpeed += spd; }

    std::vector<BaseSkill*>& GetSkillArray() { return skillArray; }

    void SetAcceleration(const float accel) { acceleration = accel; }

    StateMachine<State<Player>>* GetStateMachine() { return stateMachine.get(); }

    void SetVelocity(const DirectX::XMFLOAT3 velo) { velocity = velo; }

    Model* GetSword() { return swordModel.get(); }

private:
    void LevelUpdate();
    
    //�n�`�����̍��W�擾
    DirectX::XMFLOAT3 CollidedPosition(const DirectX::XMFLOAT3 pos);

public:
    bool isSelectingSkill;//�\�͂̑I�����o�����̃t���O

    static constexpr float InitAcceleration = 10.0f;

    float actualRotValue;//��𒆁A���ۂɉ�]������Z�l
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
    std::vector<BaseSkill*> skillArray;

    std::unique_ptr<PlayerSkill::Drain> drainSkill;
    std::unique_ptr<PlayerSkill::MoveSpeedUp> moveSpeedUpSkill;
    std::unique_ptr<PlayerSkill::AttackPowerUp> attackPowerUpSkill;
    std::unique_ptr<PlayerSkill::AttackSpeedUp> attackSpeedUpSkill;
    std::unique_ptr<PlayerSkill::BookIncrease> bookIncreaseSkill;
    std::unique_ptr<PlayerSkill::MaxHitPointUp> maxHitPointUpSkill;
    std::unique_ptr<PlayerSkill::DefenseUp> defenseUpSkill;

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
    //-----------------------------------------------------------------------

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
        END
    };

    float swordScale;//���̑傫���ɍ��킹�Ĕ���̑傫�����ς���
    
    float swordColliderRadius;//���̔���̑傫��
    int swordColliderNum = 5;//����̐�

    void ColliderPosUpdate(const float& scale);//�e�W���C���g�ɔ�������鏈��

    //--------------------------------------------------------------------------------

    // �A�r���e�B�}�l�[�W���[(��)
    AbilityManager abilityManager_ = {};


    bool showCollider = true;

};

