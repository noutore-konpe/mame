#include "Ability.h"

int Ability::nameNum = 0;

// •`‰æˆ—
void Ability::Render(const float& scale, ID3D11PixelShader* psShader)
{
    model->Render(scale, psShader);
}

// ImGui—p
void Ability::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif// USE_IMGUI
}