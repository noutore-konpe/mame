#include "BaseScene.h"

#include "../Graphics/Graphics.h"

namespace Mame::Scene
{
    // 描画の初期化
    void BaseScene::RenderInitialize()
    {
        // 別スレッド中にデバイスコンテキストが使われていた場合に
        // 同時アクセスしないように排他制御する
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