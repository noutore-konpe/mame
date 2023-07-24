#pragma once

#include "../Resource/sprite.h"
#include <memory>

enum class Dissolve
{
    Fire,
    Fade,
};

class SpriteDissolve
{
public:
    SpriteDissolve();
    SpriteDissolve(const wchar_t* filename);
    ~SpriteDissolve() {}

    void Initialize();
    //void Initialize(int state, const wchar_t* filename);
    
    void Update();
    void Render();
    void DrawDebug();

    // load用
    void FadeIn(float elapsedTime);
    void FadeOut(float elapsedTime);
    void SetFadeInTexture(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 textureSize, float delay, int textureType)
    {
        spr_dissolve.pos                = position;
        spr_dissolve.posD               = textureSize;
        spr_dissolve.delay              = delay;
        spr_dissolve.mask_texture_value = textureType;
        spr_dissolve.dissolve_value1    = 1.5f;
    }
    void SetFadeOutTexture(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 textureSize, float delay, int textureType)
    {
        spr_dissolve.pos                = position;
        spr_dissolve.posD               = textureSize;
        spr_dissolve.delay              = delay;
        spr_dissolve.mask_texture_value = textureType;
        spr_dissolve.dissolve_value1 = 0.0f;
    }
    bool FadeInReady(float time) { return spr_dissolve.dissolve_value1 < time; }
    bool FadeOutReady(float time) { return spr_dissolve.dissolve_value1 > time; }
    bool IsFade() { return isFade; } // fadeするのかの判断に使う
    void SetFade(bool fade) { isFade = fade; }

    void SetPosition(DirectX::XMFLOAT3 pos) { spr_dissolve.pos = pos; }
    DirectX::XMFLOAT3 GetPosition() { return spr_dissolve.pos; }
    void SetSize(DirectX::XMFLOAT2 size) { spr_dissolve.posD = size; }
    DirectX::XMFLOAT2 GetSize() { return spr_dissolve.posD; }

public:
    // デバッグ用
    bool debug_initialize = false;

    int DS = 0;
    
private:
    struct dissolve_constants
    {
        DirectX::XMFLOAT4 parameters = {};  // x:ディゾルブ適応量、
                                            // y: 黒色
                                            // z: 縁の閾値
                                            // w:空き
        DirectX::XMFLOAT4 edgeColor  = {};  // 縁の色
    };

    struct SprDissolve
    {
        DirectX::XMFLOAT3 pos = { 600.0f, 420.0f ,0.0f };
        DirectX::XMFLOAT2 posD      = { 300.0f, 300.0f };

        int      mask_texture_value = 0;
        float    dissolve_value     = 0.0f;
        float    dissolve_value1    = 0.0f;

        // mask edge
        float    edge_threshold     = 0.2f;        // 縁の閾値
        DirectX::XMFLOAT4 edgeColor = { 0,1,0,1 }; // 縁の色

        int      dissolve_state     = 0;           // dissolveの種類
        float    delay              = 0.4f;        // fadeのいい感じにするための変数

    } spr_dissolve = {};

private:
    std::unique_ptr<Sprite> dissolve_sprite  = nullptr;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState = nullptr;

    D3D11_TEXTURE2D_DESC mask_texture2d_desc = {};
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mask_texture[20] = {};

    Microsoft::WRL::ComPtr<ID3D11Buffer> dissolve_constant_buffer = nullptr;

    bool isFade = false;
};

