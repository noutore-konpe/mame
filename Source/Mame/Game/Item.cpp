#include "Item.h"

int Item::nameNum = 0;

// �`�揈��
void Item::Render(const float& scale)
{
    model->Render(scale);
}

// ImGui�p
void Item::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}