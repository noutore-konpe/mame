#pragma once

class EnemyBlueSlime;

// ���s����
class JudgmentBase
{
public:
	JudgmentBase(EnemyBlueSlime* enemy) :owner(enemy) {}
	virtual bool Judgment() = 0;
protected:
	EnemyBlueSlime* owner;
};