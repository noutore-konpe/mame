#include "SceneLoading.h"

#include "../Graphics/Graphics.h"
#include "SceneManager.h"

SceneLoading::SceneLoading(BaseScene* nextScene) :nextScene(nextScene) 
{
    Graphics& graphics = Graphics::Instance();

    // �V�[���̑�����ݒ�
    SetSceneType(static_cast<int>(Mame::Scene::TYPE::LOAD));
}

void SceneLoading::CreateResource()
{
}

// ������
void SceneLoading::Initialize()
{
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
    if (nextScene->IsReady())
        Mame::Scene::SceneManager::Instance().ChangeScene(nextScene);
}

// Update�̌�ɌĂ΂��
void SceneLoading::End()
{
}

// �`�揈��
void SceneLoading::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* deviceContext = graphics.GetDeviceContext();
    ID3D11RenderTargetView* renderTargetView = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depthStencileView = graphics.GetDepthStencilView();

    // ��ʃN���A&�����_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,1.0f,0.0f,1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, color);
    deviceContext->ClearDepthStencilView(depthStencileView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencileView);
}

void SceneLoading::DrawDebug()
{
#ifdef USE_IMGUI
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