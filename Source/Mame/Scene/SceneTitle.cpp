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
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
    ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

    FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
    immediate_context->ClearRenderTargetView(render_target_view, color);
    immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

    RenderContext rc;
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

    Shader* shader = graphics.GetShader();
    //shader->Begin(graphics.GetDeviceContext(), rc, 0);
}


// �f�o�b�O�p
void SceneTitle::DrawDebug()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();
    
    // ���C�g�Ƃ�
    shader->DrawDebug();
}
