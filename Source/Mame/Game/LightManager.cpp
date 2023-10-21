#include "LightManager.h"
#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"

void LightManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // 地面色
    shader->lightConstant.hemisphereLight.groundColor.x = 0.7f;
    shader->lightConstant.hemisphereLight.groundColor.y = 0.5f;
    shader->lightConstant.hemisphereLight.groundColor.z = 0.3f;

    // 天球色
    shader->lightConstant.hemisphereLight.skyColor.x = 0.15f;
    shader->lightConstant.hemisphereLight.skyColor.y = 0.7f;
    shader->lightConstant.hemisphereLight.skyColor.z = 0.95f;

    for (int i = 0; i < ColorType::END; i++)
    {
        colorData[i].baseColor = &shader->lightConstant.hemisphereLight.groundColor;
        colorData[i].initColor = *colorData[i].baseColor;

        colorData[i].isChangingColor = false;
    }
}

void LightManager::Update(float elapsedTime)
{
    for (int type = 0; type < ColorType::END; type++)
    {
        ChangeColorUpdate(type, elapsedTime);
    }
}

void LightManager::ChangeColorUpdate(int type,float elapsedTime)
{
    if (colorData[type].isChangingColor)
    {
        //色変更中
        //Easing::OutCubic()

    }
    else
    {
        //変更なし


    }
}

void LightManager::GradualChangeColor(int type, DirectX::XMFLOAT4 changeColor, const float changeTime)
{
    colorData[type].changeTime = changeTime;
    colorData[type].holdColor = changeColor;

    colorData[type].isChangingColor = true;
}

void LightManager::RestoreGroundColor(int type, const float changeTime)
{
    colorData[type].changeTime = changeTime;
    colorData[type].holdColor = colorData[type].initColor;

    colorData[type].isChangingColor = true;
}
