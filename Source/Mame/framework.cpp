#include "framework.h"

#include <fstream>

#include "Graphics/EffectManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneDemo.h"
#include "Scene/SceneManager.h"

#include "Resource/AudioManager.h"

#include "Resource/texture.h"


framework::framework(HWND hwnd)
    : hwnd(hwnd),
    graphics(hwnd,FALSE),
    input(hwnd)
{
}

bool framework::initialize()
{
    HRESULT hr{ S_OK };
    
    //framebuffers[0] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720);
    //bit_block_transfer = std::make_unique<FullscreenQuad>(graphics.GetDevice());
    //framebuffers[1] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280 / 2, 720 / 2);
    //create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/luminance_extraction_ps.cso", pixel_shaders[0].GetAddressOf());
    //create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/blur_ps.cso", pixel_shaders[1].GetAddressOf());

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
    //Mame::Scene::SceneManager::Instance().ChangeScene(new SceneTitle);
    Mame::Scene::SceneManager::Instance().ChangeScene(new SceneDemo);

#ifndef _DEBUG
    ShowCursor(!FULLSCREEN);	// フルスクリーン時はカーソルを消す
#endif

    //framebuffers[0] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720);

    //// BLOOM
    //bloomer = std::make_unique<Bloom>(graphics.GetDevice(), 1280, 720);
    //create_ps_from_cso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", pixelShader.ReleaseAndGetAddressOf());

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

 //   // BLOOM
	//ImGui::Begin("bloom");
 //   ImGui::SliderFloat("bloom_extraction_threshold", &bloomer->bloomExtractionThreshold, +0.0f, +5.0f);
 //   ImGui::SliderFloat("bloom_intensity", &bloomer->bloomIntensity, +0.0f, +5.0f);
	//ImGui::End();
}

void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
    Graphics& graphics = Graphics::Instance();
 
    //framebuffers[0]->Clear(graphics.GetDeviceContext());
    //framebuffers[0]->Activate(graphics.GetDeviceContext());
    //framebuffers[0]->Deactivate(graphics.GetDeviceContext());

    // todo scenemanager
    Mame::Scene::SceneManager::Instance().Render(elapsed_time);
    

 //   // BLOOM
 //   bloomer->Make(graphics.GetDeviceContext(), framebuffers[0]->shaderResourceViews[0].Get());

	//graphics.GetDeviceContext()->OMSetDepthStencilState(depthStencilStates[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
	//graphics.GetDeviceContext()->RSSetState(rasterizerStates[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
	//graphics.GetDeviceContext()->OMSetBlendState(blendStates[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
	//ID3D11ShaderResourceView* shader_resource_views[] =
	//{
	//	framebuffers[0]->shaderResourceViews[0].Get(),
	//	bloomer->ShaderResourceView(),
	//};
	//bit_block_transfer->Blit(graphics.GetDeviceContext(), shader_resource_views, 0, 2, pixel_shaders[0].Get());

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