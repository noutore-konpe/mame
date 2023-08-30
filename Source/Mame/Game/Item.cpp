#include "Item.h"

int Item::nameNum = 0;

// コンストラクタ
Item::Item()
{

}

// 初期化
void Item::Initialize()
{

}

// 終了化
void Item::Finalize()
{
}

// Updateの前に呼ばれる
void Item::Begin()
{
}

// 更新処理
void Item::Update(const float& elapsedTime)
{

}

// Updateの後に呼ばれる
void Item::End()
{
}

// 描画処理
void Item::Render(const float& elapsedTime, const float& scale)
{
    model->Render(scale);
}

// ImGui用
void Item::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}