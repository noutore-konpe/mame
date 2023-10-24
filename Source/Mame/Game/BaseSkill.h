#pragma once
#include "../Resource/sprite.h"
#include "../Graphics/Graphics.h"

class Player;

using Probability = int;

class BaseSkill
{
public:
    enum RARITY
    {
        COMMON,
        UNCOMMON,
        RARE,
        SUPER_RARE,
        ULTRA_RARE,
        END
    };
    //�����100�Ƃ���������̃��A���e�B�̏o���m��
    static const Probability pCommon = 50;
    static const Probability pUncommon = 20;
    static const Probability pRare = 17;
    static const Probability pSuperRare = 10;
    static const Probability pUltraRare = 3;

    BaseSkill(
        Player* player,
        const wchar_t* cardImageFilename,
        const wchar_t* iconImageFilename,
        const char* name,
        RARITY rear,
        bool isOneSheet = false) :player(player), name(name), rarity(rear),overlap(0),isOneSheet(isOneSheet)
    {
        card = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), cardImageFilename, "./Resources/Shader/sprite_dissolve_ps.cso");
        icon = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), iconImageFilename);

        icon->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(60, 60));

        card->GetSpriteDissolve()->SetMaskTextureValue(2);
    }
    ~BaseSkill() {}

    virtual void Initialize();
    virtual void Update(float elapsedTime) {};
    virtual void Render();

    void IconRender();
    
    virtual void DrawDebug();

    virtual void Overlaping() = 0;//���̃X�L�����擾���邽�тɓ��鏈��

    void SetIconPos(const DirectX::XMFLOAT2 pos) { icon->GetSpriteTransform()->SetPos(pos); }
    void SetCardPos(const DirectX::XMFLOAT2 pos) { card->GetSpriteTransform()->SetPos(pos); }

    const int GetOverlapNum() const { return overlap; }

    void SetPlayer(Player* player) { this->player = player; }

    std::unique_ptr<Sprite> card;//�J�[�h�摜
    std::unique_ptr<Sprite> icon;//�A�C�R���摜
    int rarity;//���A���e�B

    bool isSelect;//�I�����̌��ɏオ���Ă��邩�i�オ���Ă���Ȃ�card->render()���Ăԁj

    bool isOneSheet;//�ꖇ����d�����Ȃ�
protected:
    //int probability;//���̃X�L�����o�ɂ����@�l���傫���قǏo������m�����Ⴂ
    int overlap;//�d���񐔁i�����X�L�����d�����ĂƂ邱�ƂŔ\�͂������Ȃ�j

    Player* player;


    std::string name;
};

