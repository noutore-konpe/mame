#include "JudgmentDerived.h"
#include "BaseEnemyAI.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

// ステージエントリー判定関数
const bool EntryStageJudgment::Judgment()
{
	// ステージにすでに入っていたらfasleを返す
	if (true == owner_->GetEntryStageFlag()) { return false; }

	return true;
}

const bool PursuitJudgment::Judgment()
{
	using DirectX::XMFLOAT3;

	PlayerManager& playerManager = PlayerManager::Instance();

	// プレイヤーとのXZ平面での距離判定
	const XMFLOAT3&	position = owner_->GetPosition();
	const XMFLOAT3&	targetPosition = playerManager.GetPlayer()->GetPosition();
	const float		vx = targetPosition.x - position.x;
	const float		vz = targetPosition.z - position.z;
	const float		lengthSq = (vx * vx + vz * vz);

	// 攻撃距離圏内ならfalse
	const float attackLength = owner_->GetAttackLength();
	if (lengthSq <= attackLength * attackLength)
	{
		return false;
	}

	return true;
}


const bool CloseRangeAttackJudgment::Judgment()
{
	using DirectX::XMFLOAT3;

	PlayerManager&	playerManager	= PlayerManager::Instance();
	EnemyManager&	enemyManager	= EnemyManager::Instance();

	// CRA : 2.Judgment : 他の敵が近接攻撃行動中ならfalse
	if (true == enemyManager.GetIsRunningCRAAction())
	{
		return false;
	}

	// CRA : 3.Judgment : 近接攻撃行動クールタイムが終わっていなければfalse
	if (enemyManager.GetCRAActionCoolTimer() > 0.0f)
	{
		return false;
	}

	// プレイヤーとのXZ平面での距離判定
	const XMFLOAT3&	pos			= owner_->GetPosition();
	const XMFLOAT3&	targetPos	= playerManager.GetPlayer()->GetPosition();
	const float		vx			= targetPos.x - pos.x;
	const float		vz			= targetPos.z - pos.z;
	const float		lengthSq	= (vx * vx + vz * vz);

	// 攻撃距離圏外ならfalse
	const float attackLength = owner_->GetAttackLength();
	if (lengthSq > attackLength * attackLength)
	{
		return false;
	}

	// 他の敵の方がプレイヤーに近ければfalse
	const size_t enemyCount = enemyManager.GetEnemyCount();
	for (size_t i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		// 自分ならconinue(※誤代入防止キャスト)
		if (static_cast<const Enemy*>(enemy) == owner_) continue;

		// 他の敵が子ノードを持っていなければcontinue
		if (enemy->GetChildNodeCount() <= 0) continue;

		// 他の敵が近接攻撃ノードを持っていなければcontinue
		if (false == enemy->IsExistChildNode("CloseRangeAttack")) continue;

		const XMFLOAT3&	otherPos		= enemy->GetPosition();
		const float		otherVx			= targetPos.x - otherPos.x;
		const float		otherVz			= targetPos.z - otherPos.z;
		const float		otherLengthSq	= (otherVx * otherVx + otherVz * otherVz);

		if (lengthSq > otherLengthSq) return false;
	}

	return true;
}


const bool LongRangeAttackJudgment::Judgment()
{
	using DirectX::XMFLOAT3;

	PlayerManager& playerManager = PlayerManager::Instance();

	// プレイヤーとのXZ平面での距離判定
	const XMFLOAT3&	position = owner_->GetPosition();
	const XMFLOAT3&	targetPosition = playerManager.GetPlayer()->GetPosition();
	const float		vx = targetPosition.x - position.x;
	const float		vz = targetPosition.z - position.z;
	const float		lengthSq = (vx * vx + vz * vz);

	// 攻撃距離圏外ならfalse
	const float attackLength = owner_->GetAttackLength();
	if (lengthSq > attackLength * attackLength)
	{
		return false;
	}

	// カメラ外なら遠距離攻撃をしない
	bool isInCamera = false;
	Sprite::ConvertToScreenPos(position, &isInCamera);
	if (false == isInCamera) { return false; }

	return true;
}

// ひるみ判定関数
const bool FlinchJudgment::Judgment()
{
	// ひるみフラグが立っていなければfalse
	if (false == owner_->GetIsFlinch()) return false;

	return true;
}

// 吹っ飛び判定関数
const bool BlowOffJudgment::Judgment()
{
	// 吹っ飛びフラグが立っていなければfalse
	if (false == owner_->GetIsBlowOff()) return false;

	return true;
}



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
