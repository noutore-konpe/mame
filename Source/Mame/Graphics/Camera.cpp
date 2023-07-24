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