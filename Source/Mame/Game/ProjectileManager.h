#pragma once

#include "Projectile.h"
#include <vector>
#include <set>

class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    void Initialize();                                          // ������
    void Finalize();                                            // �I����
    void Begin();                                               // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);                      // �X�V����
    void End();                                                 // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime, const float& scale);  // �`�揈��
    void DrawDebug();                                           // ImGui�p

public:
    void Register(Projectile* projectile);  // �o�^
    void Remove(Projectile* projectile);    // �폜
    void Clear();                           // �S�폜

public: // �擾�E�ݒ�@�֘A
    const int GetProjectileCount() const { return static_cast<int>(projectiles.size()); } // �e�ې��擾

    Projectile* GetProjectile(int index) { return projectiles.at(index); } // �e�ێ擾

private:
    std::vector<Projectile*>  projectiles = {};
    std::set<Projectile*>     removes = {};
};

