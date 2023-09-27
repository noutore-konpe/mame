#include "Item.h"

int Item::nameNum = 0;

// 描画処理
void Item::Render(const float& scale, ID3D11PixelShader* psShader)
{
    model->Render(scale, psShader);
}

// ImGui用
void Item::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}

// アニメーション再生設定
void Item::PlayAnimation(const int& index, const bool& loop, const float& speed, const float& blendSeconds)
{
    model->PlayAnimation(index, loop, speed, blendSeconds);
}

// ブレンド率の計算更新処理
void Item::UpdateBlendRate(float blendRate, const float& elapsedTime)
{
    model->UpdateBlendRate(blendRate, elapsedTime);
}

// アニメーション更新処理
void Item::UpdateAnimation(const float& elapsedTime)
{
    model->UpdateAnimation(elapsedTime);
}

// アニメーションが再生中かどうか
bool Item::IsPlayAnimation() const
{
    return model->IsPlayAnimation();
}
