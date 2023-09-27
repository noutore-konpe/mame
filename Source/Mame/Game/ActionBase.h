#pragma once

class EnemyBlueSlime;

// �s���������N���X
class ActionBase
{
public:
	ActionBase(EnemyBlueSlime* enemy):owner(enemy){}
	// ���s���
	enum class State
	{
		Run,		// ���s��
		Failed,		// ���s���s
		Complete,	// ���s����
	};

	// ���s����(�������z�֐�)
	virtual ActionBase::State Run(float elapsedTime) = 0;
protected:
	EnemyBlueSlime* owner;
	int step = 0;
};
