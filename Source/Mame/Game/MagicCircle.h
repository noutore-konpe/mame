#pragma once
#include "Item.h"

#include "StateMachine.h"

class MagicCircle : public Item
{
public:
    MagicCircle();
    ~MagicCircle() override;

    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Begin()                            override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override; // �X�V����
    void End()                              override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // �`�揈��
    void DrawDebug()                        override; // ImGui�p

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> magicCirclePS;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;
};

