#include "SceneLoading.h"

#include "../Graphics/Graphics.h"
#include "SceneManager.h"

SceneLoading::SceneLoading(BaseScene* nextScene) :nextScene(nextScene) 
{
    Graphics& graphics = Graphics::Instance();

    // シーンの属性を設定
    SetSceneType(static_cast<int>(Mame::Scene::TYPE::LOAD));
}

void SceneLoading::CreateResource()
{
}

// 初期化
void SceneLoading::Initialize()
{
    // スレッド開始
    // std::thread(LoadingThread, this);
    // 二個目の引数はLoadingThreadの引数になるu
    thread = new std::thread(LoadingThread, this);
}

// 終了化
void SceneLoading::Finalize()
{
    // スレッド終了化
    thread->join();
    if (thread != nullptr)
    {
        delete thread;
        thread = nullptr;
    }
}

// Updateの前に呼ばれる関数
void SceneLoading::Begin()
{
}

// 更新処理
void SceneLoading::Update(const float& elapsedTime)
{
    if (nextScene->IsReady())
        Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);
}

// Updateの後に呼ばれる
void SceneLoading::End()
{
}

// 描画処理
void SceneLoading::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();
    ID3D11RenderTargetView* renderTargetView = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depthStencileView = graphics.GetDepthStencilView();

    // 画面クリア&レンダーターゲット設定
    FLOAT color[] = { 0.0f,1.0f,0.0f,1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, color);
    deviceContext->ClearDepthStencilView(depthStencileView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencileView);
}

void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
#endif// USE_IMGUI
}

// ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    std::ignore = CoInitialize(nullptr); // std::ignoreで返り値警告解消

    // 次のシーンの初期化を行う
    scene->nextScene->CreateResource();
    scene->nextScene->Initialize();

    // スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    scene->nextScene->SetReady();
}