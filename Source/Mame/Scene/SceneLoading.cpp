#include "SceneLoading.h"

#include "../Graphics/Graphics.h"
#include "SceneManager.h"

#include "../Other/Easing.h"
#include "../Other/misc.h"
#include "../framework.h"

SceneLoading::SceneLoading(BaseScene* nextScene) :nextScene(nextScene) 
{
    Graphics& graphics = Graphics::Instance();


    // �V�[���̑�����ݒ�
    SetSceneType(static_cast<int>(Mame::Scene::TYPE::LOAD));
}

void SceneLoading::CreateResource()
{
    loadingPlayer = std::make_unique<LoadingPlayer>();
    titleLogo = std::make_unique<Sprite>(Graphics::Instance().GetDevice(),
        L"./Resources/Image/Title/loading.png");

    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(PlayerConstants);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    hr = Graphics::Instance().GetDevice()->CreateBuffer(&desc, nullptr,
        playerConstant.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    // �萔�o�b�t�@�[
    {
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.ByteWidth = sizeof(Shader::SceneConstants);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        // SceneConstants
        hr = Graphics::Instance().GetDevice()->CreateBuffer(&bufferDesc, nullptr,
            ConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
}

// ������
void SceneLoading::Initialize()
{
    loadingPlayer->Initialize();
    titleLogo->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(800, 480));
    titleLogo->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(640, 360));

    isChangeScene = false;
    easingTimer = 0.0f;
    playerConstants.color = { 0.3f, 0.3f, 0.3f, 1.0f };

    // �X���b�h�J�n
    // std::thread(LoadingThread, this);
    // ��ڂ̈�����LoadingThread�̈����ɂȂ�u
    thread = new std::thread(LoadingThread, this);
}

// �I����
void SceneLoading::Finalize()
{
    // �X���b�h�I����
    thread->join();
    if (thread != nullptr)
    {
        delete thread;
        thread = nullptr;
    }
}

// Update�̑O�ɌĂ΂��֐�
void SceneLoading::Begin()
{
}

// �X�V����
void SceneLoading::Update(const float& elapsedTime)
{
    loadingPlayer->Update(elapsedTime);

#if 1
    if (nextScene->IsReady())
    {
        float maxTime = 1.0f;
        if (easingTimer <= maxTime)
        {
            float playerColor = Easing::InSine(easingTimer, maxTime, 0.0f, 0.3f);
            float SpriteColor = Easing::InSine(easingTimer, maxTime, 0.0f, 1.0f);

            playerConstants.color = { playerColor, playerColor, playerColor, 1.0f };
            titleLogo->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(SpriteColor, SpriteColor, SpriteColor, 1.0f));

            easingTimer += elapsedTime;
        }
        else
        {
            playerConstants.color = { 0.0f, 0.0f, 0.0f, 1.0f };
            titleLogo->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
            isChangeScene = true;
        }
    }

    // �V�[���؂�ւ�
    if (isChangeScene)
    {
        Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);
    }
#endif
}

// Update�̌�ɌĂ΂��
void SceneLoading::End()
{
}

// �`�揈��
void SceneLoading::Render(const float& elapsedTime)
{
    // �`��̏�����
    {
        Graphics& graphics = Graphics::Instance();
        Shader* shader = graphics.GetShader();

        Mame::Scene::BaseScene::RenderInitialize();

        Camera& camera = Camera::Instance();
        Camera::Instance().TitleSetPerspectiveFov(graphics.GetDeviceContext());

        Shader::SceneConstants sceneConstants{};
        DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
        sceneConstants.lightDirection = shader->GetViewPosition();
        sceneConstants.cameraPosition = shader->GetViewCamera();

        DirectX::XMStoreFloat4x4(&sceneConstants.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
        DirectX::XMStoreFloat4x4(&sceneConstants.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
        sceneConstants.time = framework::tictoc.time_stamp();

        graphics.GetDeviceContext()->UpdateSubresource(ConstantBuffer.Get(), 0, 0, &sceneConstants, 0, 0);
        graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());
        graphics.GetDeviceContext()->PSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());

        graphics.GetDeviceContext()->UpdateSubresource(playerConstant.Get(), 0, 0, &playerConstants, 0, 0);
        graphics.GetDeviceContext()->PSSetConstantBuffers(5, 1, playerConstant.GetAddressOf());
        loadingPlayer->Render(0.01f);
        titleLogo->Render();
    }
}

void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
    Graphics::Instance().GetShader()->DrawDebug();

    loadingPlayer->DrawDebug();

    titleLogo->DrawDebug();
#endif// USE_IMGUI
}

// ���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    // COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    std::ignore = CoInitialize(nullptr); // std::ignore�ŕԂ�l�x������

    // ���̃V�[���̏��������s��
    scene->nextScene->CreateResource();
    scene->nextScene->Initialize();

    // �X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    // ���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}