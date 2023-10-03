#include "framework.h"

#include <fstream>

#include "Graphics/EffectManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneDemo.h"
#include "Scene/SceneManager.h"

#include "Resource/AudioManager.h"

#include "Resource/texture.h"

high_resolution_timer framework::tictoc = {};

framework::framework(HWND hwnd)
    : hwnd(hwnd),
    graphics(hwnd, FALSE),
    input(hwnd)
{
}

bool framework::initialize()
{
    HRESULT hr{ S_OK };

    // mouseの初期設定
    input.GetMouse().SetScreenWidth(SCREEN_WIDTH);
    input.GetMouse().SetScreenHeight(SCREEN_HEIGHT);

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
    Mame::Scene::SceneManager::Instance().ChangeScene(new SceneGame);
    //Mame::Scene::SceneManager::Instance().ChangeScene(new SceneDemo);

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
}

void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
    Graphics& graphics = Graphics::Instance();

    // todo scenemanager
    Mame::Scene::SceneManager::Instance().Render(elapsed_time);

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