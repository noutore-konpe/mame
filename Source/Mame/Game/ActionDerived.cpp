#include "ActionDerived.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../../Taki174/Common.h"

#include "../Scene/SceneGame.h"

#include "BaseEnemyAI.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "ProjectileStraight.h"

// ステージエントリー行動
const ActionBase::State EntryStageAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using Animation = Player::Animation;

	switch (owner_->GetStep())
	{
	case 0:
		// 現在の位置からステージ中心に向かってある程度進んだ位置を目標地点に設定
		{
			const XMFLOAT3& pos  = owner_->GetTransform()->GetPosition();
			const XMFLOAT3  vecN = ::XMFloat3Normalize(SceneGame::stageCenter - pos);
			static constexpr float moveLength = 19.0f;	// 出入口からステージくらいまでの距離
			owner_->SetTargetPosition(pos + vecN * moveLength);
		}

		// アニメーション再生
		{
			owner_->PlayBlendAnimation(
				Animation::Idle, Animation::Dash,
				true, owner_->GetAnimationSpeed()
			);

			// 剣のアニメーション再生
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
		// 目標地点へ移動
		owner_->MoveToTarget(elapsedTime, 1.0);

		// 目標地点とのXZ平面での距離判定
		const XMFLOAT3&	pos		  = owner_->GetPosition();
		const XMFLOAT3&	targetPos = owner_->GetTargetPosition();
		const float		vx = targetPos.x - pos.x;
		const float		vz = targetPos.z - pos.z;
		const float		lengthXZSq = (vx * vx + vz * vz);

		// 目標地点に到達したら成功終了する
		static constexpr float acceptableLength = 0.1f; // 許容する距離
		if (lengthXZSq <= acceptableLength * acceptableLength)
		{
			owner_->SetEntryStageFlag(true); // ステージに入った

			owner_->SetStep(0);
			return ActionBase::State::Complete;
		}

		break;
	}

	return ActionBase::State::Run;
}


// 待機行動
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

		// 自分の位置から回転を始めるようにする
		{
			const XMFLOAT3&	pos			= owner_->GetPosition();
			const XMFLOAT3&	plPos		= plManager.GetPlayer()->GetPosition();
			const float		localPosX	= pos.x - plPos.x;
			const float		localPosZ	= pos.z - plPos.z;

			circleRotation_ = ::atan2f(localPosX, localPosZ); // return Radian Angle
		}

		// アニメーション再生
		{
			owner_->PlayBlendAnimation(
				Animation::Idle, Animation::Dash,
				true, owner_->GetAnimationSpeed()
			);

			// 剣のアニメーション再生
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
		// タイマー更新
		owner_->ElapseRunTimer(elapsedTime);

		//// 待機と歩きのブレンドアニメーションが終わったら
		//// 待機アニメーションにしておく
		//if (false == owner_->IsPlayAnimation())
		//{
		//	owner_->PlayAnimation(
		//		Animation::Idle, true,
		//		owner_->GetAnimationSpeed()
		//	);
		//}

		//// 誰も近接攻撃行動中でなく近接攻撃行動クールタイマーもなければ終了
		//if (false == enemyManager.GetIsRunningCRAAction() &&
		//	enemyManager.GetCRAActionCoolTimer() <= 0.0f)
		//{
		//	step_ = 0;
		//	return ActionBase::State::Complete;
		//}

		//// ひるみフラグが立っていれば終了
		//if (true == owner_->GetIsFlinch())
		//{
		//	step_ = 0;
		//	return ActionBase::State::Failed;
		//}

		// 待機時間が過ぎたら終了
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

		// 回転
		owner_->Turn(elapsedTime, vx, vz, owner_->GetTurnSpeed());

		// 攻撃距離外になったら終了(円運動の都合上甘めに設定)
		if (lengthSq > (attackLengthSq + 5.0f))
		{
			owner_->SetStep(0);
			return ActionBase::State::Complete;
		}

		// 円運動もどき
#if 0
		{
			// 左右判定
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

			// 背後に回り切ったら終了
			if (fabsf(cross) <= 0.01f)
			{
				step_ = 0;
				return ActionBase::State::Complete;
			}

			const float addRotate = (cross < 0.0f) ? ToRadian(-25.0f) : ToRadian(25.0f);

			// プレイヤーを中心に(+1.0~-1.0)＊半径(=攻撃距離)分を回転
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


// 追跡行動
const ActionBase::State PursuitAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using Animation = Player::Animation;

	PlayerManager& plManager = PlayerManager::Instance();

	switch (owner_->GetStep())
	{
	case 0:
		// 目標地点をプレイヤー位置に設定
		owner_->SetTargetPosition(plManager.GetPlayer()->GetPosition());

		// アニメーション再生
		{
			owner_->PlayBlendAnimation(
				Animation::Idle, Animation::Dash,
				true, owner_->GetAnimationSpeed()
			);

			// 剣のアニメーション再生
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
		// 目標地点をプレイヤー位置に設定
		owner_->SetTargetPosition(plManager.GetPlayer()->GetPosition());

		// プレイヤーとのXZ平面での距離判定
		const XMFLOAT3	pos		  = owner_->GetPosition();
		const XMFLOAT3	targetPos = owner_->GetTargetPosition();
		const float		vx		  = targetPos.x - pos.x;
		const float		vz		  = targetPos.z - pos.z;
		const float		lengthSq  = (vx * vx + vz * vz);

		// 攻撃距離圏内まで接近したら追跡成功を返す
		const float	attackLength = owner_->GetAttackLength();
		if (lengthSq <= attackLength * attackLength)
		{
			owner_->SetStep(0);
			return ActionBase::State::Complete;
		}
		// 目標地点へ移動
		else
		{
			owner_->MoveToTarget(elapsedTime, 1.0);
		}

		break;
	}

	return ActionBase::State::Run;
}


// 近接攻撃行動
const ActionBase::State CloseRangeAttackAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using Animation = Player::Animation;

	PlayerManager& plManager  = PlayerManager::Instance();
	EnemyManager&  enmManager = EnemyManager::Instance();

	switch (owner_->GetStep())
	{
	case 0:
		owner_->SetRunTimer(60.0f); // タイマー設定

		// アニメーション再生
		{
			// 攻撃アニメーション再生
			owner_->PlayAnimation(
				Animation::Jab_1, false,
				owner_->GetAnimationSpeed()
			);

			// 剣のアニメーション再生
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
		// タイマー更新
		owner_->ElapseRunTimer(elapsedTime);

		// 回転
		{
			const XMFLOAT3& pos = owner_->GetTransform()->GetPosition();
			const XMFLOAT3& plPos = plManager.GetPlayer()->GetTransform()->GetPosition();
			const float		vx = plPos.x - pos.x;
			const float		vz = plPos.z - pos.z;
			owner_->Turn(elapsedTime, vx, vz, owner_->GetTurnSpeed());
		}

		// 攻撃アニメーションが終わったら成功終了
		if (false == owner_->IsPlayAnimation())
		{
			owner_->SetStep(0);

			// CRA : 5.Action : 近接攻撃行動実行中フラグを下ろす
			enmManager.SetIsRunningCRAAction(false);

			// CRA : 6.Action : 近接攻撃行動クールタイマー設定
			constexpr float craCoolTime = 0.0f;
			enmManager.SetCRAActionCoolTimer(craCoolTime);

			return ActionBase::State::Complete; // 成功
		}

		// 行動時間が過ぎたら失敗終了
		if (owner_->GetRunTimer() <= 0.0f)
		{
			owner_->SetStep(0);

			// CRA : 5.Action : 近接攻撃行動実行中フラグを下ろす
			enmManager.SetIsRunningCRAAction(false);

			// CRA : 6.Action : 近接攻撃行動クールタイマー設定
			constexpr float craCoolTime = 0.0f;
			enmManager.SetCRAActionCoolTimer(craCoolTime);

			return ActionBase::State::Failed; // 失敗
		}

		break;
	}

	return ActionBase::State::Run;
}


// 遠距離攻撃行動
const ActionBase::State LongRangeAttackAction::Run(const float elapsedTime)
{
	using DirectX::XMFLOAT3;
	using Animation = Player::Animation;

	PlayerManager& plManager = PlayerManager::Instance();

	switch (owner_->GetStep())
	{
	case 0:
		//owner_->SetRunTimer(::RandFloat(+1.0f, +1.0f));
		owner_->SetRunTimer(60.0f); // 行動タイマー設定

		// アニメーション再生
		{
			// 攻撃アニメーション再生
			owner_->PlayAnimation(
				Animation::Jab_1, false,
				owner_->GetAnimationSpeed()
			);

			// 剣のアニメーション再生
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
		// タイマー更新
		owner_->ElapseRunTimer(elapsedTime);

		// 目標地点をプレイヤー位置に設定
		owner_->SetTargetPosition(plManager.GetPlayer()->GetPosition());

		const XMFLOAT3	pos		= owner_->GetPosition();
		const XMFLOAT3	plPos	= plManager.GetPlayer()->GetPosition();
		const XMFLOAT3  vec		= plPos - pos;
		const XMFLOAT3	vecN	= ::XMFloat3Normalize(vec);

		// 回転
		owner_->Turn(elapsedTime, vec.x, vec.z, owner_->GetTurnSpeed());

		// 指定のキーフレームになったら弾丸発射
		if (19 == owner_->model->GetCurrentKeyframeIndex())
		{
			const XMFLOAT3 launchPos = {
				pos.x, (pos.y + 0.4f), pos.z,
			};
			ProjectileStraight* projectile = new ProjectileStraight(owner_->GetProjectileManager());
			projectile->Launch(vecN, launchPos);
		}

		// 攻撃アニメーションが終わったら成功終了
		if (false == owner_->IsPlayAnimation())
		{
			owner_->SetStep(0);
			return ActionBase::State::Complete; // 成功
		}

		// 行動時間が過ぎたら失敗終了
		if (owner_->GetRunTimer() <= 0.0f)
		{
			owner_->SetStep(0);
			return ActionBase::State::Failed; // 失敗
		}

		break;
	}

	return ActionBase::State::Run;
}


// ひるみ行動
const ActionBase::State FlinchAction::Run(const float elapsedTime)
{
	using Animation = Player::Animation;

	switch (owner_->GetStep())
	{
	case 0:
		//owner_->SetRunTimer(1.5f);

		// ひるみアニメーション再生
		{
			owner_->PlayAnimation(Animation::SoftStagger, false, owner_->GetAnimationSpeed());

			Model* sword = owner_->GetSword();
			if (sword != nullptr)
			{
				// こっちは待機アニメーションにしておく
				sword->PlayAnimation(Animation::Idle, false, owner_->GetAnimationSpeed());
			}
		}

		owner_->SetStep(1);
		[[fallthrough]];
		//break;
	case 1:
		//owner_->ElapseRunTimer(elapsedTime);

		//// 時間が経過したらひるみを終了
		//if (owner_->GetRunTimer() <= 0.0f)
		// ひるみアニメーションが終わったら終了
		if (false == owner_->IsPlayAnimation())
		{
			owner_->SetStep(0);
			owner_->SetIsFlinch(false); // ひるみフラグを下ろす

			return ActionBase::State::Complete;
		}

		break;
	}

	return ActionBase::State::Run;
}





// 攻撃行動
const ActionBase::State NormalAction::Run(const float /*elapsedTime*/)
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
const ActionBase::State SkillAction::Run(const float /*elapsedTime*/)
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
const ActionBase::State WanderAction::Run(const float /*elapsedTime*/)
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
const ActionBase::State LeaveAction::Run(const float /*elapsedTime*/)
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

