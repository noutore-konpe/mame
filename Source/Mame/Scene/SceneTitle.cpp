#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "../Mame.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "../Resource/AudioManager.h"

#include "../Other/misc.h"
#include "../Resource/texture.h"


void SceneTitle::CreateResource()
{
}

// 初期化
void SceneTitle::Initialize()
{
    // カメラ
    Camera::Instance().Initialize();
}

// 終了化
void SceneTitle::Finalize()
{
}

// 毎フレーム一番最初に呼ばれる
void SceneTitle::Begin()
{
}

// 更新処理
void SceneTitle::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
}

// 毎フレーム一番最後に呼ばれる
void SceneTitle::End()
{
}

// 描画処理
void SceneTitle::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // 描画の初期設定
    {
        // 描画の初期設定※必ず呼ぶこと！！！
        Mame::Scene::BaseScene::RenderInitialize();
    }

    // モデル描画
    {

    }

}


// デバッグ用
void SceneTitle::DrawDebug()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    
    // ライトとか
    shader->DrawDebug();
}
