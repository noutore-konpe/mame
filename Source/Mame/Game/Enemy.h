#pragma once

#include "../../Taki174/OperatorXMFloat3.h"
#include "Character.h"

class Enemy : public Character
{
public:
    enum class TYPE
    {
        Normal,
        Golem,
    };

public:
    Enemy() {}
    virtual ~Enemy() {}

    virtual void Initialize();                                          // ������
    virtual void Finalize() = 0;                                        // �I����
    virtual void Begin()    = 0;                                           // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime);                      // �X�V����
    virtual void End()      = 0;                                             // ���t���[����ԍŌ�ɌĂ΂��
    //virtual void Render(const float& elapsedTime, const float& scale);  // �`�揈��
    virtual void Render(const float& scale, ID3D11PixelShader* psShader);  // �`�揈��
    virtual void DrawDebug() = 0;                                       // �f�o�b�O�`��

    virtual void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr) {}

    virtual int GetCurrentState() { return 0; }

    //virtual void UpdateConstants() = 0;

public:
    int GetType() { return type; }
    void SetType(int t) { type = t; }

    const DirectX::XMFLOAT3& GetVelocity() const { return velocity_; }
    void SetVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ = velocity; }
    void AddVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ += velocity; }

    const bool GetEntryStageFlag() const { return entryStageFlag_; }
    void SetEntryStageFlag(const bool entryStageFlag) { entryStageFlag_ = entryStageFlag; }

protected:
    DirectX::XMFLOAT3   velocity_       = {};
    int                 dropExpCount_   = 5;        // �h���b�v����o���l�̐�
    bool                entryStageFlag_ = false;    // �X�e�[�W�ɓ��������ǂ����̃t���O

private:
    int type = 0;

};

