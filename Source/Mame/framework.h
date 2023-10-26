#pragma once

#include <d3d11.h>

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <wrl.h>
#include <dxgi1_6.h>

#include "Other/misc.h"

#include "high_resolution_timer.h"

#include "Graphics/Graphics.h"
#include "Input/Input.h"

#include "Resource/shader.h"
#include "Resource/sprite.h"
#include "Resource/sprite_batch.h"
#include "Resource/GeometricPrimitive.h"
#include "Resource/static_mesh.h"
#include "Resource/skinned_mesh.h"
#include "FrameBuffer.h"
#include "FullscreenQuad.h"

#include "Resource/Audio.h"
#include "Resource/GltfModel.h"

#include "../../External/imgui/ImGuiCtrl.h"

#if 1	// todo : フルスクリーン実装完了したらいらなくなる
CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };
CONST BOOL FULLSCREEN{ TRUE };
CONST LPCWSTR APPLICATION_NAME{ L"Emma" };
#endif

class framework
{
private:
	Graphics graphics;
	Input input;

public:
	CONST HWND hwnd;

	framework(HWND hwnd);
	~framework();

	framework(const framework&) = delete;
	framework& operator=(const framework&) = delete;
	framework(framework&&) noexcept = delete;
	framework& operator=(framework&&) noexcept = delete;


	int run()
	{
		MSG msg{};

		if (!initialize())
		{
			return 0;
		}

		// ImGui初期化(DirectX11の初期化の下に置くこと)
		IMGUI_CTRL_INITIALIZE(hwnd, graphics.GetDevice(), graphics.GetDeviceContext());

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				tictoc.tick();
				CalculateFrameStats();
				update(tictoc.time_interval());
				render(tictoc.time_interval());
			}
		}

		// ImGui終了化
		IMGUI_CTRL_UNINITIALIZE();

#if 1
		BOOL fullscreen = 0;
		graphics.GetSwapChain()->GetFullscreenState(&fullscreen, 0);
		if (fullscreen)
		{
			graphics.GetSwapChain()->SetFullscreenState(FALSE, 0);
		}
#endif

		return uninitialize() ? static_cast<int>(msg.wParam) : 0;
	}

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		// ImGui(先頭に置く)
		IMGUI_CTRL_WND_PRC_HANDLER(hwnd, msg, wparam, lparam);

		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps{};
			BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE)
			{
				// ESCで落とす
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
		case WM_ENTERSIZEMOVE:
			tictoc.stop();
			break;
		case WM_EXITSIZEMOVE:
			tictoc.start();
			break;
		case WM_SIZE:
		{
#if 1
			RECT clientRect{};
			GetClientRect(hwnd, &clientRect);
			Graphics::Instance().OnSizeChanged(static_cast<UINT64>(clientRect.right - clientRect.left), clientRect.bottom - clientRect.top, hwnd);
#endif
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

public:
	//static high_resolution_timer& GetTictoc() { return tictoc; }

private:
	bool initialize();
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);
	bool uninitialize();

public:
	static high_resolution_timer tictoc;
	uint32_t frames{ 0 };
	//static float elapsedTime;

	float elapsedTime{0};
	void CalculateFrameStats()
	{
		if (++frames, (tictoc.time_stamp() - elapsedTime) >= 1.0f)
		{
			float fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			//outs << APPLICATION_NAME;
			outs << L"FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			elapsedTime += 1.0f;
		}
	}
};