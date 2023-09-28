#pragma once

class EnemyBlueSlime;

// é¿çsîªíË
class JudgmentBase
{
public:
	JudgmentBase(EnemyBlueSlime* enemy) :owner_(enemy) {}
	virtual ~JudgmentBase() {}
	virtual bool Judgment() = 0;

protected:
	EnemyBlueSlime* owner_;

};