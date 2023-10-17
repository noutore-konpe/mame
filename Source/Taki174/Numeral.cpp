#include "Numeral.h"
#include "NumeralManager.h"

#include <string>

#include "../Mame/Graphics/Graphics.h"
#include "../Taki174/Common.h"

Numeral::Numeral(
    const int numeral,
    const DirectX::XMFLOAT3& worldPos,
    const DirectX::XMFLOAT2& size,
    const DirectX::XMFLOAT4& color,
    const float angle,
    const float rowOffset)
    : numeral_(numeral)
    , rowOffset_(rowOffset)
{
    // 0�����̒l�͎󂯕t���Ȃ��悤�ɂ���
    _ASSERT_EXPR((numeral >= 0), L"Input numeral is smaller than 0");

    using DirectX::XMFLOAT2;

    Graphics& graphics = Graphics::Instance();

    // �����̌��������X�v���C�g�𐶐��E�ݒ�
    {
        // �����𕶎���ɕϊ�
        const std::string numeralStr = std::to_string(numeral);
        const size_t numeralCount = numeralStr.size();

        // ���S�Ɉʒu�����{�s�Ԋu�̒���
        const float modifytLeft = (-TEX_SIZE_.x + rowOffset) * 0.5f * numeralCount;    // �S�̂����ɂ��炷�C����(�^�񒆂Ɉʒu���C������)
        const float modifyRight = TEX_SIZE_.x - rowOffset;                              // �������ƂɉE�ɂ��炷�C����
        const float modifytUp   = -TEX_SIZE_.y * 0.5f;                                  // �S�̂���ɂ��炷�C����(�^�񒆂Ɉʒu���C������)

        for (size_t i = 0; i < numeralCount; ++i)
        {
            // �X�v���C�g����
            Sprite* sprite = new Sprite(
                graphics.GetDevice(),
                L"./Resources/Image/UI/numbers.png"
            );

            // �X�v���C�g�ݒ�
            Sprite::SpriteTransform* spriteT = sprite->GetSpriteTransform();
            {
                // �X�N���[�����W�ϊ��E�ʒu����
                XMFLOAT2 screenPos = Sprite::ConvertToScreenPos(worldPos, &isDraw_);
                screenPos.x += modifytLeft + (modifyRight * static_cast<float>(i));
                screenPos.y += modifytUp;
                spriteT->SetPos(screenPos);

                spriteT->SetSize(size);         // �X�P�[��
                spriteT->SetColor(color);
                spriteT->SetAngle(angle);
                spriteT->SetTexSize(TEX_SIZE_); // �؂���摜�̕��E����

                // �����ɑΉ������X�v���C�g�̃e�N�X�`���ʒu��ݒ�
                const float number  = static_cast<float>(numeralStr.at(i) - '0');
                const float texPosX = spriteT->GetTexSize().x * number;
                spriteT->SetTexPos(XMFLOAT2(texPosX, 0.0f));

                // sprites_�ɒǉ�
                sprites_.emplace_back(sprite);
            }

        }

    }

}

Numeral::~Numeral()
{
    for (Sprite*& sprite : sprites_)
    {
        ::SafeDeletePtr(sprite);
    }
    sprites_.clear();
    sprites_.shrink_to_fit();
}

void Numeral::Initialize()
{
    for (Sprite*& sprite : sprites_)
    {
        sprite->Initialize();
    }
}


void Numeral::Update(const float elapsedTime)
{
    for (Sprite*& sprite : sprites_)
    {
        sprite->Update(elapsedTime);
    }
}

void Numeral::Render()
{
    if (false == this->isDraw_) return;

    for (Sprite*& sprite : sprites_)
    {
        sprite->Render();
    }
}

void Numeral::BloomRender()
{
    if (false == this->isDraw_) return;

    //for (Sprite*& sprite : sprites_)
    //{
    //    sprite->BloomRender();
    //}
}

void Numeral::DrawDebug()
{
#ifdef USE_IMGUI

    ImGui::InputInt("numeral_", &numeral_);

    for (Sprite*& sprite : sprites_)
    {
        sprite->DrawDebug();
    }

#endif
}

void Numeral::Destroy()
{
    NumeralManager::Instance().Remove(this);
}
