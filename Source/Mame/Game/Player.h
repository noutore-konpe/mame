#pragma once

#include "Character.h"
#include "StateMachine.h"
#include "../Input/Input.h"

#include "BaseSkill.h"
#include "AttackSkill.h"
#include "ParametricSkill.h"

class Player : public Character
{
public: // enum�֘A
    // �X�e�[�g
    enum class STATE
    {
        NORMAL,   // �ړ��A�ҋ@��
        ATTACK,   // �U��
        AVOID,   // ���
        DIE,   // ���S
    };

private: // enum�֘A
    // �A�j���[�V����
    enum class Animation
    {
    };

public:
    Player();
    ~Player() override;

    void Initialize() override;                     // ������
    void Finalize();                                // �I����

    void Begin();                                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End();                                     // ���t���[����ԍŌ�ɌĂ΂��

    void MoveUpdate(float elapsedTime);
    void UpdateVelocity(float elapsedTime);

    void CameraControllerUpdate(float elapsedTime);
    
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // �`�揈��
    
    void SkillImagesRender();

    void DrawDebug() override;  // ImGui�p

    static bool InputAttack()
    {
        return (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_X);
    }

    static bool InputDash()
    {
        return (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_RIGHT_SHOULDER);
    }

    //getter,setter
    void AddMaxSpeed(const float spd) { maxSpeed += spd; }

    void AddBaseAttackPower(const float power) { baseAttackPower += power; }

    const float GetAttackSpeed() const { return attackSpeed; }
    void SetAttackSpeed(const float spd) { attackSpeed = spd; }
    void AddAttackSpeed(const float spd) { attackSpeed += spd; }

    std::vector<BaseSkill*>& GetSkillArray() { return skillArray; }

private:
    void LevelUpdate();

public:
    bool isSelectingSkill;//�\�͂̑I�����o�����̃t���O

private:
    //----------------------------�J�����֌W----------------------------------
    float cameraRotSpeed = 2.0f;//���񑬓x
    //-----------------------------------------------------------------------

    //--------------------------�ړ�-----------------------------------------
    float maxSpeed = 4.0f;
    //float maxDashSpeed = 4.0f;
    DirectX::XMFLOAT3 velocity{};

    //�J�����̌����Ă��������O�Ƃ����ړ������x�N�g��
    DirectX::XMFLOAT3 moveVec;

    float deceleration = 5.0f;
    float acceleration = 10.0f;
    //-----------------------------------------------------------------------

    //--------------------------���x��-----------------------------------------
    int level;
    float curExp;//���݂̃��x���ɏオ���Ă���̌o���l
    float totalExp;//���܂ł̌o���l�̑���
    float levelUpExp;//���x�����グ��̂ɕK�v�Ȍo���l
    //-----------------------------------------------------------------------

    //---------------------------�X�e�[�g�}�V��-------------------------------
    StateMachine<State<Player>> stateMachine;
    //-----------------------------------------------------------------------

    
    //---------------------------�X�L��-------------------------------
    std::vector<BaseSkill*> skillArray;

    std::unique_ptr<PlayerSkill::Drain> drainSkill;
    std::unique_ptr<PlayerSkill::MoveSpeedUp> moveSpeedUpSkill;
    std::unique_ptr<PlayerSkill::AttackPowerUp> attackPowerUpSkill;
    std::unique_ptr<PlayerSkill::AttackSpeedUp> attackSpeedUpSkill;
    std::unique_ptr<PlayerSkill::BookIncrease> bookIncreaseSkill;

    //---------------------------�U���֌W-------------------------------
    float baseAttackPower;//��b�U���́i����Ƀ��[�V�����{�����|���Čv�Z����j

    float attackSpeed;//�U�����x
    //-----------------------------------------------------------------------
};

