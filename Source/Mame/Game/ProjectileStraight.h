#pragma once

#include "Projectile.h"

class ProjectileStraight : public Projectile
{
public:
    ProjectileStraight(ProjectileManager* manager);
    ~ProjectileStraight();

    void Initialize()                                           override;   // ������
    void Finalize()                                             override {} // �I����
    void Begin()                                                override {} // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override;   // �X�V����
    void End()                                                  override {} // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override;   // �`�揈��
    void DrawDebug()                                            override;   // ImGui�p

public:
    // ����
    void Launch(
        const DirectX::XMFLOAT3& direction,
        const DirectX::XMFLOAT3& position
    );

private:
    static int nameNum_;

private:
    DirectX::XMFLOAT3 direction_    = {};   // ����
    float speed_                    = 2.0f; // ���x
    float lifeTimer_                = 3.0f; // ����

};