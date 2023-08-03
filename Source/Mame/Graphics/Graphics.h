#pragma once

#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include <mutex>
#include "../Resource/shader.h"

#define ENABLE_DIRECT2D
#ifdef ENABLE_DIRECT2D
#include <d2d1_1.h>
#include <dwrite.h>
#include <dxgi1_3.h>
#include <dxgi1_6.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#endif

// フルスクリーン作ってる途中
#define FULLSCREEN_CREATE 1

class Graphics
{
public:
    Graphics(HWND hWnd, BOOL fullscreen);
    ~Graphics();

    // インスタンス取得
    static Graphics& Instance() { return *instance; }

    ID3D11Device* GetDevice() const { return device.Get(); }
    ID3D11DeviceContext* GetDeviceContext() const { return deviceContext.Get(); }
#if FULLSCREEN_CREATE
    IDXGISwapChain* GetSwapChain() const { return swapchain0.Get(); }
#else
    IDXGISwapChain1* GetSwapChain() const { return swapchain.Get(); }
#endif
    ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView.Get(); }
    ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView.Get(); }

    // シェーダー取得
    Shader* GetShader()const { return shader.get(); }

    float GetScreenWidth() const { return screenWidth; }
    float GetScreenHeight() const { return screenHeight; }

    // ミューテックス取得
    std::mutex& GetMutex() { return mutex; }



private:
    static Graphics* instance;

    Microsoft::WRL::ComPtr<ID3D11Device>            device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     deviceContext;
    // todo : どっちかいらなくなる
    Microsoft::WRL::ComPtr<IDXGISwapChain>          swapchain0;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>          swapchain;

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>         depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  depthStencilView;
    void CreateSwapChain(IDXGIFactory6* dxgiFactory6, const HWND hwnd);

    std::unique_ptr<Shader> shader;

    std::mutex mutex;

    float screenWidth;
    float screenHeight;

public:
    //-----//
    SIZE framebufferDimensions;

    Microsoft::WRL::ComPtr<IDXGIAdapter3> adapter;
    size_t videoMemoryUsage()
    {
        DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
        adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);
        return videoMemoryInfo.CurrentUsage / 1024 / 1024;
    }

#ifdef ENABLE_DIRECT2D
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d1DeviceContext;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> dwriteTextFormats[8];
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> d2dSolidColorBrushes[8];
    void CreateDirext2dObjects();
#endif // ENABLE_DIRECT2D

    BOOL fullscreenMode{ FALSE };
    BOOL vsync{ TRUE };
    BOOL tearingSupported{ FALSE };

    RECT windowedRect;
    DWORD windowedStyle;
    void FullscreenState(BOOL fullscreen, const HWND hwnd);
    void OnSizeChanged(UINT64 width, UINT height, const HWND hwnd);
    //-----//
};

