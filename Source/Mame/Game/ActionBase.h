#pragma once

//class EnemyBlueSlime;
class BaseEnemyAI;

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
	template <class TemplateAI>
	ActionBase(TemplateAI* enemy) : owner_(enemy) {}
	virtual ~ActionBase() {}

	// ���s����(�������z�֐�)
	virtual const ActionBase::State Run(const float elapsedTime) = 0;

protected:
	BaseEnemyAI* owner_ = nullptr;

	int step_ = 0;

};
