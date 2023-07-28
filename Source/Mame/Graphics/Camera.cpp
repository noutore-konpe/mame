#include "Camera.h"

#include "../Input/Input.h"

#include "../Game/PlayerManager.h"
#include "../Game/EnemyManager.h"

void Camera::Initialize()
{
    transform.SetPosition(DirectX::XMFLOAT3(0.0f, 1.0f, 10.0f));
    transform.SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
    transform.SetRotation(DirectX::XMFLOAT4(0.0f, DirectX::XMConvertToRadians(180), 0.0f, 0.0f));
}

void Camera::UpdateDebug(const float& elapsedTime)
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
    
    DirectX::XMFLOAT2 nowPosition{ static_cast<float>(mouse.GetPositionX()),static_cast<float>(mouse.GetPositionY()) };
    DirectX::XMFLOAT2 oldPosition{ static_cast<float>(mouse.GetOldPositionX()),static_cast<float>(mouse.GetOldPositionY()) };
    DirectX::XMVECTOR nowVector = DirectX::XMLoadFloat2(&nowPosition);
    DirectX::XMVECTOR oldVector = DirectX::XMLoadFloat2(&oldPosition);
    DirectX::XMVECTOR moveVector = DirectX::XMVectorSubtract(nowVector, oldVector);
    DirectX::XMFLOAT2 moveVectorFloat2;
    DirectX::XMStoreFloat2(&moveVectorFloat2, moveVector);

    moveVectorFloat2.x *= elapsedTime * rotationSpeed;
    moveVectorFloat2.y *= elapsedTime * rotationSpeed;

    rotation.x += DirectX::XMConvertToRadians(moveVectorFloat2.y);
    rotation.y += DirectX::XMConvertToRadians(moveVectorFloat2.x);

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
    
    DirectX::XMFLOAT3 pos = transform.GetPosition();
    DirectX::XMFLOAT3 forward = transform.CalcForward();
    DirectX::XMVECTOR eye{ DirectX::XMVectorSet(pos.x,pos.y,pos.z,1.0f) };
    DirectX::XMVECTOR focus{ DirectX::XMVectorSet(pos.x + forward.x,pos.y + forward.y,pos.z + forward.z,1.0f) };
    //DirectX::XMVECTOR eye{ DirectX::XMVectorSet(camera.eye.x,camera.eye.y,camera.eye.z,1.0f) };
    //DirectX::XMVECTOR focus{ DirectX::XMVectorSet(camera.focus.x,camera.focus.y,camera.focus.z,1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(camera.up.x,camera.up.y,camera.up.z,0.0f) };
    V = { DirectX::XMMatrixLookAtLH(eye, focus, up) };

    //DebugMoveCamera();
}

void Camera::DrawDebug()
{
    ImGui::Begin("Camera");

    transform.DrawDebug();

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

    // todo : camera
    // デバッグ用にカメラを動かしやすいようにしている
    ImGui::DragFloat("camera_position.x", &camera.eye.x);
    ImGui::DragFloat("camera_position.y", &camera.eye.y);


    if (ImGui::Button("Reset"))Camera::Reset();

    ImGui::End();
}

void Camera::Reset()
{
    camera.eye = { 0.0f,0.0f,-10.0f };
}