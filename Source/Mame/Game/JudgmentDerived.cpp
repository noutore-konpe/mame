#include "JudgmentDerived.h"
#include "BaseEnemyAI.h"
#include "Player.h"
//#include "Mathf.h"


// BattleNodeに遷移できるか判定
const bool BattleJudgment::Judgment()
{
	//// プレイヤーが見つかるか
	//if (owner_->SearchPlayer())
	//{
	//	return true;
	//}
	return false;
}

// AttackNodeに遷移できるか判定
const bool AttackJudgment::Judgment()
{
	//// 対象との距離を算出
	//DirectX::XMFLOAT3 position = owner_->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

	//float vx = targetPosition.x - position.x;
	//float vy = targetPosition.y - position.y;
	//float vz = targetPosition.z - position.z;
	//float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	//if (dist < owner_->GetAttackRange())
	//{
	//	// AttackNodeへ遷移できる
	//	return true;
	//}
	return false;
}

// SkillNodeに遷移できるか判定
const bool SkillShotJudgment::Judgment()
{
	//// hpが半分以下の時skill発動可能
	//int health = owner_->GetHealth();
	//int baseHealth = static_cast<int>(owner_->GetMaxHealth() * 0.8);
	//if (health < baseHealth)
	//{
	//	// SkillNodeへ遷移できる
	//	return true;
	//}
	return false;
}

// WanderNodeに遷移できるか判定
const bool WanderJudgment::Judgment()
{
	//// 目的地点までのXZ平面での距離判定
	//DirectX::XMFLOAT3 position = owner_->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = owner_->GetTargetPosition();
	//float vx = targetPosition.x - position.x;
	//float vz = targetPosition.z - position.z;
	//float distSq = vx * vx + vz * vz;

	//// 目的地から離れている場合
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