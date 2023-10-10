#pragma once

#include "../Graphics/Model.h"

class Character
{
public:
    Character();
    virtual ~Character() {}

    virtual void Initialize();
    virtual void Update(const float elapsedTime);
    virtual void Render(const float scale, ID3D11PixelShader* psShader = nullptr);
    virtual void DrawDebug();

    virtual void UpdateConstants() {}

public: // GPU_Instancing
#pragma region GPU_Instancing

    struct Instance
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 color;
    };

    std::vector<Instance> modelInstance;
    Microsoft::WRL::ComPtr<ID3D11Buffer> instanceBuffer;
    UINT maxInstanceCount = 0;

    void RenderInstancing(const Model& m, const std::vector<Instance>& instances);

#pragma endregion

public: // �A�j���[�V�����֐�
#pragma region AnimationFunction

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

#pragma endregion

public: // �擾�E�ݒ�֐�
#pragma region [Get/Set]Function

    // �W���C���g�ʒu�擾
    DirectX::XMFLOAT3 GetJointPosition(const std::string& meshName, const std::string& boneName, const float& scale);
    DirectX::XMFLOAT3 GetJointPosition(size_t meshIndex, size_t boneIndex, const float& scale);

    Transform* GetTransform() { return model->GetTransform(); }
    //Transform* GetCollisionSphereTransform() { return &collisionSphereTransform; }

    const DirectX::XMFLOAT3& GetPosition() { return GetTransform()->GetPosition(); }
    void SetPosition(const DirectX::XMFLOAT3& pos) { GetTransform()->SetPosition(pos); }

    //void SetDebugSphereOffset(DirectX::XMFLOAT3 offset) { debugSphereOffset = offset; }
    //DirectX::XMFLOAT3 GetDebugSphereOffset() { return debugSphereOffset; }

    const char* const GetName() const { return name_.c_str(); }
    void SetName(const std::string& n) { name_ = n; }

    //// ��radius�ɏ�芷���\��
    //void SetRange(const float r) { range = r; }
    //float GetRange() { return range; }

    void SetRadius(const float radius) { radius_ = radius; }
    const float GetRadius() const { return radius_; }

    // emissive ��constans�̂�Ȃ̂ł������g���ꏊ�� UpdateConstans�Ŏg���Ăق���
    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }

#pragma endregion

public: // ���̑��̊֐�
#pragma region OtherFunction

    void AddMaxHealth(const float hp) { maxHealth += hp; }

    void Turn(float elapsedTime, float vx, float vz, float rotSpeed);

    //�_���[�W���󂯂��Ƃ��ɌĂ΂��
    virtual void OnDamaged() {}

    //���S�������ɌĂ΂��
    virtual void OnDead() {}

    virtual void OnHealed() {}

    bool ApplyDamage(float damage, float invincibleTime);

    bool ApplyHeal(float heal);

#pragma endregion

public:
    std::unique_ptr<Model> model;

//#ifdef _DEBUG
//    std::unique_ptr<Model> debugSqhere;   // �����蔻��p�Q��
//#endif // _DEBUG

    float maxHealth;
    float health;                   // hp
    float invincibleTime = 1.0f;    // ���G����
    float rotValue;                 // ��]��

private:
    //Transform collisionSphereTransform{};
    //DirectX::XMFLOAT3 debugSphereOffset{};  // �����蔻��p
    //float       range = 1.0f;               // �������蔻�蔼�a(��radius�ɏ�芷���\��)

    std::string name_    = "";
    float       radius_ = 0.25f;

};

