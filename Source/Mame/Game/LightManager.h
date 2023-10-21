#pragma once
#include "../Other/MathHelper.h"

class LightManager
{
private:
    LightManager() {}
    ~LightManager() {}

public:
    enum ColorType
    {
        GROUND,
        SKY,
        END
    };

    LightManager& Instance() 
    {
        static LightManager instance;
        return instance;
    }

    void Initialize();
    void Update(float elapsedTime);

    void ChangeColorUpdate(int type,float elapsedTime);

    void GradualChangeColor(int type,DirectX::XMFLOAT4 changeColor,const float changeTime);//指定の種類の色を徐々に変化させる
   
    void RestoreGroundColor(int type,const float changeTime);//指定の種類の色をもとに戻す

    const bool GetIsChangingColor(int type) const { return colorData[type].isChangingColor; }

private:

    struct ChangeColorData
    {
        //実際の値
        DirectX::XMFLOAT4* baseColor;

        //初期値
        DirectX::XMFLOAT4 initColor;

        //イージングのために保持しておく変更前の値
        DirectX::XMFLOAT4 holdColor;

        float changeTimer;
        float changeTime;

        bool isChangingColor;
    }colorData[END];
    
};

