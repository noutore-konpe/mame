#pragma once
#include "JudgmentBase.h"
//#include "EnemyBlueSlime.h"

// BattleNodeに遷移できるか判定
template <class TemplateAI>
class BattleJudgment : public JudgmentBase
{
public:
	BattleJudgment(TemplateAI* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// AttackNodeに遷移できるか判定
template <class TemplateAI>
class AttackJudgment : public JudgmentBase
{
public:
	AttackJudgment(TemplateAI* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// SkillNodeに遷移できるか判定
template <class TemplateAI>
class SkillShotJudgment : public JudgmentBase
{
public:
	SkillShotJudgment(TemplateAI* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// WanderNodeに遷移できるか判定
template <class TemplateAI>
class WanderJudgment : public JudgmentBase
{
public:
	WanderJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};


// Escapeに遷移できるか判定
class EscapeJudgment : public JudgmentBase
{
public:
	EscapeJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};