#pragma once

#include "Character.h"
#include "StateMachine.h"

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

    void CameraControllerUpdate(float elapsedTime);
    
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // �`�揈��
    
    void DrawDebug() override;  // ImGui�p

private:
    //----------------------------�J�����֌W----------------------------------
    float cameraRotSpeed = 2.0f;//���񑬓x
    //-----------------------------------------------------------------------

    //--------------------------�ړ�-----------------------------------------
    float moveSpeed = 3.0f;
    DirectX::XMFLOAT3 velocity{};
    //-----------------------------------------------------------------------


    //---------------------------�X�e�[�g�}�V��-------------------------------
    StateMachine<State<Player>> stateMachine;
    //-----------------------------------------------------------------------
};

