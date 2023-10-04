#pragma once

//class EnemyBlueSlime;
class BaseEnemyAI;

// é¿çsîªíË
class JudgmentBase
{
public:
	template <class TemplateAI>
	JudgmentBase(TemplateAI* enemy) : owner_(enemy) {}
	virtual ~JudgmentBase() {}

	virtual const bool Judgment() = 0;

protected:
	BaseEnemyAI* owner_;

};