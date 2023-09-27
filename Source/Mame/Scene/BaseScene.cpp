#include "BaseScene.h"

#include "../Graphics/Graphics.h"

namespace Mame::Scene
{
    // •`‰æ‚Ì‰Šú‰»
    void BaseScene::RenderInitialize()
    {
        Graphics& graphics = Graphics::Instance();

        ID3D11RenderTargetView* renderTargetView = graphics.GetRenderTargetView();
        ID3D11DepthStencilView* depthStencilView = graphics.GetDepthStencilView();

        FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
        graphics.GetDeviceContext()->ClearRenderTargetView(renderTargetView, color);
        graphics.GetDeviceContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        graphics.GetDeviceContext()->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
    }
}