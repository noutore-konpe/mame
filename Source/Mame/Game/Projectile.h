#pragma once

#include "../Graphics/Model.h"
#include "Character.h"

// �O���錾
class ProjectileManager;

class Projectile
{
public:
    Projectile(ProjectileManager* manager);
    virtual ~Projectile() {}

    virtual void Initialize();                      // ������
    virtual void Finalize();                        // �I����
    virtual void Begin();                           // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime);  // �X�V����
    virtual void End();                             // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);  // �`�揈��
    virtual void DrawDebug();                       // ImGui�p

public:
    void Destroy(); // �j��

public: // �擾�E�ݒ�@�֘A
    Transform* GetTransform() { return model_->GetTransform(); }

    const Character* GetParent() const { return parent_; }
    void SetParent(Character* parent) { parent_ = parent; }

    const float GetRadius() const { return radius_; }   // ���a�擾
    void SetRadius(float r) { radius_ = r; }            // ���a�ݒ�

    // --- ImGui�p --- //
    const char* GetName() const { return name_.c_str(); }
    void SetName(std::string n) { name_ = n; }

public:
    std::unique_ptr<Model> model_ = nullptr;

private:
    ProjectileManager* manager_ = nullptr;

    Character* parent_ = nullptr;  // �����𐶐������L�����N�^�[�̃|�C���^

    float radius_ = 0.5f;

    // --- ImGui�p --- //
    std::string name_ = {};

};

