#include "Numeral.h"
#include "NumeralManager.h"

#include <string>

#include "../Mame/Graphics/Graphics.h"
#include "../Taki174/Common.h"

Numeral::Numeral(
    const int numeral,
    const DirectX::XMFLOAT3& worldPos,
    const DirectX::XMFLOAT2& size,
    const DirectX::XMFLOAT4& color,
    const float angle,
    const float rowOffset)
    : numeral_(numeral)
    , rowOffset_(rowOffset)
{
    // 0未満の値は受け付けないようにする
    _ASSERT_EXPR((numeral >= 0), L"Input numeral is smaller than 0");

    using DirectX::XMFLOAT2;

    Graphics& graphics = Graphics::Instance();

    // 数字の桁数だけスプライトを生成・設定
    {
        // 数字を文字列に変換
        const std::string numeralStr = std::to_string(numeral);
        const size_t numeralCount = numeralStr.size();

        // 中心に位置調整＋行間隔の調整
        const float modifytLeft = (-TEX_SIZE_.x + rowOffset) * 0.5f * numeralCount;    // 全体を左にずらす修正量(真ん中に位置を修正する)
        const float modifyRight = TEX_SIZE_.x - rowOffset;                              // 桁数ごとに右にずらす修正量
        const float modifytUp   = -TEX_SIZE_.y * 0.5f;                                  // 全体を上にずらす修正量(真ん中に位置を修正する)

        for (size_t i = 0; i < numeralCount; ++i)
        {
            // スプライト生成
            Sprite* sprite = new Sprite(
                graphics.GetDevice(),
                L"./Resources/Image/UI/numbers.png"
            );

            // スプライト設定
            Sprite::SpriteTransform* spriteT = sprite->GetSpriteTransform();
            {
                // スクリーン座標変換・位置調整
                XMFLOAT2 screenPos = Sprite::ConvertToScreenPos(worldPos, &isDraw_);
                screenPos.x += modifytLeft + (modifyRight * static_cast<float>(i));
                screenPos.y += modifytUp;
                spriteT->SetPos(screenPos);

                spriteT->SetSize(size);         // スケール
                spriteT->SetColor(color);
                spriteT->SetAngle(angle);
                spriteT->SetTexSize(TEX_SIZE_); // 切り取る画像の幅・高さ

                // 数字に対応したスプライトのテクスチャ位置を設定
                const float number  = static_cast<float>(numeralStr.at(i) - '0');
                const float texPosX = spriteT->GetTexSize().x * number;
                spriteT->SetTexPos(XMFLOAT2(texPosX, 0.0f));

                // sprites_に追加
                sprites_.emplace_back(sprite);
            }

        }

    }

}

Numeral::~Numeral()
{
    for (Sprite*& sprite : sprites_)
    {
        ::SafeDeletePtr(sprite);
    }
    sprites_.clear();
    sprites_.shrink_to_fit();
}

void Numeral::Initialize()
{
    for (Sprite*& sprite : sprites_)
    {
        sprite->Initialize();
    }
}


void Numeral::Update(const float elapsedTime)
{
    for (Sprite*& sprite : sprites_)
    {
        sprite->Update(elapsedTime);
    }
}

void Numeral::Render()
{
    if (false == this->isDraw_) return;

    for (Sprite*& sprite : sprites_)
    {
        sprite->Render();
    }
}

void Numeral::BloomRender()
{
    if (false == this->isDraw_) return;

    //for (Sprite*& sprite : sprites_)
    //{
    //    sprite->BloomRender();
    //}
}

void Numeral::DrawDebug()
{
#ifdef USE_IMGUI

    ImGui::InputInt("numeral_", &numeral_);

    for (Sprite*& sprite : sprites_)
    {
        sprite->DrawDebug();
    }

#endif
}

void Numeral::Destroy()
{
    NumeralManager::Instance().Remove(this);
}
