#pragma once

#include "Numeral.h"

class Character;

class DamageNumeral : public Numeral
{
public:
    DamageNumeral(
        const int numeral,
        const DirectX::XMFLOAT3& worldPos,
        const DirectX::XMFLOAT2& size,
        const DirectX::XMFLOAT4& color,
        const float angle,
        const float rowOffset
    );
    ~DamageNumeral() override;

    void Initialize()  override;
    void Update(const float elapsedTime) override;
    void Render()      override;
    void BloomRender() override;    //�u���[�����ʕt���̕`��
    void DrawDebug()   override;

    void SetParent(Character* parent) { parent_ = parent; }

private:
    static constexpr float LIFE_TIME_ = 1.5f;

private:
    static unsigned int nameNum_;

private:
    Character*  parent_     = nullptr;
    float       lifeTimer_  = LIFE_TIME_;
    float       addPosY_    = 0.0f;
};
