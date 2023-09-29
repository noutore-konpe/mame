#pragma once
#include "JudgmentBase.h"
//#include "EnemyBlueSlime.h"

// BattleNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
template <class TemplateAI>
class BattleJudgment : public JudgmentBase
{
public:
	BattleJudgment(TemplateAI* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

// AttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
template <class TemplateAI>
class AttackJudgment : public JudgmentBase
{
public:
	AttackJudgment(TemplateAI* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

// SkillNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
template <class TemplateAI>
class SkillShotJudgment : public JudgmentBase
{
public:
	SkillShotJudgment(TemplateAI* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

// WanderNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
template <class TemplateAI>
class WanderJudgment : public JudgmentBase
{
public:
	WanderJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};


// Escape‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class EscapeJudgment : public JudgmentBase
{
public:
	EscapeJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};