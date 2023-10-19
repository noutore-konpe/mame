#pragma once

#include <vector>
#include "../Mame/Resource/sprite.h"

class Character;

class Numeral
{
public:
    Numeral(
        const int numeral,
        const DirectX::XMFLOAT3& worldPos,
        const DirectX::XMFLOAT2& size,
        const DirectX::XMFLOAT4& color,
        const float angle,
        const float rowOffset // 行間隔の調整値
    );
    virtual ~Numeral();

    virtual void Initialize();
    virtual void Update(const float elapsedTime);
    virtual void Render();
    virtual void BloomRender(); // ブルーム効果付きの描画
    virtual void DrawDebug();

public:
    const Character* GetParent() { return parent_; }
    void SetParent(Character* parent) { parent_ = parent; }

protected:
    void Destroy();

protected:
    static constexpr DirectX::XMFLOAT2 TEX_SIZE_ = { 60.0f, 100.0f };

protected:
    Character*           parent_ = nullptr;

protected:
    std::vector<Sprite*> sprites_;
    std::string          name_;
    float                rowOffset_;
    int                  numeral_;          // 表示する数字
    bool                 isDraw_ = false;   // 描画するか(スクリーン座標変換関数に入れて使う)

};

