#pragma once
#include "ActionBase.h"
//#include "EnemyBlueSlime.h"


// ë“ã@çsìÆ
class IdleAction : public ActionBase
{
public:
	template <class TemplateAI>
	IdleAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;

private:
	float circleRotation_ = 0.0f; // â~â^ìÆâÒì]íl

};


// í«ê’çsìÆ
class PursuitAction : public ActionBase
{
public:
template <class TemplateAI>
	PursuitAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// í«ê’çsìÆ
class CloseRangeAttackAction : public ActionBase
{
public:
	template <class TemplateAI>
	CloseRangeAttackAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// í èÌçUåÇ
class NormalAction : public ActionBase
{
public:
	template <class TemplateAI>
	NormalAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// ÉXÉLÉãçUåÇçsìÆ
class SkillAction : public ActionBase
{
public:
	template <class TemplateAI>
	SkillAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// úpújçsìÆ
class WanderAction : public ActionBase
{
public:
template <class TemplateAI>
	WanderAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// ì¶ëñçsìÆ
class LeaveAction : public ActionBase
{
public:
template <class TemplateAI>
	LeaveAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// âÒïúçsìÆ
class RecoverAction : public ActionBase
{
public:
template <class TemplateAI>
	RecoverAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};