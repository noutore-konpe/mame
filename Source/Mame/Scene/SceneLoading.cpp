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
    loadingPlayer = std::make_unique<LoadingPlayer>();
    titleLogo = std::make_unique<Sprite>(Graphics::Instance().GetDevice(),
        L"./Resources/Image/Title/loading.png");
}

// 初期化
void SceneLoading::Initialize()
{
    loadingPlayer->Initialize();
    titleLogo->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(800, 480));
    titleLogo->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(640, 360));

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
    loadingPlayer->Update(elapsedTime);

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
    // 描画の初期化
    {
        Graphics& graphics = Graphics::Instance();

        Mame::Scene::BaseScene::RenderInitialize();

        loadingPlayer->Render(0.01f);
        titleLogo->Render();
    }
}

void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
    Graphics::Instance().GetShader()->DrawDebug();

    loadingPlayer->DrawDebug();

    titleLogo->DrawDebug();
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