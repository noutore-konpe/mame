#include "Ability.h"
#include "AbilityManager.h"

Ability::Ability(AbilityManager* abilityManager)
    : abilityManager_(abilityManager)
{
}

// •`‰æˆ—
void Ability::Render(const float scale, ID3D11PixelShader* psShader)
{
    model_->Render(scale, psShader);
}

// ImGui—p
void Ability::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif// USE_IMGUI
}

void Ability::Destroy()
{
    abilityManager_->Remove(this);
}
