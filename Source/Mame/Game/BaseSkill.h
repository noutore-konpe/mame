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

    virtual void Overlaping() = 0;//���̃X�L�����擾���邽�тɓ��鏈��

protected:
    int probability;//���̃X�L�����o�ɂ����@�l���傫���قǏo������m�����Ⴂ
    int overlap;//�d���񐔁i�����X�L�����d�����ĂƂ邱�ƂŔ\�͂������Ȃ�j

    Player* player;
};

