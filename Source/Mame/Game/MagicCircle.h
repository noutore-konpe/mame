#pragma once
#include "Item.h"

#include "StateMachine.h"

class MagicCircle : public Item
{
public:
    MagicCircle();
    MagicCircle(const char* fbxFilename, const wchar_t* spriteFilename);
    ~MagicCircle() override;

    void Initialize()                       override; // ������
    void Finalize()                         override; // �I����
    void Begin()                            override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   override; // �X�V����
    void End()                              override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // �`�揈��
    void DrawDebug()                        override; // ImGui�p

    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> magicCirclePS;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;
};

