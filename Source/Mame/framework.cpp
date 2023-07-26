#include "framework.h"

#include <fstream>

#include "Graphics/EffectManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneManager.h"

#include "Resource/AudioManager.h"

#include "Resource/texture.h"

framework::framework(HWND hwnd)
    : hwnd(hwnd),
    graphics(hwnd),
    input(hwnd)
{
}

bool framework::initialize()
{
    HRESULT hr{ S_OK };
    
    framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), 1280, 720);
    bit_block_transfer = std::make_unique<fullscreen_quad>(graphics.GetDevice());
    framebuffers[1] = std::make_unique<framebuffer>(graphics.GetDevice(), 1280 / 2, 720 / 2);
    create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/luminance_extraction_ps.cso", pixel_shaders[0].GetAddressOf());
    create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/blur_ps.cso", pixel_shaders[1].GetAddressOf());

    // effectManager
    EffectManager::Instance().Initialize();

    // XAUDIO2
    AudioManager& audioManager = AudioManager::Instance();
    hr = XAudio2Create(audioManager.xAudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    hr = audioManager.xAudio2->CreateMasteringVoice(&audioManager.masterVoice);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    
    // 音楽読み込み
    //audioManager.LoadAudio();
    //audioManager.StopAllAudio(); // 全音楽停止

    Mame::Scene::SceneManager::Instance().Initialize();
    // シーンタイトル
    Mame::Scene::SceneManager::Instance().ChangeScene(new SceneTitle);

    // gltfModel
    gltfModels[0] = std::make_unique<GltfModel>(graphics.GetDevice(),
        //"./External/glTF-Sample-Models-master/2.0/ToyCar/glTF/ToyCar.gltf");
        //"./External/glTF-Sample-Models-master/2.0/ABeautifulGame/glTF/ABeautifulGame.gltf");
        //"./External/glTF-Sample-Models-master/2.0/BrainStem/glTF/BrainStem.gltf");
        //"./External/glTF-Sample-Models-master/2.0/CesiumMan/glTF/CesiumMan.gltf");
        "./External/glTF-Sample-Models-master/2.0/DamagedHelmet/glTF/DamagedHelmet.gltf");
        //"./External/glTF-Sample-Models-master/2.0/2CylinderEngine/glTF/2CylinderEngine.gltf");

    D3D11_TEXTURE2D_DESC texture2dDesc;
    load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/environments/sunset_jhbcentral_4k/sunset_jhbcentral_4k.dds",
        shaderResourceViews[0].GetAddressOf(), &texture2dDesc);
    load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/environments/sunset_jhbcentral_4k/diffuse_iem.dds",
        shaderResourceViews[1].GetAddressOf(), &texture2dDesc);
    load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/environments/sunset_jhbcentral_4k/specular_pmrem.dds",
        shaderResourceViews[2].GetAddressOf(), &texture2dDesc);
    load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/environments/lut_ggx.dds",
        shaderResourceViews[3].GetAddressOf(), &texture2dDesc);

    // sprite
    sprite = std::make_unique<Sprite>(graphics.GetDevice(), L"./Resources/Image/sanaImage/load.png",
        "./Resources/Shader/sprite_dissolve_ps.cso");
    //sprite->Initialize(DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(900, 367), DirectX::XMFLOAT2(900, 367));

    // model
    model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/sanaModel/1.fbx");

#ifndef _DEBUG
    ShowCursor(!FULLSCREEN);	// フルスクリーン時はカーソルを消す
#endif

    return true;
}


void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
    // ImGui更新
    IMGUI_CTRL_CLEAR_FRAME();

    // 入力更新処理
    input.Update();

    // シーン更新処理
    Mame::Scene::SceneManager::Instance().Update(elapsed_time);

    // spriteAnimation
    sprite->Update(elapsed_time);
    //sprite->PlayAnimation(elapsed_time, 30, 30, true);

    // gltfModel
    gltfModels[0]->Update(elapsed_time);
    gltfModels[0]->DrawDebug();
}

void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
 
    // todo scenemanager
    Mame::Scene::SceneManager::Instance().Render(elapsed_time);

    // gltfModel
    {
        graphics.GetDeviceContext()->PSSetShaderResources(32, 1, shaderResourceViews[0].GetAddressOf());
        graphics.GetDeviceContext()->PSSetShaderResources(33, 1, shaderResourceViews[1].GetAddressOf());
        graphics.GetDeviceContext()->PSSetShaderResources(34, 1, shaderResourceViews[2].GetAddressOf());
        graphics.GetDeviceContext()->PSSetShaderResources(35, 1, shaderResourceViews[3].GetAddressOf());

        gltfModels[0]->Render(1.0f);
    }

    // sprite
    {
        sprite->DrawDebug();
        sprite->Render();
    }
    
    // model
    {
        ImGui::Begin("model");
        model->DrawDebug();
        ImGui::End();
        model->Render(0.01f);
    }
    
    // ImGui表示
    IMGUI_CTRL_DISPLAY();

    UINT sync_interval{ 0 };
    graphics.GetSwapChain()->Present(sync_interval, 0); 
}

bool framework::uninitialize()
{    
    // シーン終了化
    Mame::Scene::SceneManager::Instance().Clear();

    return false;
}

framework::~framework()
{
    // effectmanager
    EffectManager::Instance().Finalize();
}