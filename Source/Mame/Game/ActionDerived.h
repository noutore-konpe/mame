#pragma once
#include "ActionBase.h"
//#include "EnemyBlueSlime.h"


// ‘Ò‹@s“®
class IdleAction : public ActionBase
{
public:
	template <class TemplateAI>
	IdleAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;

private:
	float circleRotation_ = 0.0f; // ‰~‰^“®‰ñ“]’l

};


// ’ÇÕs“®
class PursuitAction : public ActionBase
{
public:
template <class TemplateAI>
	PursuitAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// ‹ßÚUŒ‚s“®
class CloseRangeAttackAction : public ActionBase
{
public:
	template <class TemplateAI>
	CloseRangeAttackAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// ‰“‹——£UŒ‚s“®
class LongRangeAttackAction : public ActionBase
{
public:
	template <class TemplateAI>
	LongRangeAttackAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};



// ’ÊíUŒ‚
class NormalAction : public ActionBase
{
public:
	template <class TemplateAI>
	NormalAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// ƒXƒLƒ‹UŒ‚s“®
class SkillAction : public ActionBase
{
public:
	template <class TemplateAI>
	SkillAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// œpœjs“®
class WanderAction : public ActionBase
{
public:
template <class TemplateAI>
	WanderAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// “¦‘–s“®
class LeaveAction : public ActionBase
{
public:
template <class TemplateAI>
	LeaveAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// ‰ñ•œs“®
class RecoverAction : public ActionBase
{
public:
template <class TemplateAI>
	RecoverAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};