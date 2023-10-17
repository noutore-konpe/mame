#include "DamageNumeral.h"

#include "../../External/imgui/ImGuiCtrl.h"
#include "../Mame/Game/Character.h"

unsigned int DamageNumeral::nameNum_ = 0;

DamageNumeral::DamageNumeral(
    const int numeral,
    const DirectX::XMFLOAT3& worldPos,
    const DirectX::XMFLOAT2& size,
    const DirectX::XMFLOAT4& color,
    const float angle,
    const float rowOffset)
    : Numeral(numeral, worldPos, size, color, angle, rowOffset)
{
    name_ = "DamageNumeral : " + std::to_string(nameNum_++);
}

DamageNumeral::~DamageNumeral()
{
    --nameNum_;
}

void DamageNumeral::Initialize()
{
    Numeral::Initialize();
}

void DamageNumeral::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT3;

    // ��������
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) { this->Destroy(); }
    }

    Numeral::Update(elapsedTime);

    // �X�v���C�g�X�V����
    const size_t spriteCount = sprites_.size();
    for (size_t i = 0; i < spriteCount; ++i)
    {
        Sprite* sprite = sprites_.at(i);
        Sprite::SpriteTransform* spriteT = sprite->GetSpriteTransform();

        // �ړ�����
        {
            // �ʒu�X�V
            if (parent_ != nullptr)
            {
                // ���S�Ɉʒu�����{�s�Ԋu�̒���
                const float modifytLeft = (-TEX_SIZE_.x + rowOffset_) * 0.5f * spriteCount; // �S�̂����ɂ��炷�C����(�^�񒆂Ɉʒu���C������)
                const float modifyRight = TEX_SIZE_.x - rowOffset_;                         // �������ƂɉE�ɂ��炷�C����

                XMFLOAT3 worldPos = parent_->GetTransform()->GetPosition();
                worldPos.y += parent_->GetHeight(); // �L�����N�^�[�̓���ɐݒ�

                XMFLOAT2 screenPos = Sprite::ConvertToScreenPos(worldPos, &isDraw_);
                screenPos.x += modifytLeft + (modifyRight * static_cast<float>(i));

                spriteT->SetPosY(screenPos.y + addPosY_);
                spriteT->SetPosX(screenPos.x);
            }

        }

        // �F����
        {
            // �����Ă���
            spriteT->SetColorW(lifeTimer_ / LIFE_TIME_);
        }

    }

    // �㏸������
    addPosY_ += -20.0f * elapsedTime;
}

void DamageNumeral::Render()
{
    Numeral::Render();
}

void DamageNumeral::BloomRender()
{
    Numeral::BloomRender();
}

void DamageNumeral::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(name_.c_str()))
    {
        Numeral::DrawDebug();

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif
}
