#include "LightColorManager.h"
#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"

#include "../../imgui/ImGuiCtrl.h"

void LightColorManager::Initialize()
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

    colorData[ColorType::GROUND].baseColor = &shader->lightConstant.hemisphereLight.groundColor;
    colorData[ColorType::SKY].baseColor = &shader->lightConstant.hemisphereLight.skyColor;

    for (int i = 0; i < ColorType::END; i++)
    {
        colorData[i].initColor = { colorData[i].baseColor->x,colorData[i].baseColor->y,colorData[i].baseColor->z };

        colorData[i].isChangingColor = false;
    }

    //ビネット値取得
    shader->postEffectConstants.vignetteValue = 0.23f;
    vignetteValue = &shader->postEffectConstants.vignetteValue;
}

void LightColorManager::Update(float elapsedTime)
{
    for (int type = 0; type < ColorType::END; type++)
    {
        ChangeColorUpdate(type, elapsedTime);
    }
}

void LightColorManager::DrawDebug()
{
#if _DEBUG
    static DirectX::XMFLOAT3 groundColor = {0,0,0};
    static DirectX::XMFLOAT3 skyColor = { 0,0,0 };

    if (ImGui::BeginMenu("LightColor"))
    {
        ImGui::SliderFloat("Vignette", vignetteValue,0.0f,1.0f);

        if (ImGui::TreeNode("Color Changer"))
        {
            ImGui::ColorEdit3("GroundColor",&groundColor.x);
            ImGui::ColorEdit3("SkyColor",&skyColor.x);

            if (ImGui::Button("ChangeColor"))
            {
                GradualChangeColor(ColorType::GROUND,groundColor,3.0f);
                GradualChangeColor(ColorType::SKY,skyColor,3.0f);
            }

            if (ImGui::Button("RestoreColor"))
            {
                for (int i = 0; i < END; i++)
                {
                    RestoreColor(i,3.0f);
                }
            }

            ImGui::TreePop();
        }

        ImGui::EndMenu();
    }
#endif // _DEBUG
}

void LightColorManager::ChangeColorUpdate(int type,float elapsedTime)
{
    if (colorData[type].isChangingColor)
    {
        colorData[type].changeTimer += elapsedTime;

        if (colorData->changeTimer >= colorData[type].changeTimer)
        {
            colorData->changeTimer = colorData[type].changeTimer;
        }

        //色変更中
        colorData[type].baseColor->x = Easing::OutCubic(colorData[type].changeTimer, colorData[type].changeTime, colorData[type].holdColor.x, colorData[type].baseColor->x);
        colorData[type].baseColor->y = Easing::OutCubic(colorData[type].changeTimer, colorData[type].changeTime, colorData[type].holdColor.y, colorData[type].baseColor->y);
        colorData[type].baseColor->z = Easing::OutCubic(colorData[type].changeTimer, colorData[type].changeTime, colorData[type].holdColor.z, colorData[type].baseColor->z);

        if (colorData[type].changeTimer >= colorData[type].changeTime)
        {
            colorData[type].isChangingColor = false;
        }
    }
    else
    {
        //変更完了　または　変更なし


    }
}

void LightColorManager::UpdateVignette(float elapsedTime)
{
    if (isChangingVignette)
    {
        vignetteTimer += elapsedTime;

        if (vignetteTimer >= vignetteTime)
        {
            vignetteTimer = vignetteTime;
        }

        *vignetteValue = Easing::OutCubic(vignetteTimer, vignetteTime,resultVignette, holdVignette);

        if (vignetteTimer >= vignetteTime)
        {
            isChangingVignette = false;
        }
    }
}

void LightColorManager::GradualChangeColor(int type, DirectX::XMFLOAT3 changeColor, const float changeTime)
{
    colorData[type].changeTime = changeTime;
    colorData[type].holdColor = changeColor;

    colorData[type].changeTimer = 0;
    colorData[type].isChangingColor = true;
}

void LightColorManager::RestoreColor(int type, const float changeTime)
{
    colorData[type].changeTime = changeTime;
    colorData[type].holdColor = colorData[type].initColor;

    colorData[type].changeTimer = 0;
    colorData[type].isChangingColor = true;
}

void LightColorManager::AllGradualChangeColor(DirectX::XMFLOAT3 changeColor, const float changeTime)
{
    for (int i = 0; i < END; i++)
    {
        GradualChangeColor(i,changeColor,changeTime);
    }
}

void LightColorManager::AllRestoreColor(const float changeTime)
{
    for (int i = 0; i < END; i++)
    {
        RestoreColor(i,changeTime);
    }
}

void LightColorManager::ChangeVignetteValue(float vignetteValue, const float time)
{
    resultVignette = vignetteValue;
    holdVignette = vignetteValue;

    vignetteTime = time;
    vignetteTimer = 0;
    isChangingVignette = true;
}

void LightColorManager::RestoreVignetteValue(const float time)
{
    resultVignette = initVignette;
    holdVignette = *vignetteValue;

    vignetteTime = time;
    vignetteTimer = 0;
    isChangingVignette = true;
}
