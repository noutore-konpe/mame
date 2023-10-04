#pragma once
#include "ActionBase.h"
//#include "EnemyBlueSlime.h"


// 待機行動
class IdleAction : public ActionBase
{
public:
	template <class TemplateAI>
	IdleAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// 追跡行動
class PursuitAction : public ActionBase
{
public:
template <class TemplateAI>
	PursuitAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// 追跡行動
class CloseRangeAttackAction : public ActionBase
{
public:
	template <class TemplateAI>
	CloseRangeAttackAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// 通常攻撃
class NormalAction : public ActionBase
{
public:
	template <class TemplateAI>
	NormalAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// スキル攻撃行動
class SkillAction : public ActionBase
{
public:
	template <class TemplateAI>
	SkillAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// 徘徊行動
class WanderAction : public ActionBase
{
public:
template <class TemplateAI>
	WanderAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// 逃走行動
class LeaveAction : public ActionBase
{
public:
template <class TemplateAI>
	LeaveAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// 回復行動
class RecoverAction : public ActionBase
{
public:
template <class TemplateAI>
	RecoverAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};