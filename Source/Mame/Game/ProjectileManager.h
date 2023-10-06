#pragma once

#include <vector>
#include <set>

#include "Projectile.h"

class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    void Initialize();                                          // ������
    void Finalize();                                            // �I����
    void Begin();                                               // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float elapsedTime);                      // �X�V����
    void End();                                                 // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr);  // �`�揈��
    void DrawDebug();                                           // ImGui�p

public:
    void Register(Projectile* projectile);  // �o�^
    void Remove(Projectile* projectile);    // �폜
    void Clear();                           // �S�폜

public: // �擾�E�ݒ�@�֘A
    const size_t GetProjectileCount() const { return projectiles_.size(); } // �e�ې��擾
    Projectile* GetProjectile(const size_t index) { return projectiles_.at(index); } // �e�ێ擾

private:
    std::vector<Projectile*>  projectiles_ = {};
    std::set<Projectile*>     removes_     = {};
};

