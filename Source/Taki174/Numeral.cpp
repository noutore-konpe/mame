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
    const float angle)
    : numeral_(numeral)
{
    using DirectX::XMFLOAT2;

    Graphics& graphics = Graphics::Instance();

    // �����̌��������X�v���C�g�𐶐��E�ݒ�
    {
        // �����𕶎���ɕϊ�
        const std::string numeralStr = std::to_string(numeral);
        const size_t numeralCount = numeralStr.size();

        const XMFLOAT2 texSize = XMFLOAT2(60.0f, 100.0f);

        // �ʒu����
#pragma region Modify Position

#if 0   // �ʒu�����Ȃ�
        const float modifytLeft = 0.0f;
        const float modifyRight = texSize.x;
        const float modifytUp   = 0.0f;

#elif 0 // ���S�Ɉʒu����
        const float modifytLeft = -texSize.x * 0.5f * numeralCount;
        const float modifyRight = texSize.x;
        const float modifytUp   = -texSize.y * 0.5f;

#else   // ���S�Ɉʒu�����{�s�Ԋu�̒���
        static constexpr float rowOffset = 20.0f;                                   // �s�̊Ԋu�����p
        const float modifytLeft = (-texSize.x + rowOffset) * 0.5f * numeralCount;   // �S�̂����ɂ��炷�C����(�^�񒆂Ɉʒu���C������)
        const float modifyRight = texSize.x - rowOffset;                            // �������ƂɉE�ɂ��炷�C����
        const float modifytUp   = -texSize.y * 0.5f;                                // �S�̂���ɂ��炷�C����(�^�񒆂Ɉʒu���C������)

#endif

#pragma endregion

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
                XMFLOAT2 screenPos = Sprite::ConvertToScreenPos(worldPos);
                screenPos.x += modifytLeft + (modifyRight * static_cast<float>(i));
                screenPos.y += modifytUp;
                spriteT->SetPos(screenPos);

                spriteT->SetSize(size);         // �X�P�[��
                spriteT->SetColor(color);
                spriteT->SetAngle(angle);
                spriteT->SetTexSize(texSize);   // �؂���摜�̕��E����

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
    for (Sprite*& sprite : sprites_)
    {
        sprite->Render();
    }
}

void Numeral::BloomRender()
{
    for (Sprite*& sprite : sprites_)
    {
        sprite->Render();
    }
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
