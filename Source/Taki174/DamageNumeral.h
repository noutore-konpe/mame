#pragma once

#include "Numeral.h"

class DamageNumeral : public Numeral
{
public:
    DamageNumeral(
        const int numeral,
        const DirectX::XMFLOAT3& worldPos,
        const DirectX::XMFLOAT2& size = { 1,1 },
        const DirectX::XMFLOAT4& color = { 1,1,1,1 },
        const float angle = 0.0f
    );
    ~DamageNumeral() override;

    void Initialize()  override;
    void Update(const float elapsedTime) override;
    void Render()      override;
    void BloomRender() override;    //ƒuƒ‹[ƒ€Œø‰Ê•t‚«‚Ì•`‰æ
    void DrawDebug()   override;

private:
    static constexpr float LIFE_TIME_ = 2.0f;

private:
    static unsigned int nameNum_;

private:
    float lifeTimer_ = LIFE_TIME_;

};

