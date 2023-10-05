#pragma once
#include "../Resource/sprite.h"
#include "../Graphics/Graphics.h"

class Player;

class BaseSkill
{
public:
    enum RARITY
    {
        COMMON,
        UNCOMMON,
        RARE,
        SUPER_REAR,
        ULTRA_REAR,
        END
    };

    BaseSkill(
        Player* player,
        const wchar_t* cardImageFilename,
        const wchar_t* iconImageFilename,
        const char* name,
        RARITY rear) :player(player), name(name), rarity(rear),overlap(0)
    {
        card = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), cardImageFilename, "./Resources/Shader/sprite_dissolve_ps.cso");
        icon = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), iconImageFilename);

        icon->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(60, 60));
    }
    ~BaseSkill() {}

    virtual void Initialize()
    {
        overlap = 0;
    }
    virtual void Update(float elapsedTime) {};
    virtual void Render();
    
    virtual void DrawDebug();

    virtual void Overlaping() = 0;//���̃X�L�����擾���邽�тɓ��鏈��

    void SetIconPos(const DirectX::XMFLOAT2 pos) { icon->GetSpriteTransform()->SetPos(pos); }
    void SetCardPos(const DirectX::XMFLOAT2 pos) { card->GetSpriteTransform()->SetPos(pos); }

    const int GetOverlapNum() const { return overlap; }

    std::unique_ptr<Sprite> card;//�J�[�h�摜
    std::unique_ptr<Sprite> icon;//�A�C�R���摜
    int rarity;//���A���e�B
protected:
    //int probability;//���̃X�L�����o�ɂ����@�l���傫���قǏo������m�����Ⴂ
    int overlap;//�d���񐔁i�����X�L�����d�����ĂƂ邱�ƂŔ\�͂������Ȃ�j

    Player* player;


    std::string name;
};

