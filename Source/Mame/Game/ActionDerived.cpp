#include "ActionDerived.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../../Taki174/Common.h"

#include "../Scene/SceneGame.h"

#include "BaseEnemyAI.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "ProjectileStraight.h"

// �X�e�[�W�G���g���[�s��
const ActionBase::State EntryStageAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using Animation = Player::Animation;

	switch (owner_->GetStep())
	{
	case 0:
		// ���݂̈ʒu����X�e�[�W���S�Ɍ������Ă�����x�i�񂾈ʒu��ڕW�n�_�ɐݒ�
		{
			const XMFLOAT3& pos  = owner_->GetTransform()->GetPosition();
			const XMFLOAT3  vecN = ::XMFloat3Normalize(SceneGame::stageCenter - pos);
			static constexpr float moveLength = 19.0f;	// �o��������X�e�[�W���炢�܂ł̋���
			owner_->SetTargetPosition(pos + vecN * moveLength);
		}

		// �A�j���[�V�����Đ�
		{
			owner_->PlayBlendAnimation(
				Animation::Idle, Animation::Dash,
				true, owner_->GetAnimationSpeed()
			);

			// ���̃A�j���[�V�����Đ�
			Model* sword = owner_->GetSword();
			if (sword != nullptr)
			{
				sword->PlayBlendAnimation(
					Animation::Idle, Animation::Dash,
					true, owner_->GetAnimationSpeed()
				);
			}
		}

		owner_->SetStep(1);
		[[fallthrough]];
		//break;
	case 1:
		// �ڕW�n�_�ֈړ�
		owner_->MoveToTarget(elapsedTime, 1.0);

		// �ڕW�n�_�Ƃ�XZ���ʂł̋�������
		const XMFLOAT3&	pos		  = owner_->GetPosition();
		const XMFLOAT3&	targetPos = owner_->GetTargetPosition();
		const float		vx = targetPos.x - pos.x;
		const float		vz = targetPos.z - pos.z;
		const float		lengthXZSq = (vx * vx + vz * vz);

		// �ڕW�n�_�ɓ��B�����琬���I������
		static constexpr float acceptableLength = 0.1f; // ���e���鋗��
		if (lengthXZSq <= acceptableLength * acceptableLength)
		{
			owner_->SetEntryStageFlag(true); // �X�e�[�W�ɓ�����

			owner_->SetStep(0);
			return ActionBase::State::Complete;
		}

		break;
	}

	return ActionBase::State::Run;
}


// �ҋ@�s��
const ActionBase::State IdleAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using DirectX::XMFLOAT4;
	using Animation = Player::Animation;

	PlayerManager& plManager = PlayerManager::Instance();

	switch (owner_->GetStep())
	{
	case 0:
		owner_->SetRunTimer(::RandFloat(0.5f, 1.5f));
		//owner_->SetRunTimer(60.0f);

		// �����̈ʒu�����]���n�߂�悤�ɂ���
		{
			const XMFLOAT3&	pos			= owner_->GetPosition();
			const XMFLOAT3&	plPos		= plManager.GetPlayer()->GetPosition();
			const float		localPosX	= pos.x - plPos.x;
			const float		localPosZ	= pos.z - plPos.z;

			circleRotation_ = ::atan2f(localPosX, localPosZ); // return Radian Angle
		}

		// �A�j���[�V�����Đ�
		{
			owner_->PlayBlendAnimation(
				Animation::Idle, Animation::Dash,
				true, owner_->GetAnimationSpeed()
			);

			// ���̃A�j���[�V�����Đ�
			Model* sword = owner_->GetSword();
			if (sword != nullptr)
			{
				sword->PlayBlendAnimation(
					Animation::Idle, Animation::Dash,
					true, owner_->GetAnimationSpeed()
				);
			}
		}

		owner_->SetStep(1);
		[[fallthrough]];
	//	break;
	case 1:
		// �^�C�}�[�X�V
		owner_->ElapseRunTimer(elapsedTime);

		//// �ҋ@�ƕ����̃u�����h�A�j���[�V�������I�������
		//// �ҋ@�A�j���[�V�����ɂ��Ă���
		//if (false == owner_->IsPlayAnimation())
		//{
		//	owner_->PlayAnimation(
		//		Animation::Idle, true,
		//		owner_->GetAnimationSpeed()
		//	);
		//}

		//// �N���ߐڍU���s�����łȂ��ߐڍU���s���N�[���^�C�}�[���Ȃ���ΏI��
		//if (false == enemyManager.GetIsRunningCRAAction() &&
		//	enemyManager.GetCRAActionCoolTimer() <= 0.0f)
		//{
		//	step_ = 0;
		//	return ActionBase::State::Complete;
		//}

		//// �Ђ�݃t���O�������Ă���ΏI��
		//if (true == owner_->GetIsFlinch())
		//{
		//	step_ = 0;
		//	return ActionBase::State::Failed;
		//}

		// �ҋ@���Ԃ��߂�����I��
		if (owner_->GetRunTimer() <= 0.0f)
		{
			owner_->SetStep(0);
			return ActionBase::State::Complete;
		}

		const XMFLOAT3&	pos				= owner_->GetPosition();
		const XMFLOAT3&	plPos			= plManager.GetPlayer()->GetPosition();
		const float		vx				= plPos.x - pos.x;
		const float		vz				= plPos.z - pos.z;
		const float		lengthSq		= (vx * vx + vz * vz);
		const float		attackLength	= owner_->GetAttackLength();
		const float		attackLengthSq	= attackLength * attackLength;

		// ��]
		owner_->Turn(elapsedTime, vx, vz, owner_->GetTurnSpeed());

		// �U�������O�ɂȂ�����I��(�~�^���̓s����Â߂ɐݒ�)
		if (lengthSq > (attackLengthSq + 5.0f))
		{
			owner_->SetStep(0);
			return ActionBase::State::Complete;
		}

		// �~�^�����ǂ�
#if 0
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
#endif
	}

	return ActionBase::State::Run;
}


// �ǐՍs��
const ActionBase::State PursuitAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using Animation = Player::Animation;

	PlayerManager& plManager = PlayerManager::Instance();

	switch (owner_->GetStep())
	{
	case 0:
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner_->SetTargetPosition(plManager.GetPlayer()->GetPosition());

		// �A�j���[�V�����Đ�
		{
			owner_->PlayBlendAnimation(
				Animation::Idle, Animation::Dash,
				true, owner_->GetAnimationSpeed()
			);

			// ���̃A�j���[�V�����Đ�
			Model* sword = owner_->GetSword();
			if (sword != nullptr)
			{
				sword->PlayBlendAnimation(
					Animation::Idle, Animation::Dash,
					true, owner_->GetAnimationSpeed()
				);
			}
		}

		owner_->SetStep(1);
		[[fallthrough]];
		//break;
	case 1:
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner_->SetTargetPosition(plManager.GetPlayer()->GetPosition());

		// �v���C���[�Ƃ�XZ���ʂł̋�������
		const XMFLOAT3	pos		  = owner_->GetPosition();
		const XMFLOAT3	targetPos = owner_->GetTargetPosition();
		const float		vx		  = targetPos.x - pos.x;
		const float		vz		  = targetPos.z - pos.z;
		const float		lengthSq  = (vx * vx + vz * vz);

		// �U�����������܂Őڋ߂�����ǐՐ�����Ԃ�
		const float	attackLength = owner_->GetAttackLength();
		if (lengthSq <= attackLength * attackLength)
		{
			owner_->SetStep(0);
			return ActionBase::State::Complete;
		}
		// �ڕW�n�_�ֈړ�
		else
		{
			owner_->MoveToTarget(elapsedTime, 1.0);
		}

		break;
	}

	return ActionBase::State::Run;
}


// �ߐڍU���s��
const ActionBase::State CloseRangeAttackAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using Animation = Player::Animation;

	PlayerManager& plManager  = PlayerManager::Instance();
	EnemyManager&  enmManager = EnemyManager::Instance();

	switch (owner_->GetStep())
	{
	case 0:
		owner_->SetRunTimer(60.0f); // �^�C�}�[�ݒ�

		// �A�j���[�V�����Đ�
		{
			// �U���A�j���[�V�����Đ�
			owner_->PlayAnimation(
				Animation::Jab_1, false,
				owner_->GetAnimationSpeed()
			);

			// ���̃A�j���[�V�����Đ�
			Model* sword = owner_->GetSword();
			if (sword != nullptr)
			{
				sword->PlayAnimation(
					Animation::Jab_1, false,
					owner_->GetAnimationSpeed()
				);
			}
		}

		owner_->SetStep(1);
		[[fallthrough]];
		//break;
	case 1:
		// �^�C�}�[�X�V
		owner_->ElapseRunTimer(elapsedTime);

		// ��]
		{
			const XMFLOAT3& pos = owner_->GetTransform()->GetPosition();
			const XMFLOAT3& plPos = plManager.GetPlayer()->GetTransform()->GetPosition();
			const float		vx = plPos.x - pos.x;
			const float		vz = plPos.z - pos.z;
			owner_->Turn(elapsedTime, vx, vz, owner_->GetTurnSpeed());
		}

		// �U���A�j���[�V�������I������琬���I��
		if (false == owner_->IsPlayAnimation())
		{
			owner_->SetStep(0);

			// CRA : 5.Action : �ߐڍU���s�����s���t���O�����낷
			enmManager.SetIsRunningCRAAction(false);

			// CRA : 6.Action : �ߐڍU���s���N�[���^�C�}�[�ݒ�
			constexpr float craCoolTime = 0.0f;
			enmManager.SetCRAActionCoolTimer(craCoolTime);

			return ActionBase::State::Complete; // ����
		}

		// �s�����Ԃ��߂����玸�s�I��
		if (owner_->GetRunTimer() <= 0.0f)
		{
			owner_->SetStep(0);

			// CRA : 5.Action : �ߐڍU���s�����s���t���O�����낷
			enmManager.SetIsRunningCRAAction(false);

			// CRA : 6.Action : �ߐڍU���s���N�[���^�C�}�[�ݒ�
			constexpr float craCoolTime = 0.0f;
			enmManager.SetCRAActionCoolTimer(craCoolTime);

			return ActionBase::State::Failed; // ���s
		}

		break;
	}

	return ActionBase::State::Run;
}


// �������U���s��
const ActionBase::State LongRangeAttackAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using Animation = Player::Animation;

	PlayerManager& plManager = PlayerManager::Instance();

	switch (owner_->GetStep())
	{
	case 0:
		//owner_->SetRunTimer(::RandFloat(+1.0f, +1.0f));
		owner_->SetRunTimer(60.0f); // �s���^�C�}�[�ݒ�

		// �A�j���[�V�����Đ�
		{
			// �U���A�j���[�V�����Đ�
			owner_->PlayAnimation(
				Animation::Jab_1, false,
				owner_->GetAnimationSpeed()
			);

			// ���̃A�j���[�V�����Đ�
			Model* sword = owner_->GetSword();
			if (sword != nullptr)
			{
				sword->PlayAnimation(
					Animation::Jab_1, false,
					owner_->GetAnimationSpeed()
				);
			}
		}

		owner_->SetStep(1);
		[[fallthrough]];
		//break;
	case 1:
		// �^�C�}�[�X�V
		owner_->ElapseRunTimer(elapsedTime);

		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner_->SetTargetPosition(plManager.GetPlayer()->GetPosition());

		const XMFLOAT3	pos		= owner_->GetPosition();
		const XMFLOAT3	plPos	= plManager.GetPlayer()->GetPosition();
		const XMFLOAT3  vec		= plPos - pos;
		const XMFLOAT3	vecN	= ::XMFloat3Normalize(vec);

		// ��]
		owner_->Turn(elapsedTime, vec.x, vec.z, owner_->GetTurnSpeed());

		// �w��̃L�[�t���[���ɂȂ�����e�۔���
		if (19 == owner_->model->GetCurrentKeyframeIndex())
		{
			const XMFLOAT3 launchPos = {
				pos.x, (pos.y + 0.4f), pos.z,
			};
			ProjectileStraight* projectile = new ProjectileStraight(owner_->GetProjectileManager());
			projectile->Launch(vecN, launchPos);
		}

		// �U���A�j���[�V�������I������琬���I��
		if (false == owner_->IsPlayAnimation())
		{
			owner_->SetStep(0);
			return ActionBase::State::Complete; // ����
		}

		// �s�����Ԃ��߂����玸�s�I��
		if (owner_->GetRunTimer() <= 0.0f)
		{
			owner_->SetStep(0);
			return ActionBase::State::Failed; // ���s
		}

		break;
	}

	return ActionBase::State::Run;
}


// �Ђ�ݍs��
const ActionBase::State FlinchAction::Run(const float elapsedTime)
{
	using Animation = Player::Animation;

	switch (owner_->GetStep())
	{
	case 0:
		//owner_->SetRunTimer(1.5f);

		// �Ђ�݃A�j���[�V�����Đ�
		{
			owner_->PlayAnimation(Animation::SoftStagger, false, owner_->GetAnimationSpeed());

			Model* sword = owner_->GetSword();
			if (sword != nullptr)
			{
				// �������͑ҋ@�A�j���[�V�����ɂ��Ă���
				sword->PlayAnimation(Animation::Idle, false, owner_->GetAnimationSpeed());
			}
		}

		owner_->SetStep(1);
		[[fallthrough]];
		//break;
	case 1:
		//owner_->ElapseRunTimer(elapsedTime);

		//// ���Ԃ��o�߂�����Ђ�݂��I��
		//if (owner_->GetRunTimer() <= 0.0f)
		// �Ђ�݃A�j���[�V�������I�������I��
		if (false == owner_->IsPlayAnimation())
		{
			owner_->SetStep(0);
			owner_->SetIsFlinch(false); // �Ђ�݃t���O�����낷

			return ActionBase::State::Complete;
		}

		break;
	}

	return ActionBase::State::Run;
}





// �U���s��
const ActionBase::State NormalAction::Run(const float /*elapsedTime*/)
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
const ActionBase::State SkillAction::Run(const float /*elapsedTime*/)
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
const ActionBase::State WanderAction::Run(const float /*elapsedTime*/)
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
const ActionBase::State LeaveAction::Run(const float /*elapsedTime*/)
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
const ActionBase::State RecoverAction::Run(const float /*elapsedTime*/)
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

