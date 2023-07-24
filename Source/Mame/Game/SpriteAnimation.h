#pragma once

#include <DirectXMath.h>
#include <memory>
#include <string>

#include "../Resource/sprite.h"

class SpriteAnimation
{
public:
    SpriteAnimation(const wchar_t* filename, const char* psFilename = nullptr);
    ~SpriteAnimation() {}

    void Initialize(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 texSize);

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
        const int   totalAnimationFrame,
        const bool  animationVertical
    );

    void Render();

    void DrawDebug();

    void SetPosition(DirectX::XMFLOAT2 p) { position = p; }
    void SetSize(DirectX::XMFLOAT2 s) { size = s; }
    void SetTexPos(DirectX::XMFLOAT2 texP) { texPos = texP; }
    void SetTexSize(DirectX::XMFLOAT2 texS) { texSize = texS; }
    void SetColor(DirectX::XMFLOAT4 c) { color = c; }
    void SetAngle(float a) { angle = a; }

    DirectX::XMFLOAT2 GetPosition() { return position; }
    DirectX::XMFLOAT2 GetSize() { return size; }
    DirectX::XMFLOAT2 GetTexPos() { return texPos; }
    DirectX::XMFLOAT2 GetTexSize() { return texSize; }
    DirectX::XMFLOAT4 GetColor() { return color; }
    float GetAngle() { return angle; }

private:
    DirectX::XMFLOAT2 position = {};
    DirectX::XMFLOAT2 size = {};
    DirectX::XMFLOAT2 texPos = {};
    DirectX::XMFLOAT2 texSize = {};
    DirectX::XMFLOAT4 color = { 1,1,1,1 };
    float angle = 0.0f;
    float animationTime  = 0.0f;
    float animationFrame = 0.0f;

    std::unique_ptr<Sprite> sprite = nullptr;

public:
    // Imgui名前被り防止のためのやつ //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui名前被り防止のためのやつ //
    
private:
    std::string name;   // Imgui用
};

