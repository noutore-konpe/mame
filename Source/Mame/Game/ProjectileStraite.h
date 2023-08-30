#pragma once

#include "Projectile.h"

class ProjectileStraite : public Projectile
{
public:
    ProjectileStraite(ProjectileManager* manager);
    ~ProjectileStraite() {}

    void Initialize()                                           override;   // ������
    void Finalize()                                             override {} // �I����
    void Begin()                                                override {} // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override;   // �X�V����
    void End()                                                  override {} // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime, const float& scale)   override;   // �`�揈��
    void DrawDebug()                                            override;   // ImGui�p

public:
    // ����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    DirectX::XMFLOAT3 direction;    // ����
    float speed = 1.0f;            // ���x
    float lifeTimer = 3.0f;         // ����
};