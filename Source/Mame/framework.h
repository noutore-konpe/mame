﻿#pragma once
//#pragma comment (lib,"External/Effekseer162a/lib/vs2019/MT/x64/Debug/Effekseer.lib")

#include <d3d11.h>

#include <windows.h>
#include <tchar.h>
#include <sstream>

#include <wrl.h>

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
#include "framebuffer.h"
#include "fullscreen_quad.h"

#include "Resource/Audio.h"
#include "Resource/GltfModel.h"

#ifdef USE_IMGUI
#include "../../External/imgui/ImGuiCtrl.h"
#endif // USE_IMGUI

CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };
CONST BOOL FULLSCREEN{ FALSE };
CONST LPCWSTR APPLICATION_NAME{ L"MAMEO" };

class framework
{
private:
	Graphics graphics;
	Input input;

public:

	// gltfModel(test)
	std::unique_ptr<GltfModel>gltfModels[8];

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceViews[8];

	// sprite(test)
	std::unique_ptr<Sprite>sprite;

	std::unique_ptr<Model> model;

	std::unique_ptr<framebuffer> framebuffers[8];

	std::unique_ptr<fullscreen_quad> bit_block_transfer;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];

	struct parametric_constants
	{
		float extraction_threshold{ 0.8f };
		float gaussian_sigma{ 1.0f };
		float bloom_intensity{ 1.0f };
		float exposure{ 1.0f };
	};

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
				calculate_frame_stats();
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
				//PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
		case WM_ENTERSIZEMOVE:
			tictoc.stop();
			break;
		case WM_EXITSIZEMOVE:
			tictoc.start();
			break;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:
	bool initialize();
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);
	bool uninitialize();

private:
	high_resolution_timer tictoc;
	uint32_t frames{ 0 };
	float elapsed_time{ 0.0f };
	void calculate_frame_stats()
	{
		if (++frames, (tictoc.time_stamp() - elapsed_time) >= 1.0f)
		{
			float fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			elapsed_time += 1.0f;
		}
	}
};