#pragma once
#include "../Other/MathHelper.h"

class LightColorManager
{
private:
    LightColorManager() {}
    ~LightColorManager() {}

public:
    enum ColorType
    {
        GROUND,
        SKY,
        END
    };

    static LightColorManager& Instance() 
    {
        static LightColorManager instance;
        return instance;
    }

    void Initialize();
    void Update(float elapsedTime);
    void DrawDebug();

private:
    void ChangeColorUpdate(int type,float elapsedTime);
    void UpdateVignette(float elapsedTime);

public:
    void GradualChangeColor(int type,DirectX::XMFLOAT3 changeColor,const float changeTime);//指定の種類の色を徐々に変化させる
    void RestoreColor(int type,const float changeTime);//指定の種類の色をもとに戻す

    void AllGradualChangeColor(DirectX::XMFLOAT3 changeColor, const float changeTime);//全ての種類の色を徐々に変化させる
    void AllRestoreColor(const float changeTime);//全ての種類の色をもとに戻す

    void ChangeVignetteValue(float vignetteValue,const float time);
    void RestoreVignetteValue(const float time);

    const bool GetIsChangingColor(int type) const { return colorData[type].isChangingColor; }

private:

    struct ChangeColorData
    {
        //実際の値
        DirectX::XMFLOAT4* baseColor;

        //変化後の色
        DirectX::XMFLOAT3 resultColor;

        //初期値
        DirectX::XMFLOAT3 initColor;

        //イージングのために保持しておく変更前の値
        DirectX::XMFLOAT3 holdColor;

        float changeTimer;
        float changeTime;

        bool isChangingColor;
    }colorData[END];

    //めんどいのでビネットもこっちで管理する
    float* vignetteValue;
    bool isChangingVignette;
    float vignetteTimer;
    float vignetteTime;
    float initVignette;
    float holdVignette;
    float resultVignette;
};

