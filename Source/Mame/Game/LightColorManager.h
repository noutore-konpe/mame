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
    void GradualChangeColor(int type,DirectX::XMFLOAT3 changeColor,const float changeTime);//�w��̎�ނ̐F�����X�ɕω�������
    void RestoreColor(int type,const float changeTime);//�w��̎�ނ̐F�����Ƃɖ߂�

    void AllGradualChangeColor(DirectX::XMFLOAT3 changeColor, const float changeTime);//�S�Ă̎�ނ̐F�����X�ɕω�������
    void AllRestoreColor(const float changeTime);//�S�Ă̎�ނ̐F�����Ƃɖ߂�

    void ChangeVignetteValue(float vignetteValue,const float time);
    void RestoreVignetteValue(const float time);

    const bool GetIsChangingColor(int type) const { return colorData[type].isChangingColor; }

private:

    struct ChangeColorData
    {
        //���ۂ̒l
        DirectX::XMFLOAT4* baseColor;

        //�ω���̐F
        DirectX::XMFLOAT3 resultColor;

        //�����l
        DirectX::XMFLOAT3 initColor;

        //�C�[�W���O�̂��߂ɕێ����Ă����ύX�O�̒l
        DirectX::XMFLOAT3 holdColor;

        float changeTimer;
        float changeTime;

        bool isChangingColor;
    }colorData[END];

    //�߂�ǂ��̂Ńr�l�b�g���������ŊǗ�����
    float* vignetteValue;
    bool isChangingVignette;
    float vignetteTimer;
    float vignetteTime;
    float initVignette;
    float holdVignette;
    float resultVignette;
};

