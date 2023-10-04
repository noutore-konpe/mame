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

    virtual void UpdateConstants() {}

public: // GPU_Instancing
    struct Instance
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 color;
    };

    std::vector<Instance> modelInstance;
    Microsoft::WRL::ComPtr<ID3D11Buffer> instanceBuffer;
    UINT maxInstanceCount = 0;

    void RenderInstancing(const Model& m, const std::vector<Instance>& instances);

public: // �A�j���[�V�����֐��֘A
    // �A�j���[�V�����Đ��ݒ�
    // (�A�j���[�V�����ԍ��E���[�v���邩�ǂ����E�A�j���[�V�����Đ����x�E�X���[�Y�؂�ւ����ԁi���x�j)
    void PlayAnimation(
        const int& index,
        const bool& loop,
        const float& speed = 1.0f,
        const float& blendSeconds = 1.0f
    );

    void PlayBlendAnimation(
        const int& index1,
        const int& index2,
        const bool& loop,
        const float& speed = 1.0f
    );
    
    // �u�����h���̌v�Z�X�V����
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // �A�j���[�V�����X�V����
    void UpdateAnimation(const float& elapsedTime);

    // �A�j���[�V�������Đ������ǂ���
    bool IsPlayAnimation() const;

    // �W���C���g�ʒu�擾
    DirectX::XMFLOAT3 GetJointPosition(const std::string& meshName, const std::string& boneName, const float& scale);
    DirectX::XMFLOAT3 GetJointPosition(size_t meshIndex, size_t boneIndex, const float& scale);

public: // �擾�E�ݒ�@�֘A
    Transform* GetTransform() { return model->GetTransform(); }
    Transform* GetCollisionSqhereTransform() { return &collisionSqhereTransform; }

    const DirectX::XMFLOAT3 GetPosition() { return GetTransform()->GetPosition(); }
    void SetPosition(const DirectX::XMFLOAT3& pos) { GetTransform()->SetPosition(pos); }

    void SetDebugSqhereOffset(DirectX::XMFLOAT3 offset) { debugSqhereOffset = offset; }
    DirectX::XMFLOAT3 GetDebugSqhereOffset() { return debugSqhereOffset; }

    void SetRange(const float& r) { range = r; }
    float GetRange() { return range; }


    // emissive ��constans�̂�Ȃ̂ł������g���ꏊ�� UpdateConstans�Ŏg���Ăق���
    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }


public: // �֐�
    void Turn(float elapsedTime, float vx, float vz, float rotSpeed);

    //�_���[�W���󂯂��Ƃ��ɌĂ΂��
    virtual void OnDamaged() {}

    //���S�������ɌĂ΂��
    virtual void OnDead() {}

    virtual void OnHealed() {}

    bool ApplyDamage(float damage, float invincibleTime);

    bool ApplyHeal(float heal);

public:
    std::unique_ptr<Model> model = nullptr;


    float maxHealth;
    float health;//hp
    float invincibleTime = 1.0f;//���G����

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

