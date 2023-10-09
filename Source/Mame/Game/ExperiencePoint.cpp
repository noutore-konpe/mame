#include "ExperiencePoint.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../Graphics/Graphics.h"
#include "PlayerManager.h"

int ExperiencePoint::nameNum_ = 0;

ExperiencePoint::ExperiencePoint()
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(
        graphics.GetDevice(),
        "./Resources/Model/cube.fbx"
    );

    SetName("Exp_" + std::to_string(nameNum_++));
}


ExperiencePoint::~ExperiencePoint()
{
    --nameNum_;
}


void ExperiencePoint::Initialize()
{
    GetTransform()->SetScaleFactor(3.0f);

    step_ = STEP::FALL_INIT;

}


void ExperiencePoint::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();
    Transform* t = GetTransform();
    const float elapsedFrame = elapsedTime * 60;

    UpdateGroundPosition(); // 円運動の地面位置更新

    switch (step_)
    {
    case STEP::FALL_INIT:

        velocity_.y = 0.0f; // 落下速度リセット

        step_ = STEP::FALL;
        [[fallthrough]];
    case STEP::FALL:

        // プレイヤーが見つかったらプレイヤーの方へ向かう
        if (true == SearchPlayer())
        {
            step_ = STEP::MOVE_TO_PLAYER_INIT;
            break;
        }

        // 位置更新
        {
            // Y位置更新
            {
                t->AddPositionY(gravity_ * elapsedFrame);

                // 円運動の開始地点まで降下したら浮遊状態に移行する
                if (t->GetPositionY() <= circularMotion_.groundPositionY_)
                {
                    t->SetPositionY(circularMotion_.groundPositionY_);

                    step_ = STEP::FLOTING_INIT;
                    break;
                }
            }

            // XZ位置更新
            {
                UpdateHorizontalVelocity(elapsedFrame);
                UpdateHorizontalMove(elapsedTime);
            }
        }

        break;
    case STEP::FLOTING_INIT:

        // 自分の位置から回転を始めるようにする
        {
            t->SetPositionY(circularMotion_.groundPositionY_); // 円運動の開始地点Yに位置設定
            const float localPositionX = t->GetPositionX() - circularMotion_.center_.x;
            const float localPositionY = t->GetPositionY() - circularMotion_.center_.y;
            circularMotion_.angle_.z = ::atan2f(localPositionX, localPositionY); // return Radian Angle
        }

        step_ = STEP::FLOTING;
        [[fallthrough]];
    case STEP::FLOTING:

        // プレイヤーが見つかったらプレイヤーの方へ向かう
        if (true == SearchPlayer())
        {
            step_ = STEP::MOVE_TO_PLAYER_INIT;
            break;
        }

        // 空中にいたら落下状態に移行する
        if (t->GetPositionY() >= circularMotion_.outGroundPositionY_)
        {
            step_ = STEP::FALL_INIT;
            break;
        }

        // XZ位置更新
        {
            UpdateHorizontalVelocity(elapsedFrame);
            UpdateHorizontalMove(elapsedTime);
        }

        // Y軸回転
        {
            circularMotion_.angle_.z += circularMotion_.addRotate_ * elapsedTime;
            if (circularMotion_.angle_.z > 360.0f) { circularMotion_.angle_.z -= 360.0f; }
            t->SetPositionY(
                circularMotion_.center_.y +
                ::cosf(circularMotion_.angle_.z) *
                circularMotion_.radius_
            );
        }

        break;
    case STEP::MOVE_TO_PLAYER_INIT:

        step_ = STEP::MOVE_TO_PLAYER;
        [[fallthrough]];
    case STEP::MOVE_TO_PLAYER:

        // プレイヤーから離れていたらほかの状態に移る
        if (false == SearchPlayer())
        {
            // 位置が地面以下なら浮遊状態に移行する
            if (t->GetPositionY() <= circularMotion_.groundPositionY_)
            {
                t->SetPositionY(circularMotion_.groundPositionY_);

                step_ = STEP::FLOTING_INIT;
                break;
            }

            // 空中にいたら落下状態に移行する
            if (t->GetPositionY() >= circularMotion_.outGroundPositionY_)
            {
                step_ = STEP::FALL_INIT;
                break;
            }
        }
        else
        {
            // プレイヤーの方へ向かう
            const XMFLOAT3 pos    = GetTransform()->GetPosition();
            const XMFLOAT3 plPos  = plManager.GetPlayer()->GetTransform()->GetPosition();
            const XMFLOAT3 vecN = ::XMFloat3Normalize(plPos - pos);
            velocity_ += 1.0f * elapsedTime; // 仮(プレイヤーに依存にする予定)
            t->AddPosition(vecN * velocity_ * elapsedTime);
        }

        break;
    }
}


void ExperiencePoint::Render(const float scale, ID3D11PixelShader* psShader)
{
    Graphics::Instance().GetShader()->SetRasterizerState(
        static_cast<int>(Shader::RASTER_STATE::SOLID)
    );

    model_->Render(scale, psShader);
}


void ExperiencePoint::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(GetName()))
    {
        model_->DrawDebug();

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}


const bool ExperiencePoint::SearchPlayer()
{
    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();

    const XMFLOAT3 pos      = GetTransform()->GetPosition();
    const XMFLOAT3 plPos    = plManager.GetPlayer()->GetTransform()->GetPosition();
    const XMFLOAT3 vec      = plPos - pos;
    const float    lengthSq = ::XMFloat3LengthSq(vec);

    constexpr float plCollectExpLengthSq = 2.0f * 2.0f; // 仮(プレイヤーに依存する予定)
    if (lengthSq > plCollectExpLengthSq) return false;

    return true;

}


void ExperiencePoint::UpdateHorizontalVelocity(const float elapsedFrame)
{
    const float velocityXZLength = ::sqrtf(
        (velocity_.x * velocity_.x) +
        (velocity_.z * velocity_.z)
    );

    if (velocityXZLength > 0.0f)
    {
        // 摩擦力
        float friction = friction_ * elapsedFrame;

        // 空中にいるときは摩擦力を減らす
        if (!isGround_) friction *= airControl_;

        // 摩擦による横方向の減速処理
        if (velocityXZLength > friction)
        {
            // 単位ベクトル化
            const float vx = velocity_.x / velocityXZLength;
            const float vz = velocity_.z / velocityXZLength;

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

}

void ExperiencePoint::UpdateHorizontalMove(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    Transform* t = GetTransform();

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
        t->AddPositionX(velocity_.x * elapsedTime);
        t->AddPositionZ(velocity_.z * elapsedTime);

#endif
    }

}


void ExperiencePoint::UpdateGroundPosition()
{
    circularMotion_.groundPositionY_ = {
        circularMotion_.center_.y +
        circularMotion_.radius_
    };

    circularMotion_.outGroundPositionY_ = {
        circularMotion_.groundPositionY_ +
        circularMotion_.correct_
    };
}
