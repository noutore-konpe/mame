#pragma once

#include "Enemy.h"

class EnemyAura : public Enemy
{
public:
    EnemyAura();
    ~EnemyAura() override;

    void Initialize()                                           override; // ������
    void Finalize()                                             override; // �I����
    void Begin()                                                override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override; // �X�V����
    void End()                                                  override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // �`�揈��
    void DrawDebug()                                            override; // �f�o�b�O�`��

    void UpdateConstants() override;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> emissiveTextureUVScroll;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;

};

