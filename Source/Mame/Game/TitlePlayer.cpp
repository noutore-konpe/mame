#include "TitlePlayer.h"
#include "../Mame/Graphics/Graphics.h"

TitlePlayer::TitlePlayer()
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(graphics.GetDevice(), "./resources/mameoall.fbx", true);
}

void TitlePlayer::Initialize()
{  
    // パラメータ設定
    //circle_.center  = { 0,0 };
    //circle_.radius  = 20.0f;
    //rotationY_      = ToRadian(0.0f);
    //rotationSpeedY_ = ToRadian(-45.0f);

    model_->PlayAnimation(Anim_Run, true); // 走行アニメーション再生
}

void TitlePlayer::Update(const float elapsedTime)
{
    CircularMotion(elapsedTime);            // 円運動行動処理

    model_->UpdateAnimation(elapsedTime);   // アニメーション更新
}

void TitlePlayer::Render(const float elapsedTime)
{ 
    TitleCharacter::Render(elapsedTime); // 共通の描画処理
}

void TitlePlayer::DrawDebug()
{
    ImGui::Begin("TitlePlayer");
    TitleCharacter::DrawDebug();
    ImGui::End();
}
