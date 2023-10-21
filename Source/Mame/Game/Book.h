#pragma once

#include "../../Taki174/Common.h"
#include "Item.h"
#include "ProjectileManager.h"
#include "StateMachine.h"

class Book : public Item
{
public:
    enum class Animation
    {
        Closed,         // �������
        Opened,         // �J�������
        Open,           // �J��
        TurnThePage,    // �y�[�W���߂���
        Close,          // �܂�
    };

    enum class StateMachineState
    {
        Idle,   // �ҋ@
        Open,   // �J��
        Attack, // �U��
        Close,  // ����
    };

public:
    Book();
    ~Book() override;

    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Begin()                            override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override; // �X�V����
    void End()                              override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // �`�揈��
    void DrawDebug()                        override; // ImGui�p

    // �e�ۂƓG�Ƃ̏Փˏ���
    void CollisionProjectileVsEnemies();

public:
    // XZ��]
    void Turn(
        const float elapsedTime,
        const float vx,
        const float vz,
        const float turnSpeed /*Degree*/
    );

    // �e����&����
    bool LaunchProjectile(const float elapsedTime, const DirectX::XMFLOAT3& vec);

public:// �擾�E�ݒ� �֘A
    void SetTransform();

    // �X�e�[�g�}�V��
    StateMachine<State<Book>>* GetStateMachine() const { return stateMachine.get(); }

    // �e��
    void SetLaunchTimer(float time) { launchTimer = time; }
    int GetMaxLaunchNum() { return maxLaunchNum; }

    [[nodiscard]] const float GetSearchEnemyRange() const { return searchEnemyRange_; }

private:
    // ----- ����ɕK�v�ȏ�� -----
    static int totalNum;    // ���� "Book" ������������Ă邩�m�邽�߂̂���
    int num;                // �����������ڂ�
    // ----- ����ɕK�v�ȏ�� -----

    // �e��
    ProjectileManager projectileManager;
    float launchTimer = 0.0f;   // ���˗p�^�C�}�[
    float launchTime = 0.5f;    // ���˂܂ł̎���      // �� ( ��Ŕ\�͂Ƃ��Đݒ肳��� )
    int maxLaunchNum = 3;       // ���̔��˂̍ő吔  // ��

    // �X�e�[�g�}�V��
    std::unique_ptr<StateMachine<State<Book>>> stateMachine = nullptr;

    DirectX::XMFLOAT3 createPosition{}; // �����ʒu
    DirectX::XMFLOAT3 prevPosition{};   // �P�t���[���O�̈ʒu

    //bool isMoveToUp = true;             // ��Ɉړ����邩
    float circularMotionRotationZ_  = 0.0f;               // �~�^����]�lZ(���W�A��)
    float circularMotionRadius_     = 0.05f;              // �~�^�����a(�㉺����͈͂ɉe��)
    float circularMotionAddRotate_  = ::ToRadian(180.0f); // �~�^����]�l�ɒǉ������](�㉺���x�ɉe��)

    float searchEnemyRange_ = 15.0f;    // �G�����G�ł���͈�
};