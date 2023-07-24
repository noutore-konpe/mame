#include "TitleEnemyTofu.h"
#include "../Mame/Graphics/Graphics.h"

TitleEnemyTofu::TitleEnemyTofu()
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(graphics.GetDevice(), "./resources/tohuall.fbx", true);
}

void TitleEnemyTofu::Initialize()
{
    //circle_.center  = { 0,0 };
    //circle_.radius  = 20.0f;
    //rotationY_      = ToRadian(0.0f);
    //rotationSpeedY_ = ToRadian(-45.0f);

    model_->PlayAnimation(Anim_Walk, true, 1.25f); // 歩行アニメーション再生
}

void TitleEnemyTofu::Update(const float elapsedTime)
{
    CircularMotion(elapsedTime);            // 円運動行動処理

    model_->UpdateAnimation(elapsedTime);   // アニメーション更新
}

void TitleEnemyTofu::Render(const float elapsedTime)
{
    TitleCharacter::Render(elapsedTime); // 共通の描画処理
}

void TitleEnemyTofu::DrawDebug()
{
    ImGui::Begin("TitleEnemyTofu");
    TitleCharacter::DrawDebug();
    ImGui::End();
}
