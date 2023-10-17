#pragma once
#include "ActionBase.h"
//#include "EnemyBlueSlime.h"


// �X�e�[�W�G���g���[�s��
class EntryStageAction : public ActionBase
{
public:
	template <class TemplateAI>
	EntryStageAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;

};

// �ҋ@�s��
class IdleAction : public ActionBase
{
public:
	template <class TemplateAI>
	IdleAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;

private:
	float circleRotation_ = 0.0f; // �~�^����]�l

};


// �ǐՍs��
class PursuitAction : public ActionBase
{
public:
template <class TemplateAI>
	PursuitAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// �ߐڍU���s��
class CloseRangeAttackAction : public ActionBase
{
public:
	template <class TemplateAI>
	CloseRangeAttackAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// �������U���s��
class LongRangeAttackAction : public ActionBase
{
public:
	template <class TemplateAI>
	LongRangeAttackAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// �Ђ�ݍs��
class FlinchAction : public ActionBase
{
public:
	template <class TemplateAI>
	FlinchAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;

};


// �ʏ�U��
class NormalAction : public ActionBase
{
public:
	template <class TemplateAI>
	NormalAction(TemplateAI* enemy) : ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// �X�L���U���s��
class SkillAction : public ActionBase
{
public:
	template <class TemplateAI>
	SkillAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// �p�j�s��
class WanderAction : public ActionBase
{
public:
template <class TemplateAI>
	WanderAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};


// �����s��
class LeaveAction : public ActionBase
{
public:
template <class TemplateAI>
	LeaveAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};

// �񕜍s��
class RecoverAction : public ActionBase
{
public:
template <class TemplateAI>
	RecoverAction(TemplateAI* enemy) :ActionBase(enemy) {}
	const ActionBase::State Run(const float elapsedTime) override;
};