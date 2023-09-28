#pragma once

//class EnemyBlueSlime;
class BaseCharacterAI;

// ���s����
class JudgmentBase
{
public:
	template <typename TemplateAI>
	JudgmentBase(TemplateAI* enemy) :owner_(enemy) {}
	virtual ~JudgmentBase() {}

	virtual bool Judgment() = 0;

protected:
	BaseCharacterAI* owner_;

};