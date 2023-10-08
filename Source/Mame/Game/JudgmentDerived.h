#pragma once
#include "JudgmentBase.h"
//#include "EnemyBlueSlime.h"

class PursuitJudgment : public JudgmentBase
{
public:
	template <class TemplateAI>
	PursuitJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	const bool Judgment() override;
};


class CloseRangeAttackJudgment : public JudgmentBase
{
public:
	template <class TemplateAI>
	CloseRangeAttackJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	const bool Judgment() override;
};


class LongRangeAttackJudgment : public JudgmentBase
{
public:
	template <class TemplateAI>
	LongRangeAttackJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	const bool Judgment() override;
};


// BattleNode�ɑJ�ڂł��邩����
class BattleJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	BattleJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ����
	const bool Judgment() override;
};

// AttackNode�ɑJ�ڂł��邩����
class AttackJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	AttackJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ����
	const bool Judgment() override;
};

// SkillNode�ɑJ�ڂł��邩����
class SkillShotJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	SkillShotJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ����
	const bool Judgment() override;
};


// WanderNode�ɑJ�ڂł��邩����
class WanderJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	WanderJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ����
	const bool Judgment() override;
};

// Escape�ɑJ�ڂł��邩����
class EscapeJudgment : public JudgmentBase
{
public:
	template <class TemplateAI>
	EscapeJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ����
	const bool Judgment() override;
};