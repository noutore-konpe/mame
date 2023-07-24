#pragma once
#include "BaseScene.h"

namespace Mame::Scene
{

    class SceneManager
    {
    private:
        SceneManager() {}
        ~SceneManager() {}

    public:
        static SceneManager& Instance()
        {
            static SceneManager instance;
            return instance;
        }

        void Initialize();
        void Update(float elapesdTime);  // 更新処理
        void Render(float elapsedTime);  // 描画処理

        void DrawDebug();

        void Clear();   // シーンクリア

        // シーン切り替え
        void ChangeScene(BaseScene* scene);

        BaseScene* GetCurrentScene()const { return currentScene; }

    private:
        BaseScene* currentScene = nullptr;
        BaseScene* nextScene = nullptr;
    };
}
