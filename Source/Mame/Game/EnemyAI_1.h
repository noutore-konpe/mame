#pragma once

#include "BaseEnemyAI.h"

class EnemyAI_1 : public BaseEnemyAI
{
public:
    EnemyAI_1();
    ~EnemyAI_1() override;

    void Initialize()                                           override; // ������
    //void Finalize()                                             override; // �I����
    //void Begin()                                                override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override; // �X�V����
    //void End()                                                  override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // �`�揈��
    void DrawDebug()                                            override; // �f�o�b�O�`��

    void UpdateConstants() override;

private:
    static int nameNum_;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> emissiveTextureUVScroll;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;

};

