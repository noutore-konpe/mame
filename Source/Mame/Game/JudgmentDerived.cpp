#include "JudgmentDerived.h"
#include "BaseEnemyAI.h"
#include "Player.h"
//#include "Mathf.h"


// BattleNode�ɑJ�ڂł��邩����
const bool BattleJudgment::Judgment()
{
	//// �v���C���[�������邩
	//if (owner_->SearchPlayer())
	//{
	//	return true;
	//}
	return false;
}

// AttackNode�ɑJ�ڂł��邩����
const bool AttackJudgment::Judgment()
{
	//// �ΏۂƂ̋������Z�o
	//DirectX::XMFLOAT3 position = owner_->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

	//float vx = targetPosition.x - position.x;
	//float vy = targetPosition.y - position.y;
	//float vz = targetPosition.z - position.z;
	//float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	//if (dist < owner_->GetAttackRange())
	//{
	//	// AttackNode�֑J�ڂł���
	//	return true;
	//}
	return false;
}

// SkillNode�ɑJ�ڂł��邩����
const bool SkillShotJudgment::Judgment()
{
	//// hp�������ȉ��̎�skill�����\
	//int health = owner_->GetHealth();
	//int baseHealth = static_cast<int>(owner_->GetMaxHealth() * 0.8);
	//if (health < baseHealth)
	//{
	//	// SkillNode�֑J�ڂł���
	//	return true;
	//}
	return false;
}

// WanderNode�ɑJ�ڂł��邩����
const bool WanderJudgment::Judgment()
{
	//// �ړI�n�_�܂ł�XZ���ʂł̋�������
	//DirectX::XMFLOAT3 position = owner_->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = owner_->GetTargetPosition();
	//float vx = targetPosition.x - position.x;
	//float vz = targetPosition.z - position.z;
	//float distSq = vx * vx + vz * vz;

	//// �ړI�n���痣��Ă���ꍇ
	//float radius = owner_->GetRadius();
	//if (distSq > radius * radius)
	//{
	//	return true;
	//}

	return false;
}

const bool EscapeJudgment::Judgment()
{
	//if (owner_->GetHealth() < (owner_->GetMaxHealth() * 0.5))
	//{
	//	return true;
	//}
	return false;
}