#include "SceneManager.h"


#include "../Graphics/Graphics.h"

#include "../Input/Input.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"

namespace Mame::Scene
{
    // ������
    void SceneManager::Initialize()
    {
    }

    // �X�V����
    void SceneManager::Update(float elapsedTime)
    {
        if (nextScene)
        {
            // �Â��V�[�����I��
            Clear();

            // �V�����V�[����ݒ�
            currentScene = nextScene;
            nextScene = nullptr;

            // �V�[������������(�}���`�X���b�h���������Ă��Ȃ��ꍇ�ɍs��)
            if (!currentScene->IsReady())
            {
                currentScene->CreateResource();
                currentScene->Initialize();
            }
        }

        if (currentScene)
        {
            // �o�ߎ��Ԃ�60�t���[���ȓ��Ȃ�X�V�������s��
            constexpr float frame60 = (0.0166666666666667f);
            //if (elapsedTime <= frame60)
            {

                currentScene->Begin();
                currentScene->Update(elapsedTime);
                currentScene->End();
            }
#ifdef USE_IMGUI
            currentScene->DrawDebug();
            DrawDebug();
#endif
        }
    }

    // �`�揈��
    void SceneManager::Render(float elapsedTime)
    {
        if (!currentScene)return;

        currentScene->Render(elapsedTime);
    }

    // �f�o�b�O�p
    void SceneManager::DrawDebug()
    {
    }

    // �V�[���N���A
    void SceneManager::Clear()
    {
        if (!currentScene)return;

        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
        //SafeDelete(currentScene);
    }

    // �V�[���؂�ւ�
    void SceneManager::ChangeScene(BaseScene* scene)
    {
        // �V�����V�[����ݒ�
        nextScene = scene;
    }
}