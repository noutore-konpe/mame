#include "TitlePlayer.h"
#include "../Mame/Graphics/Graphics.h"

TitlePlayer::TitlePlayer()
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(graphics.GetDevice(), "./resources/mameoall.fbx", true);
}

void TitlePlayer::Initialize()
{  
    // �p�����[�^�ݒ�
    //circle_.center  = { 0,0 };
    //circle_.radius  = 20.0f;
    //rotationY_      = ToRadian(0.0f);
    //rotationSpeedY_ = ToRadian(-45.0f);

    model_->PlayAnimation(Anim_Run, true); // ���s�A�j���[�V�����Đ�
}

void TitlePlayer::Update(const float elapsedTime)
{
    CircularMotion(elapsedTime);            // �~�^���s������

    model_->UpdateAnimation(elapsedTime);   // �A�j���[�V�����X�V
}

void TitlePlayer::Render(const float elapsedTime)
{ 
    TitleCharacter::Render(elapsedTime); // ���ʂ̕`�揈��
}

void TitlePlayer::DrawDebug()
{
    ImGui::Begin("TitlePlayer");
    TitleCharacter::DrawDebug();
    ImGui::End();
}
