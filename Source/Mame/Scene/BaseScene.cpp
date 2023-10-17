#include "BaseScene.h"

#include "../Graphics/Graphics.h"

namespace Mame::Scene
{
    // �`��̏�����
    void BaseScene::RenderInitialize()
    {
        // �ʃX���b�h���Ƀf�o�C�X�R���e�L�X�g���g���Ă����ꍇ��
        // �����A�N�Z�X���Ȃ��悤�ɔr�����䂷��
        std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());

        Graphics& graphics = Graphics::Instance();

        ID3D11RenderTargetView* renderTargetView = graphics.GetRenderTargetView();
        ID3D11DepthStencilView* depthStencilView = graphics.GetDepthStencilView();

        FLOAT color[]{ 0.0f, 0.0f, 0.0f, 1.0f };
        graphics.GetDeviceContext()->ClearRenderTargetView(renderTargetView, color);
        graphics.GetDeviceContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        graphics.GetDeviceContext()->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
    }
}