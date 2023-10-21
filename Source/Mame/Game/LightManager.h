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

    void GradualChangeColor(int type,DirectX::XMFLOAT4 changeColor,const float changeTime);//�w��̎�ނ̐F�����X�ɕω�������
   
    void RestoreGroundColor(int type,const float changeTime);//�w��̎�ނ̐F�����Ƃɖ߂�

    const bool GetIsChangingColor(int type) const { return colorData[type].isChangingColor; }

private:

    struct ChangeColorData
    {
        //���ۂ̒l
        DirectX::XMFLOAT4* baseColor;

        //�����l
        DirectX::XMFLOAT4 initColor;

        //�C�[�W���O�̂��߂ɕێ����Ă����ύX�O�̒l
        DirectX::XMFLOAT4 holdColor;

        float changeTimer;
        float changeTime;

        bool isChangingColor;
    }colorData[END];
    
};

