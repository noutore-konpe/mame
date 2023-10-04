#pragma once
#include "Item.h"

#include "StateMachine.h"

class MagicCircle
{
public:
    MagicCircle();
    MagicCircle(const char* fbxFilename, const wchar_t* spriteFilename);
    ~MagicCircle();

    void Initialize()                       ; // ������
    void Finalize()                         ; // �I����
    void Begin()                            ; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)   ; // �X�V����
    void End()                              ; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) ; // �`�揈��
    void DrawDebug()                        ; // ImGui�p

    void SetEmissiveIntensity(float intensity) { model->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model->skinned_meshes->data.emissiveColor = color; }

public: // �擾�E�ݒ�@�֘A
    Transform* GetTransform() { return model->GetTransform(); }

public:
    std::unique_ptr<Model> model = nullptr;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> magicCirclePS;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;

public: // --- ImGui�p --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui�p --- //
    std::string name = {};
};

