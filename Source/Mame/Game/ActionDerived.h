#pragma once
#include "ActionBase.h"
//#include "EnemyBlueSlime.h"

// 通常攻撃
template <class TemplateAI>
class NormalAction : public ActionBase
{
public:
	NormalAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// スキル攻撃行動
template <class TemplateAI>
class SkillAction : public ActionBase
{
public:
	SkillAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 追跡行動
template <class TemplateAI>
class PursuitAction : public ActionBase
{
public:
	PursuitAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 徘徊行動
template <class TemplateAI>
class WanderAction : public ActionBase
{
public:
	WanderAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 待機行動
template <class TemplateAI>
class IdleAction : public ActionBase
{
public:
	IdleAction(EnemyBlueSlime* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 逃走行動
template <class TemplateAI>
class LeaveAction : public ActionBase
{
public:
	LeaveAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 回復行動
template <class TemplateAI>
class RecoverAction : public ActionBase
{
public:
	RecoverAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};