#include "ActionDerived.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../../Taki174/Common.h"

#include "BaseEnemyAI.h"
#include "EnemyManager.h"
#include "PlayerManager.h"


// �ҋ@�s��
const ActionBase::State IdleAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using DirectX::XMFLOAT4;

	PlayerManager& playerManager = PlayerManager::Instance();
	EnemyManager& enemyManager = EnemyManager::Instance();

	switch (step_)
	{
	case 0:
		owner_->SetRunTimer(::RandFloat(0.5f, 1.0f));
		//owner_->SetRunTimer(60.0f);
		//owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::IdleNormal), true);

		// �����̈ʒu�����]���n�߂�悤�ɂ���
		{
			const XMFLOAT3	position		= owner_->GetPosition();
			const XMFLOAT3	playerPosition	= playerManager.GetPlayer()->GetPosition();
			const float		localPositionX	= position.x - playerPosition.x;
			const float		localPositionZ	= position.z - playerPosition.z;

			circleRotation_ = ::atan2f(localPositionX, localPositionZ); // return Radian Angle
		}

		++step_;
		[[fallthrough]];
	//	break;
	case 1:
		// �^�C�}�[�X�V
		owner_->ElapseRunTimer(elapsedTime);

		//// �N���ߐڍU���s�����łȂ��ߐڍU���s���N�[���^�C�}�[���Ȃ���ΏI��
		//if (false == enemyManager.GetIsRunningCRAAction() &&
		//	enemyManager.GetCRAActionCoolTimer() <= 0.0f)
		//{
		//	step_ = 0;
		//	return ActionBase::State::Complete;
		//}

		// �ҋ@���Ԃ��߂�����I��
		if (owner_->GetRunTimer() <= 0.0f)
		{
			step_ = 0;
			return ActionBase::State::Complete;
		}

		const XMFLOAT3	position		= owner_->GetPosition();
		const XMFLOAT3	playerPosition	= playerManager.GetPlayer()->GetPosition();
		const float		vx				= playerPosition.x - position.x;
		const float		vz				= playerPosition.z - position.z;
		const float		lengthSq		= (vx * vx + vz * vz);
		const float		attackLength	= owner_->GetAttackLength();
		const float		attackLengthSq	= attackLength * attackLength;

		// �U�������O�ɂȂ�����I��(�~�^���̓s����Â߂ɐݒ�)
		if (lengthSq > (attackLengthSq + 5.0f))
		{
			step_ = 0;
			return ActionBase::State::Complete;
		}

		// �~�^�����ǂ�
		{
			// ���E����
			const float playerRotationY = playerManager.GetPlayer()->GetTransform()->GetRotation().y;
			const float frontX = ::sinf(playerRotationY);
			const float frontZ = ::cosf(playerRotationY);
			float cross = (frontZ * (-vx)) - (frontX * (-vz));

#if 0
#ifdef (USE_IMGUI && _DEBUG)
			ImGui::Begin("Cross");
			ImGui::DragFloat("Cross", &cross);
			ImGui::End();
#endif
#endif

			// �w��ɉ��؂�����I��
			if (fabsf(cross) <= 0.01f)
			{
				step_ = 0;
				return ActionBase::State::Complete;
			}

			const float addRotate = (cross < 0.0f) ? ToRadian(-25.0f) : ToRadian(25.0f);

			// �v���C���[�𒆐S��(+1.0~-1.0)�����a(=�U������)������]
			circleRotation_ += addRotate * elapsedTime;

			const XMFLOAT3 targetPosition = {
				playerPosition.x + ::sinf(circleRotation_) * attackLength, // -1.0~+1.0
				0.0f,
				playerPosition.z + ::cosf(circleRotation_) * attackLength, // -1.0~+1.0
			};

			owner_->SetTargetPosition(targetPosition);
			owner_->MoveToTarget(elapsedTime, 1.0f, false);
		}

		// ��]
		owner_->Turn(elapsedTime, vx, vz, owner_->GetTurnSpeed());

	}

	return ActionBase::State::Run;
}


// �ǐՍs��
const ActionBase::State PursuitAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;

	PlayerManager& playerManager = PlayerManager::Instance();

	//float runTimer = owner_->GetRunTimer();
	switch (step_)
	{
	case 0:
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner_->SetTargetPosition(playerManager.GetPlayer()->GetPosition());
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
		owner_->SetTargetPosition(playerManager.GetPlayer()->GetPosition());

		// �v���C���[�Ƃ�XZ���ʂł̋�������
		const XMFLOAT3	position = owner_->GetPosition();
		const XMFLOAT3	targetPosition = owner_->GetTargetPosition();
		const float		vx = targetPosition.x - position.x;
		const float		vz = targetPosition.z - position.z;
		const float		lengthSq = (vx * vx + vz * vz);

		// �U�����������܂Őڋ߂�����ǐՐ�����Ԃ�
		const float		attackLength = owner_->GetAttackLength();
		if (lengthSq <= attackLength * attackLength)
		{
			step_ = 0;
			return ActionBase::State::Complete;
		}
		// �ڕW�n�_�ֈړ�
		else
		{
			owner_->MoveToTarget(elapsedTime, 1.0);
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


const ActionBase::State CloseRangeAttackAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;

	PlayerManager&	playerManager	= PlayerManager::Instance();
	EnemyManager&	enemyManager	= EnemyManager::Instance();

	//float runTimer = owner_->GetRunTimer();
	switch (step_)
	{
	case 0:
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner_->SetTargetPosition(playerManager.GetPlayer()->GetPosition());
		owner_->SetRunTimer(::RandFloat(+1.0f, +1.0f));
		//owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::RunFWD), true);

		++step_;
		[[fallthrough]];
		//break;
	case 1:
		//runTimer -= elapsedTime;
		//owner_->SetRunTimer(runTimer);
		// �^�C�}�[�X�V
		owner_->ElapseRunTimer(elapsedTime);

		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner_->SetTargetPosition(playerManager.GetPlayer()->GetPosition());

		// �s�����Ă���̂��킩��₷���悤�ɉ��ŉ�]������
		owner_->GetTransform()->AddRotationY(ToRadian(1080.0f) * elapsedTime);

		// �s�����Ԃ��߂�����
		if (owner_->GetRunTimer() <= 0.0f)
		{
			step_ = 0;

			// CRA : 5.Action : �ߐڍU���s�����s���t���O�����낷
			enemyManager.SetIsRunningCRAAction(false);

			// CRA : 6.Action : �ߐڍU���s���N�[���^�C�}�[�ݒ�
			constexpr float craCoolTime = 0.0f;
			enemyManager.SetCRAActionCoolTimer(craCoolTime);

			//return ActionBase::State::Failed;
			return ActionBase::State::Complete;
		}

		break;
	}

	return ActionBase::State::Run;
}


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


