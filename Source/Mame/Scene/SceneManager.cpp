#include "SceneManager.h"


#include "../Graphics/Graphics.h"

#include "../Input/Input.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"

namespace Mame::Scene
{
    // 初期化
    void SceneManager::Initialize()
    {
    }

    // 更新処理
    void SceneManager::Update(float elapsedTime)
    {
        if (nextScene)
        {
            // 古いシーンを終了
            Clear();

            // 新しいシーンを設定
            currentScene = nextScene;
            nextScene = nullptr;

            // シーン初期化処理(マルチスレッド処理をしていない場合に行う)
            if (!currentScene->IsReady())
            {
                currentScene->CreateResource();
                currentScene->Initialize();
            }
        }

        if (currentScene)
        {
            // 経過時間が60フレーム以内なら更新処理を行う
            constexpr float frame60 = (0.0166666666666667f);
            //if (elapsedTime <= frame60)
            {

                currentScene->Begin();
                currentScene->Update(elapsedTime);
                currentScene->End();
            }
#ifdef USE_IMGUI
            currentScene->DrawDebug();
            DrawDebug();
#endif
        }
    }

    // 描画処理
    void SceneManager::Render(float elapsedTime)
    {
        if (!currentScene)return;

        currentScene->Render(elapsedTime);
    }

    // デバッグ用
    void SceneManager::DrawDebug()
    {
    }

    // シーンクリア
    void SceneManager::Clear()
    {
        if (!currentScene)return;

        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
        //SafeDelete(currentScene);
    }

    // シーン切り替え
    void SceneManager::ChangeScene(BaseScene* scene)
    {
        // 新しいシーンを設定
        nextScene = scene;
    }
}