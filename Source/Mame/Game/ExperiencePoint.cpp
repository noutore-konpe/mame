#include "ExperiencePoint.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../Graphics/Graphics.h"
#include "../Scene/SceneGame.h"
#include "ExperiencePointManager.h"
#include "PlayerManager.h"
#include "../Resource/texture.h"

int ExperiencePoint::nameNum_ = 0;

ExperiencePoint::ExperiencePoint()
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(
        graphics.GetDevice(),
        "./Resources/Model/cube.fbx"
    );

    D3D11_TEXTURE2D_DESC texture2dDesc;
    ::load_texture_from_file(graphics.GetDevice(),
        L"./Resources/Image/Mask/noise3.png",
        emissiveTexture.GetAddressOf(),
        &texture2dDesc);

    ::CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/ExpModelPS.cso",
        expModelPS.GetAddressOf());

    SetName("Exp_" + std::to_string(nameNum_++));
}


ExperiencePoint::~ExperiencePoint()
{
    --nameNum_;
}


void ExperiencePoint::Initialize()
{
    GetTransform()->SetScaleFactor(0.25f);

    step_ = STEP::FALL_INIT;
}


void ExperiencePoint::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // 寿命更新
    {
        ExperiencePointManager& expManager = ExperiencePointManager::Instance();
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) { expManager.Remove(this); }

        // 点滅
        {
            const int blinkTimer = static_cast<int>(lifeTimer_ * 100.0f);
            if (lifeTimer_ <= 4)
            {
                model_->color.w = (blinkTimer & 0x08) ? 1.0f : 0.25f;
            }
            else if (lifeTimer_ <= 8)
            {
                model_->color.w = (blinkTimer & 0x10) ? 1.0f : 0.25f;
            }
        }

        // 魅せるタイマー更新
        attractTimer_ = (std::max)(0.0f, attractTimer_ - elapsedTime);

    }

    PlayerManager& plManager    = PlayerManager::Instance();
    Transform*     t            = GetTransform();
    const float    elapsedFrame = elapsedTime * 60.0f;

    switch (step_)
    {
    case STEP::FALL_INIT:

        if (velocity_.y < 0.0f) velocity_.y = 0.0f; // すでに落下していたら速度をリセット
        isGround_       = false;                    // 空中にいる
        isMoveToPlayer_ = false;                    // プレイヤーに向かっていない

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
                UpdateVerticalVelocity(elapsedFrame);
                t->AddPositionY(velocity_.y * elapsedTime);

                // 上昇中でなく円運動の開始地点まで降下していたら浮遊状態に移行する
                if (velocity_.y <= 0.0f &&
                    t->GetPositionY() <= circularMotion_.center_.y)
                {
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
        t->SetPositionY(circularMotion_.center_.y); // 円運動の開始地点Yに位置設定
#if 0
        {
            const float localPositionX = t->GetPositionX() - circularMotion_.center_.x;
            const float localPositionY = t->GetPositionY() - circularMotion_.center_.y;
            circularMotion_.angle_.z = ::atan2f(localPositionX, localPositionY); // return Radian Angle
        }
#else
        // 中心の位置から開始するので90度回転した状態で始める
        circularMotion_.rotation_.z = ToRadian(90.0f);
#endif

        isGround_       = true;     // 地面にいる
        isMoveToPlayer_ = false;    // プレイヤーに向かっていない

        step_ = STEP::FLOTING;
        [[fallthrough]];
    case STEP::FLOTING:

        // プレイヤーが見つかったらプレイヤーの方へ向かう
        if (true == SearchPlayer())
        {
            step_ = STEP::MOVE_TO_PLAYER_INIT;
            break;
        }

        // 空中にいたら落下状態に移行する(+α許容値)
        if (t->GetPositionY() > circularMotion_.center_.y + 0.1f)
        {
            step_ = STEP::FALL_INIT;
            break;
        }

        // XZ位置更新
        {
            UpdateHorizontalVelocity(elapsedFrame);
            UpdateHorizontalMove(elapsedTime);
        }

        // Z軸回転
        {
            circularMotion_.rotation_.z += circularMotion_.addRotate_ * elapsedTime;
            if (circularMotion_.rotation_.z > 360.0f) { circularMotion_.rotation_.z -= 360.0f; }
            t->SetPositionY(
                circularMotion_.center_.y +
                ::cosf(circularMotion_.rotation_.z) *
                circularMotion_.radius_
            );
        }

        break;
    case STEP::MOVE_TO_PLAYER_INIT:

        isGround_       = false;    // 空中にいる
        isMoveToPlayer_ = true;     // プレイヤーに向かっていない

        step_ = STEP::MOVE_TO_PLAYER;
        [[fallthrough]];
    case STEP::MOVE_TO_PLAYER:

        UpdateHorizontalVelocity(elapsedFrame); // 減速

        // プレイヤーから離れていたらほかの状態に移る
        if (false == SearchPlayer())
        {
            // 位置が地面以下なら浮遊状態に移行する
            if (t->GetPositionY() <= circularMotion_.center_.y)
            {
                step_ = STEP::FLOTING_INIT;
                break;
            }

            // 空中にいたら落下状態に移行する(+α許容値)
            if (t->GetPositionY() > circularMotion_.center_.y + 0.1f)
            {
                step_ = STEP::FALL_INIT;
                break;
            }
        }
        else
        {
            // プレイヤーの方へ向かう
            {
                const XMFLOAT3& pos      = GetTransform()->GetPosition();
                      XMFLOAT3  plPos    = plManager.GetPlayer()->GetTransform()->GetPosition();
                                plPos.y += plManager.GetPlayer()->GetHeight() * 0.5f;
                const XMFLOAT3  vecN     = ::XMFloat3Normalize(plPos - pos);

                velocity_ += vecN * acceleration_ * elapsedFrame; // 仮(プレイヤーに依存にする予定)
                t->AddPosition(velocity_ * elapsedTime);
            }

            // 地面を貫通しないようにする
            if (t->GetPositionY() < 0.0f)
            {
                t->SetPositionY(0.0f);
                velocity_.y = velocity_.y * 0.5f; // Y速度を半減させる
            }
        }

        break;
    }

    // 回転
    t->AddRotationY(::ToRadian(45.0f) * elapsedTime);

}


void ExperiencePoint::Render(const float scale, ID3D11PixelShader* psShader)
{
    Graphics::Instance().GetShader()->SetRasterizerState(
        static_cast<int>(Shader::RASTER_STATE::SOLID)
    );

    Graphics::Instance().GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    //定数バッファー更新
    UpdateConstants();

    model_->Render(scale, expModelPS.Get());

    // 当たり判定球描画
#ifdef _DEBUG
    if (true == SceneGame::isDispCollision_)
    {
        DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
        debugRenderer->DrawSphere(GetTransform()->GetPosition(), radius_, { 1,1,1,1 });
    }
#endif
}


void ExperiencePoint::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(GetName().c_str()))
    {
        model_->DrawDebug();

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}

void ExperiencePoint::UpdateConstants()
{
    SetEmissiveIntensity(3.0f);
    SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));
    SetEmissiveColor(DirectX::XMFLOAT4(1.0f, 1.0f, 0.3f, 1.0f));
}


const bool ExperiencePoint::SearchPlayer()
{
    // 経験値が出る演出時間中は飛ばす
    if (attractTimer_ > 0.0f) return false;

    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();

    const XMFLOAT3& pos       = this->GetTransform()->GetPosition();
    XMFLOAT3        plPos     = plManager.GetPlayer()->GetTransform()->GetPosition();
                    plPos.y  += plManager.GetPlayer()->GetHeight() * 0.5f;
    const XMFLOAT3  vec       = plPos - pos;
    const float     lengthSq  = ::XMFloat3LengthSq(vec);

    constexpr float plCollectExpLengthSq = 6.0f * 6.0f; // 仮(プレイヤーに依存する予定)
    if (lengthSq > plCollectExpLengthSq) return false;

    return true;

}


void ExperiencePoint::UpdateVerticalVelocity(const float elapsedFrame)
{
    velocity_.y += gravity_ * elapsedFrame;
    constexpr float velocityY_min = -1.0f;
    if (velocity_.y < velocityY_min) velocity_.y = velocityY_min;
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

        // プレイヤーに向かっている場合は摩擦を強める
        // (プレイヤーの周りをぐるぐるしすぎて遠心力でプレイヤーの収集距離外に出てしまって遠くに吹っ飛んでいかないようにするため)
        if (true == isMoveToPlayer_) friction = 0.2f * elapsedFrame;

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