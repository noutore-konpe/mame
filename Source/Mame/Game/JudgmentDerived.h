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


// BattleNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class BattleJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	BattleJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	const bool Judgment() override;
};

// AttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class AttackJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	AttackJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	const bool Judgment() override;
};

// SkillNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class SkillShotJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	SkillShotJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	const bool Judgment() override;
};


// WanderNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class WanderJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	WanderJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	const bool Judgment() override;
};

// Escape‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class EscapeJudgment : public JudgmentBase
{
public:
	template <class TemplateAI>
	EscapeJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	const bool Judgment() override;
};