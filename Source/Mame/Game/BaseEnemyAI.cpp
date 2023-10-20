#include "BaseEnemyAI.h"

#include "../../Taki174/Common.h"
#include "EnemyManager.h"
#include "../Other/MathHelper.h"

#include "../Scene/SceneGame.h"

BaseEnemyAI::BaseEnemyAI()
{
    // EnemyManagerのProjectileManagerにアクセスできるようにポインタを持っておく
    projectileManager_ = EnemyManager::Instance().GetProjectileManager();
}

void BaseEnemyAI::Initialize()
{
    Character::Initialize();

    //ジョイント位置取得するために適当なアニメーション再生
    //※アニメーションを流しとかないとkeyframeに情報が入らないからジョイント位置がとれない
    PlayAnimation(0,true);

    ColliderInitialize();
}

void BaseEnemyAI::Update(const float& elapsedTime)
{
    Character::Update(elapsedTime);

    // ノード更新
    UpdateNode(elapsedTime);

    // 速力処理更新
    UpdateVelocity(elapsedTime);

    //// 無敵時間更新
    //UpdateInvincibleTimer(elapsedTime);

    // アニメーション更新
    Character::UpdateAnimation(elapsedTime);

    // 剣の更新
    UpdateSword(elapsedTime);
}

void BaseEnemyAI::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader);

#if _DEBUG
    if (SceneGame::isDispCollision_)
    {
        for (auto& collider : hitCollider)
        {
            collider.DebugRender();
        }

        for (auto& collider : attackCollider)
        {
            collider.DebugRender(DirectX::XMFLOAT4(1, 0, 0, 1));
        }
    }
#endif

    //ColliderPosUpdate(scale);
}

// ImGui用
void BaseEnemyAI::DrawDebug()
{
#ifdef USE_IMGUI
        std::string str = (activeNode_ != nullptr)
                        ? activeNode_->GetName()
                        : u8"なし";
        ImGui::Text(u8"Behavior：%s", str.c_str());

        ImGui::InputFloat("RunTimer", &runTimer_);

        Character::DrawDebug();

        model->skinned_meshes->Drawdebug();

        //float r = GetRange();
        //ImGui::DragFloat("range", &r);
        //SetRange(r);
#endif // USE_IMGUI
}

void BaseEnemyAI::OnDamaged()
{
    Flinch(); // ひるませる
}

void BaseEnemyAI::OnDead()
{
    // 吹っ飛ばす（事前にSaveBlowOffInfo関数などで吹っ飛び情報を保存しておく必要がある）
    BlowOff(blowOffVec_, blowOffForceLevel_);
}

void BaseEnemyAI::ColliderInitialize()
{
    //初期化
    if (hitCollider.size() == 0)
    {
        swordColliderNum = 5;
        swordColliderRadius = 0.12f;
        if (sword_ != nullptr)
        {
            for (int i = 0; i < swordColliderNum; i++)
            {
                attackCollider.emplace_back(SphereCollider(swordColliderRadius));
            }
        }

        for (int i = 0; i < static_cast<int>(HitColName::END); i++)
        {
            hitCollider.emplace_back(SphereCollider(0.1f));
        }
    }
    const float scale = GetTransform()->GetScale().x;
    hitCollider[static_cast<int>(HitColName::NECK)].radius        = 0.15f * scale;
    hitCollider[static_cast<int>(HitColName::HIP)].radius         = 0.25f * scale;
    hitCollider[static_cast<int>(HitColName::R_LEG)].radius       = 0.15f * scale;
    hitCollider[static_cast<int>(HitColName::L_LEG)].radius       = 0.15f * scale;
    hitCollider[static_cast<int>(HitColName::R_LEG_END)].radius   = 0.15f * scale;
    hitCollider[static_cast<int>(HitColName::L_LEG_END)].radius   = 0.15f * scale;
    hitCollider[static_cast<int>(HitColName::R_ELBOW)].radius     = 0.12f * scale;
    hitCollider[static_cast<int>(HitColName::L_ELBOW)].radius     = 0.12f * scale;
    hitCollider[static_cast<int>(HitColName::R_HAND)].radius      = 0.12f * scale;
    hitCollider[static_cast<int>(HitColName::L_HAND)].radius      = 0.12f * scale;
}

void BaseEnemyAI::ColliderPosUpdate(const float& scale)
{
    //喰らい判定
    {
        const std::string meshBodyName = "ref_P:chara_rig_0906:chara_mdl_1017:pasted__Body";
        const std::string meshLegName = "ref_P:chara_rig_0906:chara_mdl_1017:pasted__Socks";
        hitCollider[static_cast<int>(HitColName::NECK)].position = GetJointPosition(meshBodyName, "ref_P:chara_rig_0906:j_Neck", scale);
        hitCollider[static_cast<int>(HitColName::HIP)].position = GetJointPosition(meshBodyName, "ref_P:chara_rig_0906:j_Hips", scale);
        hitCollider[static_cast<int>(HitColName::R_ELBOW)].position = GetJointPosition(meshBodyName, "ref_P:chara_rig_0906:j_RightForeArm", scale);
        hitCollider[static_cast<int>(HitColName::L_ELBOW)].position = GetJointPosition(meshBodyName, "ref_P:chara_rig_0906:j_LeftForeArm", scale);
        hitCollider[static_cast<int>(HitColName::R_HAND)].position = GetJointPosition(meshBodyName, "ref_P:chara_rig_0906:j_RightHand", scale);
        hitCollider[static_cast<int>(HitColName::L_HAND)].position = GetJointPosition(meshBodyName, "ref_P:chara_rig_0906:j_LeftHand", scale);


        //hitCollider[static_cast<int>(HitColName::LEG)].position = GetJointPosition(meshBodyName,"ref_P:chara_rig_0906:j_Sentar",scale);
        hitCollider[static_cast<int>(HitColName::R_LEG)].position = GetJointPosition(meshLegName, "ref_P:chara_rig_0906:j_RightLeg", scale);
        hitCollider[static_cast<int>(HitColName::L_LEG)].position = GetJointPosition(meshLegName, "ref_P:chara_rig_0906:j_LeftLeg", scale);
        hitCollider[static_cast<int>(HitColName::R_LEG_END)].position = GetJointPosition(meshLegName, "ref_P:chara_rig_0906:j_RightFoot", scale);
        hitCollider[static_cast<int>(HitColName::L_LEG_END)].position = GetJointPosition(meshLegName, "ref_P:chara_rig_0906:j_LeftFoot", scale);
    }

    //攻撃判定
    if(attackCollider.size() > 0)
    {
        DirectX::XMFLOAT4X4 world{};
        DirectX::XMStoreFloat4x4(&world, sword_->GetTransform()->CalcWorldMatrix(scale));
        //const std::string swordMeshName = "sword_rig_1004:sword_rig_1005:sword_mdl_1005:Sword";
        const std::string swordMeshName = "sword_rig_1004:sword_rig_1005:sword_mdl_1005:Sword";
        const DirectX::XMFLOAT3 swordRoot = sword_->skinned_meshes->JointPosition(swordMeshName, "sword_rig_1004:sword_rig_1005:j_sword", &sword_->keyframe, world);//根本
        const DirectX::XMFLOAT3 swordTip = sword_->skinned_meshes->JointPosition(swordMeshName, "sword_rig_1004:sword_rig_1005:j_sword_end", &sword_->keyframe, world);//先端

        const DirectX::XMFLOAT3 vec = swordTip - swordRoot;
        float swordLength = Length(vec);
        const DirectX::XMFLOAT3 vecNormal = Normalize(vec);

        const float collideInterval = swordLength / swordColliderNum;//判定ごとの設置間隔

        for (int i = 0; i < attackCollider.size(); ++i)
        {
            attackCollider[i].position = swordRoot + vecNormal * collideInterval * static_cast<float>(i);
        }
    }
}


void BaseEnemyAI::Move(
    const float vx,
    const float vz,
    const float moveSpeed)
{
    moveVec_.x = vx;
    moveVec_.z = vz;
    maxMoveSpeed_ = moveSpeed;
}


void BaseEnemyAI::Turn(
    const float elapsedTime,
    float vx,
    float vz,
    float turnSpeed /*Degree*/)
{
    if (vx == 0.0f && vz == 0.0f) return;

    using DirectX::XMFLOAT3;
    using DirectX::XMFLOAT4;

    const float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001f) return;

    vx /= length;
    vz /= length;

    XMFLOAT4 rotation = GetTransform()->GetRotation();
    const float frontX = ::sinf(rotation.y);
    const float frontZ = ::cosf(rotation.y);

    const float dot = (frontX * vx) + (frontZ * vz);

    // 回転角が微小な場合は回転を行わない
    const float angle = ::acosf(dot); // ラジアン絶対値
    if (::fabsf(angle) <= 0.01f) return;

    float rot = 1.0f - dot;
    turnSpeed = ToRadian(turnSpeed) * elapsedTime;
    if (rot > turnSpeed) rot = turnSpeed;

    //左右判定のための外積
    const float cross = (frontZ * vx) - (frontX * vz);

    rotation.y += (cross < 0.0f) ? -rot : rot;

    GetTransform()->SetRotation(rotation);
}


// ひるませる関数
void BaseEnemyAI::Flinch()
{
    //// 入場中ならひるませない
    //if (activeNode_ != nullptr && "EntryStage" == activeNode_->GetName()) return;

    flinchStartFlag_ = true;     // ひるみ開始フラグを立てる

    // 連続でひるんだ際にひるみアニメ―ションを
    // 毎回リセットするために適当なアニメーションを入力しておく
    PlayAnimation(0, false, animationSpeed_);
}

// 吹っ飛ばす関数
void BaseEnemyAI::BlowOff(
    const DirectX::XMFLOAT3& blowOffVec,
    const BLOW_OFF_FORCE_LEVEL& blowOffForceLevel)
{
    //// 入場中なら吹っ飛ばさない
    //if (activeNode_ != nullptr && "EntryStage" == activeNode_->GetName()) return;

    // 吹っ飛び情報の保存
    blowOffVec_         = blowOffVec;
    blowOffForceLevel_  = blowOffForceLevel;

    isBlowOff_ = true;     // 吹っ飛びフラグを立てる

    // 連続で吹っ飛んだ際にひるみアニメ―ションを
    // 毎回リセットするために適当なアニメーションを入力しておく
    PlayAnimation(0, false, animationSpeed_);
}


void BaseEnemyAI::UpdateNode(const float elapsedTime)
{
    // 現在実行するノードがあれば、ビヘイビアツリーからノードを実行
    if (activeNode_ != nullptr)
    {
        activeNode_ = behaviorTree_->Run(
            activeNode_,
            behaviorData_.get(),
            elapsedTime
        );
    }
    // 現在実行されているノードがなければ、次に実行するノードを推論する
    else
    {
        activeNode_ = behaviorTree_->ActiveNodeInference(
            behaviorData_.get()
        );
    }

}


void BaseEnemyAI::UpdateVelocity(const float elapsedTime)
{
    // 経過フレーム
    const float elapsedFrame = elapsedTime * 60.0f;

    UpdateVerticalVelocity(elapsedFrame);
    UpdateVerticalMove(elapsedTime);

    UpdateHorizontalVelocity(elapsedFrame);
    UpdateHorizontalMove(elapsedTime);
}


// 剣の更新処理
void BaseEnemyAI::UpdateSword(const float elapsedTime)
{
    if (nullptr == sword_) return;

    Transform* swordT = sword_->GetTransform();
    Transform* parentT = model->GetTransform();

    // 位置・回転更新
    {
        // 親の位置と回転を代入
        swordT->SetPosition(parentT->GetPosition());
        swordT->SetRotation(parentT->GetRotation());
    }

    // アニメーション更新
    // ※アニメーション再生はActionDerivedの方で行う
    {
        sword_->UpdateAnimation(elapsedTime);
    }

}


void BaseEnemyAI::UpdateVerticalVelocity(const float elapsedFrame)
{
    velocity_.y += gravity_ * elapsedFrame;
}


void BaseEnemyAI::UpdateVerticalMove(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    XMFLOAT3 position = GetPosition();

    // 落下中
    const float velocityY = velocity_.y * elapsedTime;
    if (velocityY < 0.0f)
    {
        // 地面に接地している
        constexpr float groundPos = 0.0f;
        if (position.y <= groundPos)
        {
            position.y = groundPos;

            // 着地した
            if (!isGround_)
            {
                OnLanding();
            }
            isGround_ = true;
            velocity_.y = 0.0f;
        }
        // 空中に浮いている
        else
        {
            position.y += velocityY;
            isGround_ = false;
        }
    }
    // 上昇中
    else if (velocityY > 0.0f)
    {
        position.y += velocityY;
        isGround_ = false;
    }

    SetPosition(position);
}


void BaseEnemyAI::UpdateHorizontalVelocity(const float elapsedFrame)
{
    const float oldLength = ::sqrtf(
        (velocity_.x * velocity_.x) +
        (velocity_.z * velocity_.z)
    );

    //アニメーションの重みの変更(0~1)
    model->weight = (std::min)(1.0f, oldLength / maxMoveSpeed_);
    if (nullptr != sword_)
    {
        sword_->weight = (std::min)(1.0f, oldLength / maxMoveSpeed_);
    }

    // XZ平面の速力を減速する
    if (oldLength > 0.0f)
    {
        // 摩擦力
        float friction = friction_ * elapsedFrame;

        // 空中にいるときは摩擦力を減らす
        if (!isGround_) friction *= airControl_;

        // 摩擦による横方向の減速処理
        if (oldLength > friction)
        {
            // 単位ベクトル化
            const float vx = velocity_.x / oldLength;
            const float vz = velocity_.z / oldLength;

            velocity_.x -= vx * friction;
            velocity_.z -= vz * friction;
        }
        // 横方向の速度が摩擦力以下になったので速力を無効化
        else
        {
            velocity_.x = 0.0f;
            velocity_.z = 0.0f;
        }
    }

    // XZ平面の速力を加速する
    if (oldLength <= maxMoveSpeed_)
    {
#if 0
        const float moveVecLength = sqrtf(
            moveVec_.x * moveVec_.x +
            moveVec_.z * moveVec_.z
        );
        // 移動ベクトルがゼロベクトルでないなら加速する
        if (moveVecLength > 0.0f)
#else
        const float moveVecLengthSq = {
            moveVec_.x * moveVec_.x +
            moveVec_.z * moveVec_.z
        };
        // 移動ベクトルがゼロベクトルでないなら加速する
        if (moveVecLengthSq > 0.0f)
#endif
        {
            // 加速力
            float acceleration = acceleration_ * elapsedFrame;
            // 空中にいるときは加速力を減らす
            if (!isGround_) acceleration *= airControl_;

            // 移動ベクトルによる加速処理
            velocity_.x += moveVec_.x * acceleration;
            velocity_.z += moveVec_.z * acceleration;

            // 最大速度制限
            float length = ::sqrtf(
                velocity_.x * velocity_.x +
                velocity_.z * velocity_.z
            );
            if (length > maxMoveSpeed_)
            {
                float vx = velocity_.x / length;
                float vz = velocity_.z / length;

                velocity_.x = vx * maxMoveSpeed_;
                velocity_.z = vz * maxMoveSpeed_;
            }
        }
    }
    // 移動ベクトルをリセット
    moveVec_.x = 0.0f;
    moveVec_.z = 0.0f;

}

// #define USE_RAYCAST
void BaseEnemyAI::UpdateHorizontalMove(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    XMFLOAT3 position = GetPosition();

    // 水平速力量計算
    const float velocityLengthXZ = ::sqrtf(
        velocity_.x * velocity_.x +
        velocity_.z * velocity_.z
    );

    if (velocityLengthXZ > 0.0f)
    {
#ifdef USE_RAYCAST
        // 水平移動値
        float velocityX = velocity_.x * elapsedTime;
        float velocityZ = velocity_.z * elapsedTime;

        // レイとの開始位置と終点位置
        DirectX::XMFLOAT3 start = { position.x , position.y + stepOffset , position.z };
        DirectX::XMFLOAT3 end = { position.x + velocityX , position.y + stepOffset , position.z + velocityZ };

        // レイキャストによる壁判定
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            // 壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

            // 壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            //入射ベクトルを法線に射影
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

            // 補正位置の計算
            DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 collectPosition;
            DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

            // 壁ずり方向へレイキャスト
            HitResult hit2;
            if (!Stage::Instance().RayCast(hit.position, collectPosition, hit2))
            {
                // 壁ずり方向で壁に当たらなかったら補正位置に移動
                position.x = collectPosition.x;
                position.z = collectPosition.z;
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }
        }
        else
        {
            // 移動
            position.x += velocityX;
            position.z += velocityZ;
        }

#else
        // 移動
        position.x += velocity_.x * elapsedTime;
        position.z += velocity_.z * elapsedTime;

#endif
    }

    SetPosition(position);
}
#undef USE_RAYCAST


// 目的地点へ移動
void BaseEnemyAI::MoveToTarget(
    const float elapsedTime,
    const float speedRate,
    const bool isLookAtTarget)
{
    using DirectX::XMFLOAT3;

    // ターゲット方向への進行ベクトルを算出
    const XMFLOAT3 position = GetTransform()->GetPosition();
    float vx = targetPosition_.x - position.x;
    float vz = targetPosition_.z - position.z;
    const float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    // 移動処理
    Move(vx, vz, moveSpeed_ * speedRate);
    if (true == isLookAtTarget)
    {
        Turn(elapsedTime, vx, vz, turnSpeed_ * speedRate);
    }
}
