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
    //分母を100としたそぞれのレアリティの出現確率
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

    virtual void Overlaping() = 0;//このスキルを取得するたびに入る処理

    void SetIconPos(const DirectX::XMFLOAT2 pos) { icon->GetSpriteTransform()->SetPos(pos); }
    void SetCardPos(const DirectX::XMFLOAT2 pos) { card->GetSpriteTransform()->SetPos(pos); }

    const int GetOverlapNum() const { return overlap; }

    void SetPlayer(Player* player) { this->player = player; }

    std::unique_ptr<Sprite> card;//カード画像
    std::unique_ptr<Sprite> icon;//アイコン画像
    int rarity;//レアリティ

    bool isSelect;//選択肢の候補に上がっているか（上がっているならcard->render()を呼ぶ）

    bool isOneSheet;//一枚限定重複しない
protected:
    //int probability;//このスキルが出にくさ　値が大きいほど出現する確率が低い
    int overlap;//重複回数（同じスキルを重複してとることで能力が強くなる）

    Player* player;


    std::string name;
};

