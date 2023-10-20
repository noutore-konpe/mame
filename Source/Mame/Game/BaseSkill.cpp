#include "BaseSkill.h"
#include "Player.h"

void BaseSkill::Render()
{
    if (player->isSelectingSkill && isSelect)
    {
        card->Render();//‰¼
    }
    if (overlap > 0)icon->Render();
}

void BaseSkill::IconRender()
{
    icon->Render();
}

void BaseSkill::DrawDebug()
{
    if (ImGui::BeginMenu(name.c_str()))
    {
        ImGui::InputInt("Overlap", &overlap);

        if (ImGui::Button("Overlaping"))
        {
            Overlaping();
        }

        card->DrawDebug();
        icon->DrawDebug();

        ImGui::EndMenu();
    }
}
