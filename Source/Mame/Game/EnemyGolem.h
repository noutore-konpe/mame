#pragma once

#include "Enemy.h"

#include "MagicCircleGolem.h"
#include "MagicCircleEnemySummon.h"

#include "StateMachine.h"

class EnemyGolem : public Enemy
{
public: // �萔
    enum class StateMachineState
    {
        IdleState,      // �ҋ@
        EntryState,     // �o��
        RoarState,      // ���K
        SummonState,    // ����
    };

    const DirectX::XMFLOAT4 magicCircleColor[10] =
    {
        { 0.80f, 0.44f, 0.24f, 1.0f },
        { 0.54f, 0.27f, 0.07f, 1.0f },   // saddleBrown
        { 0.44f, 0.36f, 0.12f, 1.0f },   // ��
        { 0.34f, 0.21f, 0.13f, 1.0f },   // ����
        { 0.44f, 0.36f, 0.12f, 1.0f },   // ��
    };

public:
    EnemyGolem();
    ~EnemyGolem() override;

    void Initialize()                                           override; // ������
    void Finalize()                                             override; // �I����
    void Begin()                                                override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override; // �X�V����
    void End()                                                  override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // �`�揈��
    void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr);
    void DrawDebug()                                            override; // �f�o�b�O�`��

    void UpdateConstants() override;

public:// �擾�E�ݒ�
    // �X�e�[�g�}�V��
    StateMachine<State<EnemyGolem>>* GetStateMachine() const { return stateMachine.get(); }


public:
    // �������@�w�X�V����
    void UpdateSummoningMagicCircle(const float& lengthX, const float& lengthZ, const float& angle);

public:
    std::unique_ptr<MagicCircleGolem> magicCircleGolem;
    std::unique_ptr<MagicCircleEnemySummon> magicCircleEnemySummon;

private:
    // �X�e�[�g�}�V��
    std::unique_ptr<StateMachine<State<EnemyGolem>>> stateMachine = nullptr;
};

