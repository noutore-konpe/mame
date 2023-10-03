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

// BattleNodeに遷移できるか判定
class BattleJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	BattleJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// 判定
	const bool Judgment() override;
};

// AttackNodeに遷移できるか判定
class AttackJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	AttackJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// 判定
	const bool Judgment() override;
};

// SkillNodeに遷移できるか判定
class SkillShotJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	SkillShotJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// 判定
	const bool Judgment() override;
};


// WanderNodeに遷移できるか判定
class WanderJudgment : public JudgmentBase
{
public:
template <class TemplateAI>
	WanderJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// 判定
	const bool Judgment() override;
};

// Escapeに遷移できるか判定
class EscapeJudgment : public JudgmentBase
{
public:
	template <class TemplateAI>
	EscapeJudgment(TemplateAI* enemy) : JudgmentBase(enemy) {}
	// 判定
	const bool Judgment() override;
};