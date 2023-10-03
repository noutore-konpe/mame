#pragma once

class Player;

class BaseSkill
{
public:
    BaseSkill(Player* player):player(player), overlap(0) {}
    ~BaseSkill() {}

    virtual void Initialize() = 0;
    virtual void Update(float elapsedTime) {};
    virtual void Render() {};
    virtual void DrawDebug() {};

    virtual void Overlaping() = 0;//このスキルを取得するたびに入る処理

protected:
    int probability;//このスキルが出にくさ　値が大きいほど出現する確率が低い
    int overlap;//重複回数（同じスキルを重複してとることで能力が強くなる）

    Player* player;
};

