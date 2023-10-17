#pragma once
#include "ActionBase.h"
//#include "EnemyBlueSlime.h"


// ステージエントリー行動
class EntryStageAction : public ActionBase
{
public:
	template <class TemplateAI>
	EntryStageAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;

};

// 待機行動
class IdleAction : public ActionBase
{
public:
	template <class TemplateAI>
	IdleAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;

private:
	float circleRotation_ = 0.0f; // 円運動回転値

};


// 追跡行動
class PursuitAction : public ActionBase
{
public:
template <class TemplateAI>
	PursuitAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// 近接攻撃行動
class CloseRangeAttackAction : public ActionBase
{
public:
	template <class TemplateAI>
	CloseRangeAttackAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// 遠距離攻撃行動
class LongRangeAttackAction : public ActionBase
{
public:
	template <class TemplateAI>
	LongRangeAttackAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// ひるみ行動
class FlinchAction : public ActionBase
{
public:
	template <class TemplateAI>
	FlinchAction(TemplateAI* enemy) : ActionBase(enemy) {}
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