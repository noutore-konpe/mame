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
    loadingPlayer = std::make_unique<LoadingPlayer>();
    titleLogo = std::make_unique<Sprite>(Graphics::Instance().GetDevice(),
        L"./Resources/Image/Title/loading.png");
}

// ������
void SceneLoading::Initialize()
{
    loadingPlayer->Initialize();
    titleLogo->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(800, 480));
    titleLogo->GetSpriteTransform()->SetSize(DirectX::XMFLOAT2(640, 360));

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
    // �`��̏�����
    {
        Graphics& graphics = Graphics::Instance();

        Mame::Scene::BaseScene::RenderInitialize();

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