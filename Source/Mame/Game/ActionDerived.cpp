#include "ActionDerived.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"

//#include "EnemyBlueSlime.h"
#include "BaseEnemyAI.h"
#include "PlayerManager.h"
//#include "Mathf.h"



// �U���s��
const ActionBase::State NormalAction::Run(const float elapsedTime)
{
	//switch (step_)
	//{
	//case 0:
	//	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	//	owner_->SetTargetPosition(Player::Instance().GetPosition());
	//	// �A�j���[�V�����Đ�
	//	owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::Attack01), false);
	//	step_++;
	//	break;
	//case 1:

	//	// �A�j���[�V�������I�����Ă���Ƃ�
	//	if (!owner_->GetModel()->IsPlayAnimation())
	//	{
	//		step_ = 0;
	//		// �U��������Ԃ�
	//		return ActionBase::State::Complete;
	//	}
	//	break;
	//}
	//// �X�L������Ԃ�
	return ActionBase::State::Run;
}

// �X�L���U���s��
const ActionBase::State SkillAction::Run(const float elapsedTime)
{
	//switch (step_)
	//{
	//	case 0:
	//		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	//		owner_->SetTargetPosition(Player::Instance().GetPosition());
	//		// �A�j���[�V�����Đ�
	//		owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::Attack02), false);
	//		step_++;
	//	case 1:
	//		// �A�j���[�V�������I�����Ă���Ƃ�
	//		if (!owner_->GetModel()->IsPlayAnimation())
	//		{
	//			step_ = 0;
	//			// �U��������Ԃ�
	//			return ActionBase::State::Complete;
	//		}
	//		break;
	//}
	//// �X�L������Ԃ�
	return ActionBase::State::Run;
}

// �p�j�s��
const ActionBase::State WanderAction::Run(const float elapsedTime)
{
	//switch (step_)
	//{
	//case 0:
	//	// �p�j���[�V�����ݒ�
	//	owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::WalkFWD), true);
	//	step_++;
	//	break;
	//case 1:
	//	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	//	DirectX::XMFLOAT3 position = owner_->GetPosition();
	//	DirectX::XMFLOAT3 targetPosition = owner_->GetTargetPosition();
	//	float vx = targetPosition.x - position.x;
	//	float vz = targetPosition.z - position.z;
	//	float distSq = vx * vx + vz * vz;

	//	// �ړI�n�֒�����
	//	float radius = owner_->GetRadius();
	//	if (distSq < radius * radius)
	//	{
	//		step_ = 0;
	//		// �p�j������Ԃ�
	//		return ActionBase::State::Complete;
	//	}

	//	// �ړI�n�_�ֈړ�
	//	owner_->MoveToTarget(elapsedTime, 0.5f);

	//	// �v���C���[���G����������
	//	if (owner_->SearchPlayer())
	//	{
	//		step_ = 0;
	//		// �p�j������Ԃ�
	//		return ActionBase::State::Complete;
	//	}
	//	break;
	//}
	//// ���s��
	return ActionBase::State::Run;
}

// �ǐՍs��
const ActionBase::State PursuitAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;

	//float runTimer = owner_->GetRunTimer();
	switch (step_)
	{
	case 0:
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		//owner_->SetTargetPosition(PlayerManager::Instance().GetPosition());
		//owner_->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		//owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::RunFWD), true);
		++step_;
		[[fallthrough]];
		//break;
	case 1:
		//runTimer -= elapsedTime;
		//// �^�C�}�[�X�V
		//owner_->SetRunTimer(runTimer);
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		const XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer()->GetPosition();
		owner_->SetTargetPosition(targetPosition);
		// �ړI�n�_�ֈړ�
		owner_->MoveToTarget(elapsedTime, 1.0);

		// �v���C���[�Ƃ̋������v�Z
		//const XMFLOAT3 position = owner_->GetTransform()->GetPosition();
		//PlayerManager& playerManager = PlayerManager::Instance();
		//const XMFLOAT3 targetPosition = playerManager.GetPlayer()->GetTransform()->GetPosition();

		//const float dist = sqrtf(vx * vx + vy * vy + vz * vz);
		//// �U���͈͂ɂ���Ƃ�
		//if (dist < owner_->GetAttackRange())
		//{
		//	step_ = 0;
		//	// �ǐՐ�����Ԃ�
		//	return ActionBase::State::Complete;
		//}
		const XMFLOAT3 vec = targetPosition - owner_->GetPosition();
		const float distSq = XMFloat3LengthSq(vec);
		// �U���͈͂ɂ���Ƃ�
		constexpr float distlimit = 5.0f;
		if (distSq < distlimit * distlimit)
		{
			step_ = 0;
			// �ǐՐ�����Ԃ�
			return ActionBase::State::Complete;
		}

		//// �s�����Ԃ��߂�����
		//if (runTimer <= 0.0f)
		//{
		//	step_ = 0;
		//	// �ǐՎ��s��Ԃ�
		//	return ActionBase::State::Failed;
		//}
		break;
	}
	return ActionBase::State::Run;
}

// �ҋ@�s��
const ActionBase::State IdleAction::Run(const float elapsedTime)
{
	//float runTimer = owner_->GetRunTimer();
	//switch (step_)
	//{
	//case 0:
	//	owner_->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
	//	owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::IdleNormal), true);
	//	step_++;
	//	break;
	//case 1:
	//	runTimer -= elapsedTime;
	//	// �^�C�}�[�X�V
	//	owner_->SetRunTimer(runTimer);

	//	// �ҋ@���Ԃ��߂�����
	//	if (runTimer <= 0.0f)
	//	{
	//		owner_->SetRandomTargetPosition();
	//		step_ = 0;
	//		return ActionBase::State::Complete;
	//	}

	//	// �v���C���[����������
	//	if (owner_->SearchPlayer())
	//	{
	//		step_ = 0;
	//		return ActionBase::State::Complete;
	//	}
	//	break;
	//}
	return ActionBase::State::Run;
}

// �����s��
const ActionBase::State LeaveAction::Run(const float elapsedTime)
{
	//DirectX::XMFLOAT3 targetPosition;
	//switch (step_)
	//{
	//case 0:
	//	// �ڕW�n�_���v���C���[�Ɛ����΂̃x�N�g���~5�̈ʒu�Ɏw��
	//	DirectX::XMVECTOR startPosition = DirectX::XMLoadFloat3(&Player::Instance().GetPosition());
	//	DirectX::XMVECTOR endPosition = DirectX::XMLoadFloat3(&owner_->GetPosition());

	//	DirectX::XMVECTOR TargetPosition = DirectX::XMVectorSubtract(endPosition, startPosition);
	//	TargetPosition = DirectX::XMVector3Normalize(TargetPosition);
	//	TargetPosition = DirectX::XMVectorScale(TargetPosition, 5.0f);


	//	DirectX::XMStoreFloat3(&targetPosition, TargetPosition);
	//	targetPosition.x += owner_->GetPosition().x;
	//	targetPosition.y += owner_->GetPosition().y;
	//	targetPosition.z += owner_->GetPosition().z;
	//	owner_->SetTargetPosition(targetPosition);

	//	owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::RunFWD), true);
	//	step_++;
	//	break;
	//case 1:

	//	targetPosition = owner_->GetTargetPosition();
	//	// �ړI�n�_�ֈړ�
	//	owner_->MoveToTarget(elapsedTime, 1.0);

	//	DirectX::XMFLOAT3 position = owner_->GetPosition();
	//	targetPosition = owner_->GetTargetPosition();

	//	float vx = targetPosition.x - position.x;
	//	float vz = targetPosition.z - position.z;
	//	float distSq = vx * vx + vz * vz;

	//	// �ړI�n�֒�����
	//	float radius = owner_->GetRadius();
	//	if (distSq < radius * radius)
	//	{
	//		step_ = 0;
	//		return ActionBase::State::Complete;
	//	}

	//	break;
	//}

	return ActionBase::State::Run;
}

// �񕜍s��
const ActionBase::State RecoverAction::Run(const float elapsedTime)
{
	//switch (step_)
	//{
	//case 0:
	//	owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::Taunt), false);
	//	step_++;
	//	break;
	//case 1:
	//	if (!owner_->GetModel()->IsPlayAnimation())
	//	{
	//		owner_->SetHealth(owner_->GetMaxHealth());
	//		step_ = 0;
	//		return ActionBase::State::Complete;
	//	}
	//	break;
	//}

	return ActionBase::State::Run;
}