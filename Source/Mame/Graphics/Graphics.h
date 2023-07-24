#pragma once

#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include <mutex>
#include "../Resource/shader.h"

class Graphics
{
public:
    Graphics(HWND hWnd);
    ~Graphics();

    // インスタンス取得
    static Graphics& Instance() { return *instance; }

    // デバイス取得
    ID3D11Device* GetDevice() const { return device.Get(); }

    // デバイスコンテキスト取得
    ID3D11DeviceContext* GetDeviceContext() const { return immediateContext.Get(); }

    // スワップチェーン取得
    IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

    // レンダーターゲットビュー取得
    ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView.Get(); }

    // デプスステンシルビュー取得
    ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView.Get(); }

    // シェーダー取得
    Shader* GetShader()const { return shader.get(); }

    // スクリーン幅取得
    float GetScreenWidth() const { return screenWidth; }

    // スクリーン高さ取得
    float GetScreenHeight() const { return screenHeight; }

    // ミューテックス取得
    std::mutex& GetMutex() { return mutex; }

private:
    static Graphics* instance;

    Microsoft::WRL::ComPtr<ID3D11Device>            device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     immediateContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain>          swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>         depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  depthStencilView;

    std::unique_ptr<Shader> shader;

    std::mutex mutex;

    float screenWidth;
    float screenHeight;
};

