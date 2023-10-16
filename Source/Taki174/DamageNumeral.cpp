#include "DamageNumeral.h"

#include "../../External/imgui/ImGuiCtrl.h"


unsigned int DamageNumeral::nameNum_ = 0;

DamageNumeral::DamageNumeral(
    const int numeral,
    const DirectX::XMFLOAT3& worldPos,
    const DirectX::XMFLOAT2& size,
    const DirectX::XMFLOAT4& color,
    const float angle)
    : Numeral(numeral, worldPos, size, color, angle)
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
    // 寿命処理
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) { this->Destroy(); }
    }

    // スプライト更新処理
    for (Sprite*& sprite : sprites_)
    {
        Numeral::Update(elapsedTime);

        //// 移動処理
        Sprite::SpriteTransform* spriteT = sprite->GetSpriteTransform();
        {
            // 上昇していく
            const float addPosY = -10.0f * elapsedTime;
            spriteT->AddPosY(addPosY);
        }

        // 色処理
        {
            // 透けていく
            spriteT->SetColorW(lifeTimer_ / LIFE_TIME_);
        }

    }
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
