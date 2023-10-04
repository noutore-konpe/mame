#include "JudgmentDerived.h"
#include "BaseEnemyAI.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
//#include "Mathf.h"


const bool CloseRangeAttackJudgment::Judgment()
{
	using DirectX::XMFLOAT3;

	PlayerManager&	playerManager	= PlayerManager::Instance();
	EnemyManager&	enemyManager	= EnemyManager::Instance();

	// CRA : 2.Judgment : ���̓G���ߐڍU���s�����Ȃ�false
	if (true == enemyManager.GetIsRunningCRAAction())
	{
		return false;
	}

	// CRA : 3.Judgment : �ߐڍU���s���N�[���^�C�����I����Ă��Ȃ����false
	if (enemyManager.GetCRAActionCoolTimer() > 0.0f)
	{
		return false;
	}

	// �v���C���[�Ƃ�XZ���ʂł̋�������
	const XMFLOAT3	position		= owner_->GetPosition();
	const XMFLOAT3	targetPosition	= playerManager.GetPlayer()->GetPosition();
	const float		vx				= targetPosition.x - position.x;
	const float		vz				= targetPosition.z - position.z;
	const float		lengthSq		= (vx * vx + vz * vz);

	// �U���������O�Ȃ�false
	const float attackLength = owner_->GetAttackLength();
	if (lengthSq > attackLength * attackLength)
	{
		return false;
	}

	// ���̓G�̕����v���C���[�ɋ߂����false
	const size_t enemyCount = enemyManager.GetEnemyCount();
	for (size_t i = 0; i < enemyCount; ++i)
	{
		BaseEnemyAI* enemy = enemyManager.GetEnemy(i);

		if (enemy != owner_)
		{
			const XMFLOAT3	otherPosition	= enemy->GetPosition();
			const float		otherVx			= targetPosition.x - otherPosition.x;
			const float		otherVz			= targetPosition.z - otherPosition.z;
			const float		otherLengthSq	= (otherVx * otherVx + otherVz * otherVz);

			if (lengthSq > otherLengthSq) return false;
		}
	}

	return true;
}

const bool PursuitJudgment::Judgment()
{
	using DirectX::XMFLOAT3;

	PlayerManager& playerManager = PlayerManager::Instance();

	// �v���C���[�Ƃ�XZ���ʂł̋�������
	const XMFLOAT3	position = owner_->GetPosition();
	const XMFLOAT3	targetPosition = playerManager.GetPlayer()->GetPosition();
	const float		vx = targetPosition.x - position.x;
	const float		vz = targetPosition.z - position.z;
	const float		lengthSq = (vx * vx + vz * vz);

	// �U�����������Ȃ�false
	const float attackLength = owner_->GetAttackLength();
	if (lengthSq <= attackLength * attackLength)
	{
		return false;
	}

	return true;
}


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
