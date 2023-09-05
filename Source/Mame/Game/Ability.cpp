#include "Ability.h"

int Ability::nameNum = 0;

// •`‰æˆ—
void Ability::Render(const float& scale)
{
    model->Render(scale);
}

// ImGui—p
void Ability::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif// USE_IMGUI
}