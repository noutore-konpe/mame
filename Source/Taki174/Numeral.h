#pragma once

#include <vector>
#include "../Mame/Resource/sprite.h"

class Numeral
{
public:
    Numeral(
        const int numeral,
        const DirectX::XMFLOAT3& worldPos,
        const DirectX::XMFLOAT2& size,
        const DirectX::XMFLOAT4& color,
        const float
    );
    virtual ~Numeral();

    virtual void Initialize();
    virtual void Update(const float elapsedTime);
    virtual void Render();
    virtual void BloomRender(); //ブルーム効果付きの描画
    virtual void DrawDebug();

protected:
    void Destroy();

protected:
    std::vector<Sprite*> sprites_;
    std::string name_;
    int numeral_; // 表示する数字

};

