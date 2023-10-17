#include "DamageNumeral.h"

#include "../../External/imgui/ImGuiCtrl.h"
#include "../Mame/Game/Character.h"

unsigned int DamageNumeral::nameNum_ = 0;

DamageNumeral::DamageNumeral(
    const int numeral,
    const DirectX::XMFLOAT3& worldPos,
    const DirectX::XMFLOAT2& size,
    const DirectX::XMFLOAT4& color,
    const float angle,
    const float rowOffset)
    : Numeral(numeral, worldPos, size, color, angle, rowOffset)
{
    name_ = "DamageNumeral : " + std::to_string(nameNum_++);
}

DamageNumeral::~DamageNumeral()
{
    --nameNum_;
}

void DamageNumeral::Initialize()
{
    Numeral::Initialize();
}

void DamageNumeral::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT3;

    // 寿命処理
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) { this->Destroy(); }
    }

    Numeral::Update(elapsedTime);

    // スプライト更新処理
    const size_t spriteCount = sprites_.size();
    for (size_t i = 0; i < spriteCount; ++i)
    {
        Sprite* sprite = sprites_.at(i);
        Sprite::SpriteTransform* spriteT = sprite->GetSpriteTransform();

        // 移動処理
        {
            // 位置更新
            if (parent_ != nullptr)
            {
                // 中心に位置調整＋行間隔の調整
                const float modifytLeft = (-TEX_SIZE_.x + rowOffset_) * 0.5f * spriteCount; // 全体を左にずらす修正量(真ん中に位置を修正する)
                const float modifyRight = TEX_SIZE_.x - rowOffset_;                         // 桁数ごとに右にずらす修正量

                XMFLOAT3 worldPos = parent_->GetTransform()->GetPosition();
                worldPos.y += parent_->GetHeight(); // キャラクターの頭上に設定

                XMFLOAT2 screenPos = Sprite::ConvertToScreenPos(worldPos, &isDraw_);
                screenPos.x += modifytLeft + (modifyRight * static_cast<float>(i));

                spriteT->SetPosY(screenPos.y + addPosY_);
                spriteT->SetPosX(screenPos.x);
            }

        }

        // 色処理
        {
            // 透けていく
            spriteT->SetColorW(lifeTimer_ / LIFE_TIME_);
        }

    }

    // 上昇させる
    addPosY_ += -20.0f * elapsedTime;
}

void DamageNumeral::Render()
{
    Numeral::Render();
}

void DamageNumeral::BloomRender()
{
    Numeral::BloomRender();
}

void DamageNumeral::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(name_.c_str()))
    {
        Numeral::DrawDebug();

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif
}
