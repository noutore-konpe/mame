#pragma once

#include <vector>
#include "../Mame/Resource/sprite.h"

class Numeral
{
public:
    Numeral(
        const int numeral,
        const DirectX::XMFLOAT3& worldPos,
        const DirectX::XMFLOAT2& size,
        const DirectX::XMFLOAT4& color,
        const float angle,
        const float rowOffset // �s�Ԋu�̒����l
    );
    virtual ~Numeral();

    virtual void Initialize();
    virtual void Update(const float elapsedTime);
    virtual void Render();
    virtual void BloomRender(); // �u���[�����ʕt���̕`��
    virtual void DrawDebug();

protected:
    void Destroy();

protected:
    static constexpr DirectX::XMFLOAT2 TEX_SIZE_ = { 60.0f, 100.0f };

protected:
    std::vector<Sprite*> sprites_;
    std::string          name_;
    float                rowOffset_;
    int                  numeral_;          // �\�����鐔��
    bool                 isDraw_ = false;   // �`�悷�邩(�X�N���[�����W�ϊ��֐��ɓ���Ďg��)

};

