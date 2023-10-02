#pragma once
#include "JudgmentBase.h"
//#include "EnemyBlueSlime.h"

// BattleNode�ɑJ�ڂł��邩����
template <class TemplateAI>
class BattleJudgment : public JudgmentBase
{
public:
	BattleJudgment(TemplateAI* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// AttackNode�ɑJ�ڂł��邩����
template <class TemplateAI>
class AttackJudgment : public JudgmentBase
{
public:
	AttackJudgment(TemplateAI* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// SkillNode�ɑJ�ڂł��邩����
template <class TemplateAI>
class SkillShotJudgment : public JudgmentBase
{
public:
	SkillShotJudgment(TemplateAI* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// WanderNode�ɑJ�ڂł��邩����
template <class TemplateAI>
class WanderJudgment : public JudgmentBase
{
public:
	WanderJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};


// Escape�ɑJ�ڂł��邩����
class EscapeJudgment : public JudgmentBase
{
public:
	EscapeJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};