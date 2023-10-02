#pragma once

#include "../Graphics/Model.h"

class Character
{
public:
    Character();
    virtual ~Character() {}

    virtual void Initialize();
    virtual void Update(const float& elapsedTime);
    virtual void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);
    virtual void DrawDebug();

public: // �A�j���[�V�����֐��֘A
    // �A�j���[�V�����Đ��ݒ�
    // (�A�j���[�V�����ԍ��E���[�v���邩�ǂ����E�A�j���[�V�����Đ����x�E�X���[�Y�؂�ւ����ԁi���x�j)
    void PlayAnimation(
        const int& index,
        const bool& loop,
        const float& speed = 1.0f,
        const float& blendSeconds = 1.0f
    );

    // �u�����h���̌v�Z�X�V����
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // �A�j���[�V�����X�V����
    void UpdateAnimation(const float& elapsedTime);

    // �A�j���[�V�������Đ������ǂ���
    bool IsPlayAnimation() const;

public: // �擾�E�ݒ�@�֘A
    Transform* GetTransform() { return model->GetTransform(); }
    Transform* GetCollisionSqhereTransform() { return &collisionSqhereTransform; }

    const DirectX::XMFLOAT3 GetPosition() { return GetTransform()->GetPosition(); }
    void SetPosition(const DirectX::XMFLOAT3& pos) { GetTransform()->SetPosition(pos); }

    void SetDebugSqhereOffset(DirectX::XMFLOAT3 offset) { debugSqhereOffset = offset; }
    DirectX::XMFLOAT3 GetDebugSqhereOffset() { return debugSqhereOffset; }

    void SetRange(const float& r) { range = r; }
    float GetRange() { return range; }

    void Turn(float elapsedTime, float vx, float vz, float rotSpeed);


public:
    std::unique_ptr<Model> model = nullptr;

#ifdef _DEBUG
    std::unique_ptr<Model> debugSqhere = nullptr;   // �����蔻��p�Q��
#endif // _DEBUG

private:
    Transform collisionSqhereTransform{};
    DirectX::XMFLOAT3 debugSqhereOffset{};  // �����蔻��p
    float range = 1.0f;                     // �������蔻�蔼�a


public: // --- ImGui�p --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui�p --- //
    std::string name = {};
};

