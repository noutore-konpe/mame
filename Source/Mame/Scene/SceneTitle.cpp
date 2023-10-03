#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "../Mame.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "../Resource/AudioManager.h"

#include "../Other/misc.h"
#include "../Resource/texture.h"


void SceneTitle::CreateResource()
{
}

// ������
void SceneTitle::Initialize()
{
    // �J����
    Camera::Instance().Initialize();
}

// �I����
void SceneTitle::Finalize()
{
}

// ���t���[����ԍŏ��ɌĂ΂��
void SceneTitle::Begin()
{
}

// �X�V����
void SceneTitle::Update(const float& elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
        Mame::Scene::SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
}

// ���t���[����ԍŌ�ɌĂ΂��
void SceneTitle::End()
{
}

// �`�揈��
void SceneTitle::Render(const float& elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // �`��̏����ݒ�
    {
        // �`��̏����ݒ聦�K���ĂԂ��ƁI�I�I
        Mame::Scene::BaseScene::RenderInitialize();
    }

    // ���f���`��
    {

    }

}


// �f�o�b�O�p
void SceneTitle::DrawDebug()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    
    // ���C�g�Ƃ�
    shader->DrawDebug();
}
