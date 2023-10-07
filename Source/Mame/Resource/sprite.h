#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <string>

// spriteAnimation�̎g����
// Initialize��pos,size,texSize���w�肷��B
// PlayAnimation�֐����Ăяo������

// spriteDissolve�̎g����
// �R���X�g���N�^�̑�O�����Ƀf�B�]���u�p�̃s�N�Z���V�F�[�_�[���Z�b�g����
// ---> ("./Resources/Shader/sprite_dissolve_ps.cso")
// Update�֐����Ăяo���B
// Fade�̎d����
// �O����spriteDissolve.dissolveType�̒l����������(0:FadeIn,1:FadeOut)
// Fade���������Ƃ���isFade��true�ɂ���(SetIsFade(true))

class Sprite
{
public:
    Sprite(ID3D11Device* device, const wchar_t* filename, const char* psFilename = nullptr);
    ~Sprite();

public:
    struct SpriteTransform
    {
    private:
        DirectX::XMFLOAT2 pos = {};
        DirectX::XMFLOAT2 size = { 100,100 };
        DirectX::XMFLOAT4 color = { 1,1,1,1 };
        float angle = 0.0f;
        DirectX::XMFLOAT2 texPos = {};
        DirectX::XMFLOAT2 texSize = { 100,100 };

    public:
        void DrawDebug();

        void SetPos(DirectX::XMFLOAT2 p) { pos = p; }
        void SetSize(DirectX::XMFLOAT2 s) { size = s; }
        void SetColor(DirectX::XMFLOAT4 c) { color = c; }
        void SetAngle(float a) { angle = a; }
        void SetTexPos(DirectX::XMFLOAT2 texP) { texPos = texP; }
        void SetTexSize(DirectX::XMFLOAT2 texS) { texSize = texS; }

        DirectX::XMFLOAT2 GetPos() { return pos; }
        DirectX::XMFLOAT2 GetSize() { return size; }
        DirectX::XMFLOAT4 GetColor() { return color; }
        float GetAngle() { return angle; }
        DirectX::XMFLOAT2 GetTexPos() { return texPos; }
        DirectX::XMFLOAT2 GetTexSize() { return texSize; }
    };

    struct SpriteDissolve
    {
    private:
        int maskTextureValue = 0;           // �e�N�X�`���ԍ�
        float dissolveValue = 0.0f;         // �f�B�]���u�K����
        float dissolveBlackValue = 0.0f;    // ���F

        float edgeThreshold = 0.2f; // ����臒l
        DirectX::XMFLOAT4 edgeColor = { 0.0f, 1.0f, 0.0f, 1.0f }; // ���̐F

        float delay = 0.4f;
        int dissolveType = 0;

    public:
        void DrawDebug();

        void AddDissolveBlackValue(float value) { dissolveBlackValue += value; }
        void SubtractDissolveBlackValue(float value) { dissolveBlackValue -= value; }

        void SetMaskTextureValue(int value) { maskTextureValue = value; }
        void SetDissolveValue(float value) { dissolveValue = value; }
        void AddDissolveValue(float value) { dissolveValue += value; }
        void SetDissolveBlackValue(float value) { dissolveBlackValue = value; }
        void SetEdgeThreshold(float threshold) { edgeThreshold = threshold; }
        void SetEdgeColor(DirectX::XMFLOAT4 color) { edgeColor = color; }
        void SetDelay(float d) { delay = d; }
        void SetDissolveType(int type) { dissolveType = type; }

        int GetMaskTextureValue() { return maskTextureValue; }
        float GetDissolveValue() { return dissolveValue; }
        float GetDissolveBlackValue() { return dissolveBlackValue; }
        float GetEdgeThreshold() { return edgeThreshold; }
        DirectX::XMFLOAT4 GetEdgeColor() { return edgeColor; }
        float GetDelay() { return delay; }
        int GetDissolveType() { return dissolveType; }
    };
public:
    void Initialize();
    void Initialize(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size,
        DirectX::XMFLOAT2 texSize, DirectX::XMFLOAT2 texPos = { 0,0 },
        DirectX::XMFLOAT4 color = { 1,1,1,1 }, float angle = 0);

    void Update(const float& elapsedTime);

    void UpdateSpriteDissolve(const float& elapsedTime);
    bool FadeIn(const float& elapsedTime);
    bool FadeOut(const float& elapsedTime);

    /// <summary>
    /// �A�j���[�V�����X�V�֐�
    /// </summary>
    /// <param name="elapsedTime">�t���[���o�ߎ���</param>
    /// <param name="frameSpeed">�A�j���[�V�������鑬��</param>
    /// <param name="totalAnimationFrame">�A�j���[�V�����t���[����</param>
    /// <param name="animationVertical">�c�A�j���[�V�����̏ꍇtrue</param>
    void PlayAnimation(
        const float elapsedTime,
        const float frameSpeed,
        const float totalAnimationFrame,
        const bool animationVertical
    );

    void Render();
    void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size);
    void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color);
    void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color, float angle/*degree*/);    
    void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color, float angle/*degree*/, DirectX::XMFLOAT2 texPos, DirectX::XMFLOAT2 texSize); // �{��

    void DrawDebug();

    SpriteTransform* GetSpriteTransform() { return &spriteTransform; }
    SpriteDissolve* GetSpriteDissolve() { return &spriteDissolve; }

    void SetIsFade(bool fade) { isFade = fade; }
    bool GetIsFade() { return isFade; }

private:
    SpriteTransform spriteTransform;
    SpriteDissolve spriteDissolve;

    // Animation
    float animationTime = 0.0f;
    float animationFrame = 0.0f;

    // Dissolve
    bool isDissolve = false;
    bool isFade = false;

public:
    //---ImGui���O���Ԃ�h�~�p---//
    static int nameNum;
    const char* GetName() { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    //---ImGui���O���Ԃ�h�~�p---//

private:
    //---ImGui���O���Ԃ�h�~�p---//
    std::string name;
    //---ImGui���O���Ԃ�h�~�p---//

public:
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcord;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer() { return vertexBuffer; }
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return shaderResourceView; }

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    D3D11_TEXTURE2D_DESC texture2dDesc;

    Microsoft::WRL::ComPtr<ID3D11Buffer> dissolveConstantBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> maskTexture[20] = {};
    D3D11_TEXTURE2D_DESC maskTexture2dDesc[20] = {};

    struct DissolveConstants
    {
        DirectX::XMFLOAT4 parameters = {};  // x: �f�B�]���u�K����
                                            // y: ���F
                                            // z: ����臒l
                                            // w: ��
        DirectX::XMFLOAT4 edgeColor = {};   // ���̐F
    };                                      
};                                          
