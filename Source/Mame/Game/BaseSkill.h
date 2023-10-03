#pragma once

class Player;

class BaseSkill
{
public:
    BaseSkill(Player* player):player(player), overlap(0) {}
    ~BaseSkill() {}

    virtual void Initialize() = 0;
    virtual void Update(float elapsedTime) = 0;
    virtual void Render() = 0;
    virtual void DrawDebug() = 0;

    virtual void Overlaping() = 0;

protected:
    int probability;//���̃X�L�����o�ɂ����@�l���傫���قǏo������m�����Ⴂ
    int overlap;//�d���񐔁i�����X�L�����d�����ĂƂ邱�ƂŔ\�͂������Ȃ�j

    Player* player;
};

