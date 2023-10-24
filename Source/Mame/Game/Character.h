#pragma once

#include "../Graphics/Model.h"
#include "../Graphics/Effect.h"

// ������΂��͂̓x����
enum class BLOW_OFF_FORCE_LEVEL : int
{
    NONE,       // ����
    VERY_LOW,   // ���Ȃ���
    LOW,        // ���
    MIDDLE,     // �����x
    HIGH,       // ����
    VERY_HIGH,  // ���Ȃ苭��
    COUNT,      // �v�f��
};

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

public:
    struct SphereCollider//����p�̋���
    {
        SphereCollider(const float radius) : radius(radius) {}

        float radius;
        DirectX::XMFLOAT3 position;

        //��������邩���Ȃ��̃t���O�i�U�����肪��u�����~�������Ɏg�� ����̓S�[�����ɂ����g���j
        bool activeAttack;

        void DebugRender(const DirectX::XMFLOAT4 color = {1,1,1,1});
    };

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
    const bool IsPlayAnimation() const;

#pragma endregion

public: // �擾�E�ݒ�֐�
#pragma region [Get/Set]Function

    // �W���C���g�ʒu�擾
    DirectX::XMFLOAT3 GetJointPosition(const std::string& meshName, const std::string& boneName, const float& scale);
    DirectX::XMFLOAT3 GetJointPosition(size_t meshIndex, size_t boneIndex, const float& scale);

    Transform* GetTransform() { return model->GetTransform(); }

    const DirectX::XMFLOAT3& GetPosition() { return GetTransform()->GetPosition(); }
    void SetPosition(const DirectX::XMFLOAT3& pos) { GetTransform()->SetPosition(pos); }

    const char* const GetName() const { return name_.c_str(); }
    void SetName(const std::string& n) { name_ = n; }

    void SetRadius(const float radius) { radius_ = radius; }
    const float GetRadius() const { return radius_; }


    const float GetLockOnHeight() const { return lockOnHeight; }

    void SetHeight(const float height) { height_ = height; }
    const float GetHeight() const { return height_; }


    void SetHealth(const float hp)       { health = hp; }
    const float GetHealth() const { return health; }
    void SetMaxHealth(const float maxHp) { maxHealth = maxHp; }
    const float GetMaxHealth() const { return maxHealth; }

    // emissive ��constans�̂�Ȃ̂ł������g���ꏊ�� UpdateConstans�Ŏg���Ăق���
    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }

    const float GetDefense() const { return defence; }
    void AddDefense(const float defe) { defence += defe; }

    const bool GetIsDead() const { return isDead; }
    void SetIsDead(const bool dead) { isDead = dead; }

    std::vector<SphereCollider> GetHitCollider() { return hitCollider; }        //��炢����擾
    std::vector<SphereCollider> GetAttackCollider() { return attackCollider; }  //�U������擾

    [[nodiscard]] const size_t GetHitColliderCount() const { return hitCollider.size(); }
    [[nodiscard]] const size_t GetAttackColliderCount() const { return attackCollider.size(); }

    [[nodiscard]] const SphereCollider GetHitColliderAt(const size_t index) { return hitCollider.at(index); }
    [[nodiscard]] const SphereCollider GetAttackColliderAt(const size_t index) { return attackCollider.at(index); }

#pragma endregion

public: // ���̑��̊֐�
#pragma region OtherFunction

    void AddMaxHealth(const float hp) { maxHealth += hp; }

    void Turn(float elapsedTime, float vx, float vz, float rotSpeed);

    

    struct DamageResult
    {
        bool hit;
        float damage = 0;
        DirectX::XMFLOAT3 hitVector;
    };
    //�߂�l�͗^����ꂽ�_���[�W���A��{���G���Ԃ͋��݃��[�V�����ɍ��킹�邩�炱���͂O�ł���
    virtual DamageResult ApplyDamage(float damage,
        const DirectX::XMFLOAT3 hitPosition,
        Character* attacker = nullptr/*�U�����Ă����G*/, 
        float invincibleTime = 0,
        bool ignoreDefence = false/*�h�䖳��*/);

    bool ApplyHeal(float heal);

    //�_���[�W���󂯂��Ƃ��ɌĂ΂��
    virtual void OnDamaged() {}

    //���S�������ɌĂ΂��
    virtual void OnDead(DamageResult result) {}

    virtual void OnHealed() {}

    //�Ђ�܂���
    virtual void Flinch() {}


#pragma endregion

public:
    std::unique_ptr<Model> model;


//#ifdef _DEBUG
//    std::unique_ptr<Model> debugSqhere;   // �����蔻��p�Q��
//#endif // _DEBUG


    float rotValue;                 // ��]��

    bool isDead = false; //���S�t���O

    bool isInvincible = false;

protected:
    std::string name_           = "";           // ���O(ImGui�Ɏg�p)
    float       radius_         = 0.25f;        // ���a(�����蔻��Ɏg�p)
    float       height_         = 1.5f;         // ����(�ʒu�C���Ɏg�p)


    float       maxHealth       = 1.0f;
    float       health          = maxHealth;    // hp
    float       invincibleTime  = 1.0f;         // ���G����

    float       lockOnHeight    = 1.0f;         // �g��
    float       defence         = 0.0f;         // �h���


    //--------------------------------��炢�A�U������------------------------------------
    std::vector<SphereCollider> hitCollider;
    std::vector<SphereCollider> attackCollider;
    //--------------------------------------------------------------------------------

    //�G�t�F�N�g
    std::unique_ptr<Effect> hitEffect;
    std::unique_ptr<Effect> healEffect;

};

