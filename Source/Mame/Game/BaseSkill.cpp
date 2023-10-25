#include "BaseSkill.h"
#include "PlayerManager.h"

void BaseSkill::Initialize()
{
    player = PlayerManager::Instance().GetPlayer().get();
    overlap = 0;
    timer = 0;

    card = card1.get();
}

void BaseSkill::Render()
{
    if (player->isSelectingSkill && isSelect)
    {
        card->Render();//��
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
