#pragma once

//class EnemyBlueSlime;


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
	ActionBase(TemplateAI* enemy) :owner_(enemy) {}
	virtual ~ActionBase() {}

	// ���s����(�������z�֐�)
	virtual const ActionBase::State Run(const float elapsedTime) = 0;

protected:
	BaseCharacterAI* owner_;

	int step_ = 0;

};
