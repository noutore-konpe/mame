#pragma once

#include "../Graphics/Model.h"

// �O���錾
class ProjectileManager;

class Projectile
{
public:
    Projectile(ProjectileManager* manager);
    virtual ~Projectile() {}

    virtual void Initialize();                                          // ������
    virtual void Finalize();                                            // �I����
    virtual void Begin();                                               // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime);                      // �X�V����
    virtual void End();                                                 // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float& elapsedTime, const float& scale);  // �`�揈��
    virtual void DrawDebug();                                           // ImGui�p

public:
    void Destroy(); // �j��

public: // �擾�E�ݒ�@�֘A
    Transform* GetTransform() { return model->GetTransform(); }

    float GetRadius()const { return radius; }   // ���a�擾
    void SetRadius(float r) { radius = r; }     // ���a�ݒ�

public:
    std::unique_ptr<Model> model = nullptr;

private:
    ProjectileManager* manager = nullptr;

    float radius = 0.5f;

public: // --- ImGui�p --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui�p --- //
    std::string name = {};
};

