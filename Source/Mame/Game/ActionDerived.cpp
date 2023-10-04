#include "ActionDerived.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../../Taki174/Common.h"

//#include "EnemyBlueSlime.h"
#include "BaseEnemyAI.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
//#include "Mathf.h"


// 待機行動
const ActionBase::State IdleAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;

	PlayerManager& playerManager = PlayerManager::Instance();

	//float runTimer = owner_->GetRunTimer();
	switch (step_)
	{
	case 0:
		owner_->SetRunTimer(::RandFloat(1.0f, 2.0f));
	//	owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::IdleNormal), true);

		++step_;
		[[fallthrough]];
	//	break;
	case 1:
	//	runTimer -= elapsedTime;
	//	owner_->SetRunTimer(runTimer);
		// タイマー更新
		owner_->ElapseRunTimer(elapsedTime);

		// 待機時間が過ぎた時
		if (owner_->GetRunTimer() <= 0.0f)
		{
			//owner_->SetRandomTargetPosition();
			owner_->SetRunTimer(0.0f);
			step_ = 0;

			return ActionBase::State::Complete;
		}

		const XMFLOAT3 position			= owner_->GetPosition();
		const XMFLOAT3 targetPosition	= playerManager.GetPlayer()->GetPosition();
		const float vx	= targetPosition.x - position.x;
		const float vz	= targetPosition.z - position.z;
		owner_->Turn(elapsedTime, vx, vz, owner_->GetTurnSpeed());

	//	// プレイヤーを見つけた時
	//	if (owner_->SearchPlayer())
	//	{
	//		step_ = 0;
	//		return ActionBase::State::Complete;
	//	}
	//	break;
	}

	return ActionBase::State::Run;
}


// 追跡行動
const ActionBase::State PursuitAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;

	PlayerManager& playerManager = PlayerManager::Instance();

	//float runTimer = owner_->GetRunTimer();
	switch (step_)
	{
	case 0:
		// 目標地点をプレイヤー位置に設定
		owner_->SetTargetPosition(playerManager.GetPlayer()->GetPosition());
		//owner_->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		//owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::RunFWD), true);

		++step_;
		[[fallthrough]];
		//break;
	case 1:
		//runTimer -= elapsedTime;
		//// タイマー更新
		//owner_->SetRunTimer(runTimer);

		// 目標地点をプレイヤー位置に設定
		owner_->SetTargetPosition(playerManager.GetPlayer()->GetPosition());

		// プレイヤーとのXZ平面での距離判定
		const XMFLOAT3	position = owner_->GetPosition();
		const XMFLOAT3	targetPosition = owner_->GetTargetPosition();
		const float		vx = targetPosition.x - position.x;
		const float		vz = targetPosition.z - position.z;
		const float		lengthSq = (vx * vx + vz * vz);

		// 攻撃距離圏内まで接近したら追跡成功を返す
		const float		attackLength = owner_->GetAttackLength();
		if (lengthSq <= attackLength * attackLength)
		{
			step_ = 0;
			return ActionBase::State::Complete;
		}
		// 目標地点へ移動
		else
		{
			owner_->MoveToTarget(elapsedTime, 1.0);
		}

		//// 行動時間が過ぎた時
		//if (runTimer <= 0.0f)
		//{
		//	step_ = 0;
		//	// 追跡失敗を返す
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
		// 目標地点をプレイヤー位置に設定
		//owner_->SetTargetPosition(playerManager.GetPlayer()->GetPosition());
		owner_->SetRunTimer(::RandFloat(+1.0f, +1.0f));
		//owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::RunFWD), true);

		++step_;
		[[fallthrough]];
		//break;
	case 1:
		//runTimer -= elapsedTime;
		//owner_->SetRunTimer(runTimer);
		// タイマー更新
		owner_->ElapseRunTimer(elapsedTime);

		// 目標地点をプレイヤー位置に設定
		//owner_->SetTargetPosition(playerManager.GetPlayer()->GetPosition());

		// 行動しているのがわかりやすいように仮で回転させる
		owner_->GetTransform()->AddRotationY(ToRadian(1000.0f) * elapsedTime);

		// 行動時間が過ぎた時
		if (owner_->GetRunTimer() <= 0.0f)
		{
			step_ = 0;

			// CRA : 4.Action : 近接攻撃行動実行中フラグを下ろす
			enemyManager.SetIsRunningCRAAction(false);

			// CRA : 4.Action : 近接攻撃行動クールタイマー設定
			constexpr float craCoolTime = 0.0f;
			enemyManager.SetCRAActionCoolTimer(craCoolTime);

			//return ActionBase::State::Failed;
			return ActionBase::State::Complete;
		}

		break;
	}

	return ActionBase::State::Run;
}


// 攻撃行動
const ActionBase::State NormalAction::Run(const float elapsedTime)
{
	//switch (step_)
	//{
	//case 0:
	//	// 目標地点をプレイヤー位置に設定
	//	owner_->SetTargetPosition(Player::Instance().GetPosition());
	//	// アニメーション再生
	//	owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::Attack01), false);
	//	step_++;
	//	break;
	//case 1:

	//	// アニメーションが終了しているとき
	//	if (!owner_->GetModel()->IsPlayAnimation())
	//	{
	//		step_ = 0;
	//		// 攻撃成功を返す
	//		return ActionBase::State::Complete;
	//	}
	//	break;
	//}
	//// スキル中を返す
	return ActionBase::State::Run;
}

// スキル攻撃行動
const ActionBase::State SkillAction::Run(const float elapsedTime)
{
	//switch (step_)
	//{
	//	case 0:
	//		// 目標地点をプレイヤー位置に設定
	//		owner_->SetTargetPosition(Player::Instance().GetPosition());
	//		// アニメーション再生
	//		owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::Attack02), false);
	//		step_++;
	//	case 1:
	//		// アニメーションが終了しているとき
	//		if (!owner_->GetModel()->IsPlayAnimation())
	//		{
	//			step_ = 0;
	//			// 攻撃成功を返す
	//			return ActionBase::State::Complete;
	//		}
	//		break;
	//}
	//// スキル中を返す
	return ActionBase::State::Run;
}

// 徘徊行動
const ActionBase::State WanderAction::Run(const float elapsedTime)
{
	//switch (step_)
	//{
	//case 0:
	//	// 徘徊モーション設定
	//	owner_->GetModel()->PlayAnimation(static_cast<int>(EnemyBlueSlime::EnemyAnimation::WalkFWD), true);
	//	step_++;
	//	break;
	//case 1:
	//	// 目的地点までのXZ平面での距離判定
	//	DirectX::XMFLOAT3 position = owner_->GetPosition();
	//	DirectX::XMFLOAT3 targetPosition = owner_->GetTargetPosition();
	//	float vx = targetPosition.x - position.x;
	//	float vz = targetPosition.z - position.z;
	//	float distSq = vx * vx + vz * vz;

	//	// 目的地へ着いた
	//	float radius = owner_->GetRadius();
	//	if (distSq < radius * radius)
	//	{
	//		step_ = 0;
	//		// 徘徊成功を返す
	//		return ActionBase::State::Complete;
	//	}

	//	// 目的地点へ移動
	//	owner_->MoveToTarget(elapsedTime, 0.5f);

	//	// プレイヤー索敵成功したら
	//	if (owner_->SearchPlayer())
	//	{
	//		step_ = 0;
	//		// 徘徊成功を返す
	//		return ActionBase::State::Complete;
	//	}
	//	break;
	//}
	//// 実行中
	return ActionBase::State::Run;
}


// 逃走行動
const ActionBase::State LeaveAction::Run(const float elapsedTime)
{
	//DirectX::XMFLOAT3 targetPosition;
	//switch (step_)
	//{
	//case 0:
	//	// 目標地点をプレイヤーと正反対のベクトル×5の位置に指定
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
	//	// 目的地点へ移動
	//	owner_->MoveToTarget(elapsedTime, 1.0);

	//	DirectX::XMFLOAT3 position = owner_->GetPosition();
	//	targetPosition = owner_->GetTargetPosition();

	//	float vx = targetPosition.x - position.x;
	//	float vz = targetPosition.z - position.z;
	//	float distSq = vx * vx + vz * vz;

	//	// 目的地へ着いた
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

// 回復行動
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


