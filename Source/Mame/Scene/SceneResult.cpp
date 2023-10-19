#include "SceneResult.h"

#include "SceneManager.h"
#include "SceneTitle.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"
#include "../Other/misc.h"

// ÉäÉ\Å[ÉXê∂ê¨
void SceneResult::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    whiteSprite = std::make_unique<Sprite>(graphics.GetDevice(),
        L"./Resources/Image/Game/white.png");

    enemyGolem = std::make_unique<EnemyGolemResult>();
    player = std::make_unique<PlayerResult>();


    CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/FinalPassTexturePS.cso", finalPassPS.GetAddressOf());
    
    HRESULT hr{ S_OK };    

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(ResultConstants);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    hr = graphics.GetDevice()->CreateBuffer(&desc, nullptr, resultConstantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void SceneResult::Initialize()
{
    enemyGolem->Initialize();
    player->Initialize();

    enemyResult[0].scale = { 0.7f, 0.7f, 0.7f };
    enemyResult[1].scale = { 0.7f, 0.7f, 0.7f };
    enemyResult[2].scale = { 0.7f, 0.7f, 0.7f };
    enemyResult[0].color = { 0.11f, 0.56f, 1.00f, 1.0f };
    enemyResult[1].color = { 0.0f, 0.0f, 1.0f, 1.0f };
    enemyResult[2].color = { 0.00f, 0.80f, 0.81f, 1.0f };
}

void SceneResult::Finalize()
{
}

void SceneResult::Begin()
{
}

void SceneResult::Update(const float& elapsedTime)
{
    enemyGolem->Update(elapsedTime);
    player->Update(elapsedTime);
}

void SceneResult::End()
{
}

void SceneResult::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Mame::Scene::BaseScene::RenderInitialize();


    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));
    shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
    whiteSprite->Render();

    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::NONE));
    resultConstants.color = { 0.3f, 0.3f, 0.3f ,1.0f };
    graphics.GetDeviceContext()->UpdateSubresource(resultConstantBuffer.Get(), 0, 0, &resultConstants, 0, 0);
    graphics.GetDeviceContext()->PSSetConstantBuffers(5, 1, resultConstantBuffer.GetAddressOf());
    enemyGolem->Render(0.01f);

    for (int i = 0; i < 3; ++i)
    {
        resultConstants.color = enemyResult[i].color;
        graphics.GetDeviceContext()->UpdateSubresource(resultConstantBuffer.Get(), 0, 0, &resultConstants, 0, 0);
        graphics.GetDeviceContext()->PSSetConstantBuffers(5, 1, resultConstantBuffer.GetAddressOf());
        player->GetTransform()->SetPosition(enemyResult[i].position);
        player->GetTransform()->SetScale(enemyResult[i].scale);
        player->swordModel->GetTransform()->SetPosition(enemyResult[i].position);
        player->Render(0.01f);
    }
}

void SceneResult::DrawDebug()
{
    ImGui::ColorEdit4("golemColor", &resultConstants.color.x);

    ImGui::Begin("golem");
    enemyGolem->DrawDebug();
    ImGui::End();

    ImGui::Begin("player");
    player->DrawDebug();
    ImGui::End();

    if (ImGui::BeginMenu("zako"))
    {
        ImGui::Begin("enemy0");
        ImGui::DragFloat3("position", &enemyResult[0].position.x);
        ImGui::DragFloat3("scale", &enemyResult[0].scale.x);
        ImGui::ColorEdit4("color", &enemyResult[0].color.x);
        ImGui::End();

        ImGui::Begin("enemy1");
        ImGui::DragFloat3("position", &enemyResult[1].position.x);
        ImGui::DragFloat3("scale", &enemyResult[1].scale.x);
        ImGui::ColorEdit4("color", &enemyResult[1].color.x);
        ImGui::End();

        ImGui::Begin("enemy2");
        ImGui::DragFloat3("position", &enemyResult[2].position.x);
        ImGui::DragFloat3("scale", &enemyResult[2].scale.x);
        ImGui::ColorEdit4("color", &enemyResult[2].color.x);
        ImGui::End();

        ImGui::EndMenu();
    }
}
