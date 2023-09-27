#include "Item.h"

int Item::nameNum = 0;

// �`�揈��
void Item::Render(const float& scale, ID3D11PixelShader* psShader)
{
    model->Render(scale, psShader);
}

// ImGui�p
void Item::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}

// �A�j���[�V�����Đ��ݒ�
void Item::PlayAnimation(const int& index, const bool& loop, const float& speed, const float& blendSeconds)
{
    model->PlayAnimation(index, loop, speed, blendSeconds);
}

// �u�����h���̌v�Z�X�V����
void Item::UpdateBlendRate(float blendRate, const float& elapsedTime)
{
    model->UpdateBlendRate(blendRate, elapsedTime);
}

// �A�j���[�V�����X�V����
void Item::UpdateAnimation(const float& elapsedTime)
{
    model->UpdateAnimation(elapsedTime);
}

// �A�j���[�V�������Đ������ǂ���
bool Item::IsPlayAnimation() const
{
    return model->IsPlayAnimation();
}
