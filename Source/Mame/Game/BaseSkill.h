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

    virtual void Overlaping() = 0;//このスキルを取得するたびに入る処理

    void SetIconPos(const DirectX::XMFLOAT2 pos) { icon->GetSpriteTransform()->SetPos(pos); }
    void SetCardPos(const DirectX::XMFLOAT2 pos) { card->GetSpriteTransform()->SetPos(pos); }

    const int GetOverlapNum() const { return overlap; }

    std::unique_ptr<Sprite> card;//カード画像
    std::unique_ptr<Sprite> icon;//アイコン画像
    int rarity;//レアリティ
protected:
    //int probability;//このスキルが出にくさ　値が大きいほど出現する確率が低い
    int overlap;//重複回数（同じスキルを重複してとることで能力が強くなる）

    Player* player;


    std::string name;
};

