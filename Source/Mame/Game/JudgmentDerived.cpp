#include "JudgmentDerived.h"
#include "BaseEnemyAI.h"
#include "PlayerManager.h"
//#include "Mathf.h"


// BattleNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
const bool BattleJudgment::Judgment()
{
	//// ƒvƒŒƒCƒ„[‚ªŒ©‚Â‚©‚é‚©
	//if (owner_->SearchPlayer())
	//{
	//	return true;
	//}
	return false;
}

// AttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
const bool AttackJudgment::Judgment()
{
	//// ‘ÎÛ‚Æ‚Ì‹——£‚ðŽZo
	//DirectX::XMFLOAT3 position = owner_->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = Player::Instance().GetPosition();

	//float vx = targetPosition.x - position.x;
	//float vy = targetPosition.y - position.y;
	//float vz = targetPosition.z - position.z;
	//float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	//if (dist < owner_->GetAttackRange())
	//{
	//	// AttackNode‚Ö‘JˆÚ‚Å‚«‚é
	//	return true;
	//}
	return false;
}

// SkillNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
const bool SkillShotJudgment::Judgment()
{
	//// hp‚ª”¼•ªˆÈ‰º‚ÌŽžskill”­“®‰Â”\
	//int health = owner_->GetHealth();
	//int baseHealth = static_cast<int>(owner_->GetMaxHealth() * 0.8);
	//if (health < baseHealth)
	//{
	//	// SkillNode‚Ö‘JˆÚ‚Å‚«‚é
	//	return true;
	//}
	return false;
}

// WanderNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
const bool WanderJudgment::Judgment()
{
	//// –Ú“I’n“_‚Ü‚Å‚ÌXZ•½–Ê‚Å‚Ì‹——£”»’è
	//DirectX::XMFLOAT3 position = owner_->GetPosition();
	//DirectX::XMFLOAT3 targetPosition = owner_->GetTargetPosition();
	//float vx = targetPosition.x - position.x;
	//float vz = targetPosition.z - position.z;
	//float distSq = vx * vx + vz * vz;

	//// –Ú“I’n‚©‚ç—£‚ê‚Ä‚¢‚éê‡
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

const bool PursuitJudgment::Judgment()
{
	using DirectX::XMFLOAT3;

	PlayerManager& playerManager = PlayerManager::Instance();

	// ƒvƒŒƒCƒ„[‚Æ‚ÌXZ•½–Ê‚Å‚Ì‹——£”»’è
	const XMFLOAT3	position		= owner_->GetPosition();
	const XMFLOAT3	targetPosition	= playerManager.GetPlayer()->GetPosition();
	const float		vx				= targetPosition.x - position.x;
	const float		vz				= targetPosition.z - position.z;
	const float		lengthSq		= (vx * vx + vz * vz);

	// UŒ‚‹——£Œ—ŠO‚Ìê‡
	const float attackLength = owner_->GetAttackLength();
	if (lengthSq > attackLength * attackLength)
	{
		return true;
	}

	return false;
}
