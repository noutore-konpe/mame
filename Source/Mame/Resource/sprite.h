#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <string>

// spriteAnimationの使い方
// Initializeでpos,size,texSizeを指定する。
// PlayAnimation関数を呼び出すだけ

// spriteDissolveの使い方
// コンストラクタの第三引数にディゾルブ用のピクセルシェーダーをセットする
// ---> ("./Resources/Shader/sprite_dissolve_ps.cso")
// Update関数を呼び出す。
// Fadeの仕方↓
// 外からspriteDissolve.dissolveTypeの値をいじって(0:FadeIn,1:FadeOut)
// FadeをしたいときにisFadeをtrueにする(SetIsFade(true))

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
        int maskTextureValue = 0;           // テクスチャ番号
        float dissolveValue = 0.0f;         // ディゾルブ適応量
        float dissolveBlackValue = 0.0f;    // 黒色

        float edgeThreshold = 0.2f; // 縁の閾値
        DirectX::XMFLOAT4 edgeColor = { 0.0f, 1.0f, 0.0f, 1.0f }; // 縁の色

        float delay = 0.4f;
        int dissolveType = 0;

    public:
        void DrawDebug();

        void AddDissolveBlackValue(float value) { dissolveBlackValue += value; }
        void SubtractDissolveBlackValue(float value) { dissolveBlackValue -= value; }

        void SetMaskTextureValue(int value) { maskTextureValue = value; }
        void SetDissolveValue(float value) { dissolveValue = value; }
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
    /// アニメーション更新関数
    /// </summary>
    /// <param name="elapsedTime">フレーム経過時間</param>
    /// <param name="frameSpeed">アニメーションする速さ</param>
    /// <param name="totalAnimationFrame">アニメーションフレーム数</param>
    /// <param name="animationVertical">縦アニメーションの場合true</param>
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
    void Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color, float angle/*degree*/, DirectX::XMFLOAT2 texPos, DirectX::XMFLOAT2 texSize); // 本体

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
    //---ImGui名前かぶり防止用---//
    static int nameNum;
    const char* GetName() { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    //---ImGui名前かぶり防止用---//

private:
    //---ImGui名前かぶり防止用---//
    std::string name;
    //---ImGui名前かぶり防止用---//

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
        DirectX::XMFLOAT4 parameters = {};  // x: ディゾルブ適応量
                                            // y: 黒色
                                            // z: 縁の閾値
                                            // w: 空き
        DirectX::XMFLOAT4 edgeColor = {};   // 縁の色
    };                                      
};                                          
