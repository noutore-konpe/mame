#include "Item.h"

int Item::nameNum = 0;

// �R���X�g���N�^
Item::Item()
{

}

// ������
void Item::Initialize()
{

}

// �I����
void Item::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void Item::Begin()
{
}

// �X�V����
void Item::Update(const float& elapsedTime)
{

}

// Update�̌�ɌĂ΂��
void Item::End()
{
}

// �`�揈��
void Item::Render(const float& elapsedTime, const float& scale)
{
    model->Render(scale);
}

// ImGui�p
void Item::DrawDebug()
{
#ifdef USE_IMGUI

    GetTransform()->DrawDebug();

#endif // USE_IMGUI
}