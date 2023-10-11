#include "Camera.h"

#include "../Input/Input.h"
#include "../Other/MathHelper.h"
#include "../Other/Easing.h"
#include "../Game/PlayerManager.h"

void Camera::Initialize()
{
    transform.SetPosition(DirectX::XMFLOAT3(0.0f, 1.0f, 10.0f));
    transform.SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
    transform.SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));

    activeLockOn = false;
}

void Camera::Update(float elapsedTime)
{
    if (activeLockOn)
    {
        EyeMoveDelayUpdate(elapsedTime, PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition());
        FocusMoveDelayUpdate(elapsedTime, lockOnTargetTransform->GetPosition());
    }
    else
    {
        EyeMoveDelayUpdate(elapsedTime, PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition());
        FocusMoveDelayUpdate(elapsedTime,GetTransform()->CalcForward());
    }
}

void Camera::UpdateDebug(const float& elapsedTime, DirectX::XMFLOAT2 moveVector)
{
#if 1
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    DirectX::XMFLOAT3 position = transform.GetPosition();
    DirectX::XMFLOAT4 rotation = transform.GetRotation();

    DirectX::XMFLOAT3 forward = transform.CalcForward();
    DirectX::XMFLOAT3 right = transform.CalcRight();

    float ax = gamePad.GetAxisLX() * moveSpeed;
    float ay = gamePad.GetAxisLY() * moveSpeed;

    moveVector.x *= elapsedTime * rotationSpeed;
    moveVector.y *= elapsedTime * rotationSpeed;

    rotation.x += DirectX::XMConvertToRadians(moveVector.y);
    rotation.y += DirectX::XMConvertToRadians(moveVector.x);

    forward.x *= ay;
    forward.y *= ay;
    forward.z *= ay;
    right.x *= ax;
    right.y *= ax;
    right.z *= ax;

    position.x += forward.x + right.x;
    //position.y += forward.y + right.y;
    position.z += forward.z + right.z;

    if (rotation.y >= DirectX::XMConvertToRadians(360))rotation.y -= DirectX::XMConvertToRadians(360);
    if (rotation.y <= DirectX::XMConvertToRadians(0))rotation.y += DirectX::XMConvertToRadians(360);

    transform.SetPosition(position);
    transform.SetRotation(rotation);
#else

    GamePad& gamePad = Input::Instance().GetGamePad();

    float aLx = gamePad.GetAxisLX();
    float aLy = gamePad.GetAxisLY();

    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();
    DirectX::XMFLOAT4 rotation = GetTransform()->GetRotation();

    float speed = elapsedTime * 10;
    if (rotation.y > DirectX::XMConvertToRadians(90) &&
        rotation.y < DirectX::XMConvertToRadians(270))
        speed *= (speed > 0) ? -1 : 1;
    else
        speed *= (speed < 0) ? -1 : 1;

    if (aLx <= -0.3f)position.x -= speed;   // 左
    if (aLx >=  0.3f)position.x += speed;   // 右
    if (aLy <= -0.3f)position.z -= speed;   // 後
    if (aLy >=  0.3f)position.z += speed;   // 前

    if (rotation.y >= DirectX::XMConvertToRadians(360))rotation.y -= DirectX::XMConvertToRadians(360);
    if (rotation.y <= DirectX::XMConvertToRadians(0))rotation.y += DirectX::XMConvertToRadians(360);

    GetTransform()->SetPosition(position);
    GetTransform()->SetRotation(rotation);
#endif
}


void Camera::SetPerspectiveFov(ID3D11DeviceContext* dc)
{
    D3D11_VIEWPORT viewport{};
    UINT unm_viewports{ 1 };
    dc->RSGetViewports(&unm_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,1000.0f) };

    DirectX::XMVECTOR eye;
    DirectX::XMVECTOR focus;

    //TANA変更
    if (activeLockOn)//ロックオン時
    {
        DirectX::XMFLOAT3 targetPos = focusTarget->GetPosition();
        DirectX::XMFLOAT3 lockOnTargetPos = lockOnTargetTransform->GetPosition();
        DirectX::XMFLOAT3 forward = lockOnTargetPos - targetPos;
        forward = Normalize(forward);
        lockOnForward = forward;

        eye = { DirectX::XMVectorSet(
            eyePos.x - forward.x * focalLength,
            eyePos.y - forward.y * focalLength + 1.0f,
            eyePos.z - forward.z * focalLength,
            1.0f) };

        focus = { DirectX::XMVectorSet(
           focusPos.x,
           focusPos.y + focusOffsetY,
           focusPos.z,
           1.0f) };

        focus = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&screenVibrationOffset), focus);
    }
    if (focusTarget && !enableDebugCamera)
    {
        //DirectX::XMFLOAT3 pos = transform.GetPosition();
        DirectX::XMFLOAT3 targetPos = focusTarget->GetPosition();
        DirectX::XMFLOAT3 forward = GetForward();

        eye = { DirectX::XMVectorSet(
            eyePos.x - forward.x * focalLength,
            eyePos.y - forward.y * focalLength + 1.0f,
            eyePos.z - forward.z * focalLength,
            1.0f) };

        focus = { DirectX::XMVectorSet(
            targetPos.x,
            targetPos.y + focusOffsetY,
            targetPos.z,
            1.0f) };

        focus = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&screenVibrationOffset), focus);
    }
    else
    {
        DirectX::XMFLOAT3 pos = transform.GetPosition();
        DirectX::XMFLOAT3 forward = transform.CalcForward();
        eye = { DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f) };
        focus = { DirectX::XMVectorSet(pos.x + forward.x, pos.y + forward.y, pos.z + forward.z, 1.0f) };
    }

    //DirectX::XMVECTOR eye{ DirectX::XMVectorSet(camera.eye.x,camera.eye.y,camera.eye.z,1.0f) };
    //DirectX::XMVECTOR focus{ DirectX::XMVectorSet(camera.focus.x,camera.focus.y,camera.focus.z,1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(camera.up.x,camera.up.y,camera.up.z,0.0f) };
    V = { DirectX::XMMatrixLookAtLH(eye, focus, up) };

    //DebugMoveCamera();
}

void Camera::DrawDebug()
{
    if (ImGui::BeginMenu("Camera"))
    {
        //ImGui::Begin("Camera");

        transform.DrawDebug();

        ImGui::Checkbox("Enable Debug Camera", &enableDebugCamera);

        if (ImGui::TreeNode("DebugCamera"))
        {
            ImGui::DragFloat("moveSpeed", &moveSpeed);
            ImGui::DragFloat("rotationSpeed", &rotationSpeed);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("camera_information"))
        {
            ImGui::DragFloat3("eye", &camera.eye.x);
            ImGui::DragFloat3("focus", &camera.focus.x);
            ImGui::DragFloat3("up", &camera.up.x);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("player camera"))
        {
            ImGui::Checkbox("Lock On", &activeLockOn);

            ImGui::DragFloat3("Velocity", &eyeVelocity.x);
            ImGui::DragFloat3("Eye Pos", &eyePos.x);
            ImGui::SliderFloat("Max Speed", &maxEyeSpeed,0.1f,20.0f);
            ImGui::SliderFloat("Acceleration", &acceleration,0.1f,10.0f);

            ImGui::SliderFloat("FocalLength", &focalLength, 0.1f, 20.0f);
            ImGui::SliderFloat("OffsetY", &offsetY, 0.1f, 10.0f);
            ImGui::SliderFloat("FocusOffsetY", &focusOffsetY, 0.1f, 10.0f);
            ImGui::TreePop();
        }

        static float vibVolume;
        static float vibTime;
        if (ImGui::TreeNode("Vibration"))
        {
            ImGui::SliderFloat("VibrationVolume", &vibVolume, 0.0f, 1.0f);
            ImGui::SliderFloat("VibrationTime", &vibTime, 0.0f, 5.0f);
            if (ImGui::Button("Vibrate"))
            {
                ScreenVibrate(vibVolume, vibTime);
            }
            ImGui::TreePop();
        }

        // todo : camera
        // デバッグ用にカメラを動かしやすいようにしている
        ImGui::DragFloat("camera_position.x", &camera.eye.x);
        ImGui::DragFloat("camera_position.y", &camera.eye.y);


        if (ImGui::Button("Reset"))Camera::Reset();

        //ImGui::End();

        ImGui::EndMenu();
    }
    ImGui::Separator();

}

void Camera::Reset()
{
    camera.eye = { 0.0f,0.0f,-10.0f };
}

void Camera::EyeMoveDelayUpdate(float elapsedTime,
    const DirectX::XMFLOAT3 eyeTargetPos)
{
    
    DirectX::XMFLOAT3 moveVec = eyeTargetPos - eyePos;
    float length = Length(moveVec);
    moveVec = Normalize(moveVec);

    //カメラ位置と目標位置までの距離が近いなら処理しない
    if (length < 0.1f && length > -0.1f)return;

    eyeVelocity = moveVec * length * acceleration;

    //最大速度制限
    length = Length(eyeVelocity);
    if (length > maxEyeSpeed)
    {
        eyeVelocity = moveVec * maxEyeSpeed;

        length = maxEyeSpeed;
    }

    eyePos = eyePos + eyeVelocity * elapsedTime;
}

void Camera::FocusMoveDelayUpdate(float elapsedTime,
    const DirectX::XMFLOAT3 focusTargetPos)
{

    DirectX::XMFLOAT3 moveVec = focusTargetPos - focusPos;
    float length = Length(moveVec);
    moveVec = Normalize(moveVec);

    //カメラ位置と目標位置までの距離が近いなら処理しない
    if (length < 0.1f && length > -0.1f)return;

    focusVelocity = moveVec * length * acceleration;

    //最大速度制限
    length = Length(focusVelocity);
    if (length > maxFocusSpeed)
    {
        focusVelocity = moveVec * maxFocusSpeed;
        length = maxFocusSpeed;
    }

    focusPos = focusPos + focusVelocity * elapsedTime;
    
}


void Camera::ScreenVibrate(float volume, float effectTime)
{
    vibrationVolume = volume;
    vibrationTimer = effectTime;
    vibrationTime = effectTime;
}

void Camera::ScreenVibrationUpdate(float elapsedTime)
{
    screenVibrationOffset = {};
    if (vibrationTimer <= 0)return;

    //振動方向の指定(乱数)
    DirectX::XMFLOAT3 vibVec;
    auto right = GetTransform()->CalcRight();
    auto up = GetTransform()->CalcUp();

    right = right * (rand() % 100 - 50.0f);
    up = up * (rand() % 100 - 50.0f);

    vibVec = {
        right.x + up.x,
        right.y + up.y,
        0.0f
    };
    vibVec = Normalize(vibVec);

    //イージングを使い経過時間で振動量を調整する
    float vibrationVolume = Easing::InSine(vibrationTimer, vibrationTime, this->vibrationVolume, 0.0f);

    //振動値を入れる
    screenVibrationOffset = vibVec * vibrationVolume;

    vibrationTimer -= elapsedTime;
}