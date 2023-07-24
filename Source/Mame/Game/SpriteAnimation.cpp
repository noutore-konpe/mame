#include "SpriteAnimation.h"

#include "../Mame/Graphics/Graphics.h"

int SpriteAnimation::nameNum = 0;

SpriteAnimation::SpriteAnimation(const wchar_t* filename, const char* psFilename)
{
    Graphics& graphics = Graphics::Instance();

    sprite = std::make_unique<Sprite>(graphics.GetDevice(), filename, psFilename);

    // imguiñºëOÇ©Ç‘ÇËãNÇ´Ç»Ç¢ÇÊÇ§Ç…...
    name = "Animation" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

void SpriteAnimation::Initialize(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 texSize)
{
    SetPosition(pos);
    SetSize(size);
    SetTexPos(DirectX::XMFLOAT2(0, 0));
    SetTexSize(texSize);
    SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
    SetAngle(0);

    animationFrame = 0;
    animationTime = 0;
}

void SpriteAnimation::PlayAnimation(
    const float elapsedTime,
    const float frameSpeed, 
    const int   totalAnimationFrame, 
    const bool  animationVertical)
{
    animationTime += frameSpeed * elapsedTime;

    const int animetionTime_int = static_cast<int>(animationTime); // è¨êîì_êÿÇËéÃÇƒ
    animationFrame = static_cast<float>(animetionTime_int);

    if (animationFrame > totalAnimationFrame)
    {
        animationFrame = 0.0f;
        animationTime  = 0.0f;
    }

    if (animationVertical) texPos.y = texSize.y * animationFrame;
    else                   texPos.x = texSize.x * animationFrame;

    {
        //animationFrame = animationTime / frameTime;

        //if (animationFrame >= totalAnimationFrame)
        //{
        //    animationFrame = 0;
        //    animationTime = 0;
        //}

        //if(animationVertical)
        //    texPos.y = texSize.y * animationFrame;
        //else
        //    texPos.x = texSize.x * animationFrame;

        //++animationTime;
    }

}

void SpriteAnimation::Render()
{
    Graphics& graphics = Graphics::Instance();

    sprite->render(graphics.GetDeviceContext(), position.x, position.y,
        size.x, size.y, color.x, color.y, color.z, color.w, angle,
        texPos.x, texPos.y, texSize.x, texSize.y);
}

void SpriteAnimation::DrawDebug()
{
#ifdef USE_IMGUI
    ImGui::Begin(GetName());

    ImGui::DragFloat2("pos", &position.x);
    ImGui::DragFloat2("size", &size.x);
    ImGui::DragFloat2("texPos", &texPos.x);
    ImGui::DragFloat2("texSize", &texSize.x);
    ImGui::DragFloat("angle", &angle);
    ImGui::ColorEdit4("color", &color.x);

    ImGui::End();
#endif // USE_IMGUI
}
