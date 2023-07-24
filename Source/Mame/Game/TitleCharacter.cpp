#include "TitleCharacter.h"
#include "../Mame/Graphics/Graphics.h"
#include <DirectXMath.h>

void TitleCharacter::Render(const float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();

    // world行列更新
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model_->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (&model_->keyframe)
    {
        model_->skinned_meshes->render(
            graphics.GetDeviceContext(), 
            transform, 
            DirectX::XMFLOAT4(1,1,1,1), 
            &model_->keyframe
        );
    }
    else
    {
        model_->skinned_meshes->render(
            graphics.GetDeviceContext(), 
            transform, 
            DirectX::XMFLOAT4(1,1,1,1), 
            nullptr
        );
    }
}

void TitleCharacter::DrawDebug()
{
    GetTransform()->DrawDebug();    // ImGui描画
}


void TitleCharacter::CircularMotion(const float elapsedTime)
{
    Transform* transform = GetTransform();

    // 回転させる
    rotationY_ += rotationSpeedY_ * elapsedTime;

    // 単位円（単位ベクトル）を求める
    const float unitCircleX = sinf(rotationY_);  // sinf：中心からX軸に向かう単位円を求める
    const float unitCircleZ = cosf(rotationY_);  // cosf：中心からY軸(Z軸)に向かう単位円を求める

    // 円の中心から単位円の方向に向かって円の半径分進んだ位置を求める
    const float posX = circle_.center.x + unitCircleX * circle_.radius;
    const float posZ = circle_.center.z + unitCircleZ * circle_.radius;

    // 位置を更新
    transform->SetPositionX(posX);
    transform->SetPositionZ(posZ);

    // プレイヤーを進行方向に向かせる
    const float moveVecX = sinf(rotationY_ + ToRadian(-90.0f)); // 進行方向に向かせるために追加で-90度曲げている
    const float moveVecZ = cosf(rotationY_ + ToRadian(-90.0f)); // 進行方向に向かせるために追加で-90度曲げている
    transform->SetRotationY(atan2f(moveVecX, moveVecZ));        // atan2f：座標から角度を求める

    //Turn(elapsedTime, turnVecX, turnVecZ, ToRadian(360.0f));
}


void TitleCharacter::Turn(
    const float elapsedTime,
    NO_CONST float vx,
    NO_CONST float vz,
    NO_CONST float turnSpeed)
{
    // 進行ベクトルがゼロベクトルの場合は処理する必要なし
    if (vx == 0.0f && vz == 0.0f) return;

    turnSpeed *= elapsedTime;

    // 進行ベクトルを単位ベクトル化
    const float vLength = sqrtf((vx * vx) + (vz * vz));
    if (vLength < 0.001f) return;

    // 単位ベクトル化
    vx /= vLength;
    vz /= vLength;

    // 自身の回転値から前方向を求める
    const float frontX = sinf(GetTransform()->GetRotation().y);
    const float frontZ = cosf(GetTransform()->GetRotation().y);

    // 回転角を求めるため、2つの単位ベクトルの内積計算する
    const float dot = (frontX * vx) + (frontZ * vz);

    // 回転角が微小な場合は回転を行わない
    const float angle = acosf(dot); // ラジアン
    //if (fabsf(angle) <= 0.001f) return;
    if (fabsf(angle) <= 0.01f) return;

    // 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
    // 小さいほど1.0に近づくという性質を利用して回転速度を調整する  
    NO_CONST float rot = 1.0f - dot;   // 補正値                    
    if (rot > turnSpeed) rot = turnSpeed;

    // 左右判定を行うために2つの単位ベクトルの外積を計算する
    const float cross = (frontZ * vx) - (frontX * vz);

    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    // 左右判定を行うことによって左右回転を選択する
    GetTransform()->AddRotationY((cross < 0.0f) ? -rot : rot);
}