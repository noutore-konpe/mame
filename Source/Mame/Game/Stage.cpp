#include "Stage.h"
#include "../Graphics/Graphics.h"

// コンストラクタ
Stage::Stage()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), 
        "./Resources/Model/Stage/plane.fbx", 
        //"./Resources/Model/testModel/death_valley_-_terrain.fbx", 
        "./Resources/Shader/StagePS.cso");
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    GetTransform()->SetPositionY(-1.0f);
    GetTransform()->SetScaleFactor(100.0f);
}

void Stage::Finalize()
{
}

void Stage::Begin()
{
}

void Stage::Update(const float& elapsedTime)
{
}

void Stage::End()
{
}

void Stage::Render(const float& elapsedTime, const float& scale)
{
    model->Render(scale);
}

void Stage::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("Stage"))
    {
        GetTransform()->DrawDebug();
        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
