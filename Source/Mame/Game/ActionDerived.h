#pragma once
#include "ActionBase.h"
//#include "EnemyBlueSlime.h"

// �ʏ�U��
template <class TemplateAI>
class NormalAction : public ActionBase
{
public:
	NormalAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �X�L���U���s��
template <class TemplateAI>
class SkillAction : public ActionBase
{
public:
	SkillAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �ǐՍs��
template <class TemplateAI>
class PursuitAction : public ActionBase
{
public:
	PursuitAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �p�j�s��
template <class TemplateAI>
class WanderAction : public ActionBase
{
public:
	WanderAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �ҋ@�s��
template <class TemplateAI>
class IdleAction : public ActionBase
{
public:
	IdleAction(EnemyBlueSlime* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �����s��
template <class TemplateAI>
class LeaveAction : public ActionBase
{
public:
	LeaveAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �񕜍s��
template <class TemplateAI>
class RecoverAction : public ActionBase
{
public:
	RecoverAction(TemplateAI* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};