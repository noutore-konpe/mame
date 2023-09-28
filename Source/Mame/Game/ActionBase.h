#pragma once

class EnemyBlueSlime;

// �s���������N���X
class ActionBase
{
public:
	// ���s���
	enum class State
	{
		Run,		// ���s��
		Failed,		// ���s���s
		Complete,	// ���s����
	};

public:
	ActionBase(EnemyBlueSlime* enemy) :owner_(enemy) {}
	virtual ~ActionBase() {}

	// ���s����(�������z�֐�)
	virtual ActionBase::State Run(const float elapsedTime) = 0;

protected:
	EnemyBlueSlime* owner_;
	int step_ = 0;

};
