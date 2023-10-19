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

public:
    struct SphereCollider//判定用の球体
    {
        SphereCollider(const float radius) : radius(radius) {}

        float radius;
        DirectX::XMFLOAT3 position;

        //判定をするかしないのフラグ（攻撃判定が一瞬だけ欲しい時に使う 今回はゴーレムにしか使わん）
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

public: // アニメーション関数
#pragma region AnimationFunction

    // アニメーション再生設定
    // (アニメーション番号・ループするかどうか・アニメーション再生速度・スムーズ切り替え時間（速度）)
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

    // ブレンド率の計算更新処理
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // アニメーション更新処理
    void UpdateAnimation(const float& elapsedTime);

    // アニメーションが再生中かどうか
    const bool IsPlayAnimation() const;

#pragma endregion

public: // 取得・設定関数
#pragma region [Get/Set]Function

    // ジョイント位置取得
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


    // emissive ※constansのやつなのでこいつを使う場所は UpdateConstansで使ってほしい
    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }

    const float GetDefense() const { return defence; }
    void AddDefense(const float defe) { defence += defe; }

    std::vector<SphereCollider> GetHitCollider() { return hitCollider; }//喰らい判定取得
    std::vector<SphereCollider> GetAttackCollider() { return attackCollider; }//攻撃判定取得

#pragma endregion

public: // その他の関数
#pragma region OtherFunction

    void AddMaxHealth(const float hp) { maxHealth += hp; }

    void Turn(float elapsedTime, float vx, float vz, float rotSpeed);

    //ダメージを受けたときに呼ばれる
    virtual void OnDamaged() {}

    //死亡した時に呼ばれる
    virtual void OnDead() {}

    virtual void OnHealed() {}

    struct DamageResult
    {
        bool hit;
        float damage = 0;
    };
    //戻り値は与えられたダメージ数、基本無敵時間は怯みモーションに合わせるからここは０でいい
    DamageResult ApplyDamage(float damage, float invincibleTime = 0);

    bool ApplyHeal(float heal);

#pragma endregion

public:
    std::unique_ptr<Model> model;


//#ifdef _DEBUG
//    std::unique_ptr<Model> debugSqhere;   // 当たり判定用＿球
//#endif // _DEBUG

    
    float rotValue;                 // 回転量

    bool isDead = false; //死亡フラグ

    bool isInvincible = false;

protected:
    std::string name_           = "";       // 名前(ImGuiに使用)
    float       radius_         = 0.25f;    // 半径(当たり判定に使用)
    float       height_         = 1.5f;     // 高さ(位置修正に使用)


    float maxHealth;
    float health;                   // hp
    float invincibleTime = 1.0f;    // 無敵時間

    float lockOnHeight = 1.0f;//身長
    float       defence         = 0.0f;     // 防御力

    //--------------------------------喰らい、攻撃判定------------------------------------
    std::vector<SphereCollider> hitCollider;
    std::vector<SphereCollider> attackCollider;
    //--------------------------------------------------------------------------------
};

