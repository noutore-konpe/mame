#include "Graphics.h"
#include "../Other/misc.h"
#include "../framework.h"

void acquireHighPerformanceAdapter(IDXGIFactory6* dxgiFactory6, IDXGIAdapter3** dxgiAdapter3)
{
	HRESULT hr{ S_OK };

	Microsoft::WRL::ComPtr<IDXGIAdapter3> enumeratedAdapter;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory6->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(enumeratedAdapter.ReleaseAndGetAddressOf())); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		hr = enumeratedAdapter->GetDesc1(&adapterDesc);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		if (adapterDesc.VendorId == 0x1002/*AMD*/ || adapterDesc.VendorId == 0x10DE/*NVIDIA*/)
		{
			OutputDebugStringW((std::wstring(adapterDesc.Description) + L" has been selected.\n").c_str());
			OutputDebugStringA(std::string("\tVendorId:" + std::to_string(adapterDesc.VendorId) + '\n').c_str());
			OutputDebugStringA(std::string("\tDeviceId:" + std::to_string(adapterDesc.DeviceId) + '\n').c_str());
			OutputDebugStringA(std::string("\tSubSysId:" + std::to_string(adapterDesc.SubSysId) + '\n').c_str());
			OutputDebugStringA(std::string("\tRevision:" + std::to_string(adapterDesc.Revision) + '\n').c_str());
			OutputDebugStringA(std::string("\tDedicatedVideoMemory:" + std::to_string(adapterDesc.DedicatedVideoMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tDedicatedSystemMemory:" + std::to_string(adapterDesc.DedicatedSystemMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tSharedSystemMemory:" + std::to_string(adapterDesc.SharedSystemMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tAdapterLuid.HighPart:" + std::to_string(adapterDesc.AdapterLuid.HighPart) + '\n').c_str());
			OutputDebugStringA(std::string("\tAdapterLuid.LowPart:" + std::to_string(adapterDesc.AdapterLuid.LowPart) + '\n').c_str());
			OutputDebugStringA(std::string("\tFlags:" + std::to_string(adapterDesc.Flags) + '\n').c_str());
			break;
		}
	}
	*dxgiAdapter3 = enumeratedAdapter.Detach();
}

Graphics* Graphics::instance = nullptr;

Graphics::Graphics(HWND hWnd, BOOL fullscreen)
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	HRESULT hr = S_OK;

	// �t���X�N���s�������̂��߂̓����f�[�^
	{
		// �X���b�v�`�F�[���̍쐬
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

		D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
		swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = !FULLSCREEN;

		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
			&featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc,
			&swapchain0, &device, NULL, &deviceContext);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		// �����_�[�^�[�Q�b�g�r���[�̍쐬
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer{};
		hr = swapchain0->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


		// �[�x�X�e���V���r���[�̍쐬
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer{};
		D3D11_TEXTURE2D_DESC texture2dDesc{};
		texture2dDesc.Width = SCREEN_WIDTH;
		texture2dDesc.Height = SCREEN_HEIGHT;
		texture2dDesc.MipLevels = 1;
		texture2dDesc.ArraySize = 1;
		texture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texture2dDesc.SampleDesc.Count = 1;
		texture2dDesc.SampleDesc.Quality = 0;
		texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texture2dDesc.CPUAccessFlags = 0;
		texture2dDesc.MiscFlags = 0;
		hr = device->CreateTexture2D(&texture2dDesc, NULL, depthStencilBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = texture2dDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT viewport{};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(SCREEN_WIDTH);
		viewport.Height = static_cast<float>(SCREEN_HEIGHT);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		deviceContext->RSSetViewports(1, &viewport);
	}

	// �t���X�N���풆
#if 0

#if FULLSCREEN_CREATE
	// ��ʂ̃T�C�Y���擾����B
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT screenWidth = rc.right - rc.left;
	UINT screenHeight = rc.bottom - rc.top;

	this->screenWidth = static_cast<float>(screenWidth);
	this->screenHeight = static_cast<float>(screenHeight);

	HRESULT hr = S_OK;


	// �f�o�C�X���X���b�v�`�F�[���̐���
	{
		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		// �X���b�v�`�F�[�����쐬���邽�߂̐ݒ�I�v�V����
		DXGI_SWAP_CHAIN_DESC swapchainDesc;
		{
			swapchainDesc.BufferDesc.Width = screenWidth;
			swapchainDesc.BufferDesc.Height = screenHeight;
			swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 1�s�N�Z��������̊e�F(RGBA)��8bit(0�`255)�̃e�N�X�`��(�o�b�N�o�b�t�@)���쐬����B
			swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

#if 0 // fps����
			swapchainDesc.SampleDesc.Count = 1;
			swapchainDesc.SampleDesc.Quality = 0;
			swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchainDesc.BufferCount = 2;		// �o�b�N�o�b�t�@�̐�
			swapchainDesc.OutputWindow = hWnd;	// DirectX�ŕ`�������\������E�C���h�E
			swapchainDesc.Windowed = TRUE;		// �E�C���h�E���[�h���A�t���X�N���[���ɂ��邩�B
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapchainDesc.Flags = 0; // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
#else 
			swapchainDesc.SampleDesc.Count = 1;
			swapchainDesc.SampleDesc.Quality = 0;
			swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchainDesc.BufferCount = 1;		// �o�b�N�o�b�t�@�̐�
			swapchainDesc.OutputWindow = hWnd;	// DirectX�ŕ`�������\������E�C���h�E
			swapchainDesc.Windowed = !FULLSCREEN;		// �E�C���h�E���[�h���A�t���X�N���[���ɂ��邩�B
			//swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapchainDesc.Flags = 0; // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
#endif
		}

		D3D_FEATURE_LEVEL featureLevel;

		// �f�o�C�X���X���b�v�`�F�[���̐���
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,						// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr�ŁAIDXGIAdapter�̃A�h���X��n���B
			D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���BD3D_DRIVER_TYPE_HARDWARE �ȊO�͊�{�I�Ƀ\�t�g�E�F�A�����ŁA���ʂȂ��Ƃ�����ꍇ�ɗp����B
			nullptr,						// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵���ۂɁA���̏������s��DLL�̃n���h����n���B����ȊO���w�肵�Ă���ۂɂ͕K��nullptr��n���B
			createDeviceFlags,				// ���炩�̃t���O���w�肷��B�ڂ�����D3D11_CREATE_DEVICE�񋓌^�Ō����B
			featureLevels,					// D3D_FEATURE_LEVEL�񋓌^�̔z���^����Bnullptr�ɂ��邱�Ƃł���Lfeature�Ɠ����̓��e�̔z�񂪎g�p�����B
			ARRAYSIZE(featureLevels),		// featureLevels�z��̗v�f����n���B
			D3D11_SDK_VERSION,				// SDK�̃o�[�W�����B�K�����̒l�B
			&swapchainDesc,					// �����Őݒ肵���\���̂ɐݒ肳��Ă���p�����[�^��SwapChain���쐬�����B
			swapchain0.GetAddressOf(),		// �쐬�����������ꍇ�ɁASwapChain�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��SwapChain�𑀍삷��B
			device.GetAddressOf(),			// �쐬�����������ꍇ�ɁADevice�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��Device�𑀍삷��B
			&featureLevel,					// �쐬�ɐ�������D3D_FEATURE_LEVEL���i�[���邽�߂�D3D_FEATURE_LEVEL�񋓌^�ϐ��̃A�h���X��ݒ肷��B
			deviceContext.GetAddressOf()	// �쐬�����������ꍇ�ɁAContext�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��Context�𑀍삷��B
		);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// �����_�[�^�[�Q�b�g�r���[�̐���
	{
		// �X���b�v�`�F�[������o�b�N�o�b�t�@�e�N�X�`�����擾����B
		// ���X���b�v�`�F�[���ɓ����Ă���o�b�N�o�b�t�@�e�N�X�`����'�F'���������ރe�N�X�`���B
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = swapchain0->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		// �o�b�N�o�b�t�@�e�N�X�`���ւ̏������݂̑����ƂȂ郌���_�[�^�[�Q�b�g�r���[�𐶐�����B
		hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// �[�x�X�e���V���r���[�̐���
	{
		// �[�x�X�e���V�������������ނ��߂̃e�N�X�`�����쐬����B
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		depthStencilBufferDesc.Width = screenWidth;
		depthStencilBufferDesc.Height = screenHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 1�s�N�Z��������A�[�x����24Bit / �X�e���V������8bit�̃e�N�X�`�����쐬����B
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// �[�x�X�e���V���p�̃e�N�X�`�����쐬����B
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;
		hr = device->CreateTexture2D(&depthStencilBufferDesc, nullptr, depthStencilBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		// �[�x�X�e���V���e�N�X�`���ւ̏������݂ɑ����ɂȂ�[�x�X�e���V���r���[���쐬����B
		hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// �r���[�|�[�g�̐ݒ�
	{
		// ��ʂ̂ǂ̗̈��DirectX�ŕ`�������\�����邩�̐ݒ�B
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screenWidth);
		viewport.Height = static_cast<float>(screenHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		deviceContext->RSSetViewports(1, &viewport);
	}
#else

	if (fullscreen)
	{
		FullscreenState(TRUE, hWnd);
	}

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	framebufferDimensions.cx = clientRect.right - clientRect.left;
	framebufferDimensions.cy = clientRect.bottom - clientRect.top;

	HRESULT hr{ S_OK };

	UINT createFactoryFlags{};
#ifdef _DEBUG
	createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory6;
	// todo:�t���X�N���������ǂ����G���[�o�܂�
	hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(dxgiFactory6.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	acquireHighPerformanceAdapter(dxgiFactory6.Get(), adapter.GetAddressOf());

	UINT createDeviceFlags{ 0 };
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
#ifdef ENABLE_DIRECT2D
	createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif // ENABLE_DIRECT2D

	D3D_FEATURE_LEVEL featureLevels{ D3D_FEATURE_LEVEL_11_1 };
	hr = D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, 0, createDeviceFlags, &featureLevels, 1, D3D11_SDK_VERSION, &device, NULL, &deviceContext);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	CreateSwapChain(dxgiFactory6.Get(), hWnd);
#ifdef ENABLE_DIRECT2D
	CreateDirext2dObjects();
#endif // ENABLE_DIRECT2D

#endif

#endif

	// �V�F�[�_�[
	{
		shader = std::make_unique<Shader>(device.Get());
	}

}

Graphics::~Graphics()
{
}

#ifdef ENABLE_DIRECT2D
void Graphics::CreateDirext2dObjects()
{
	HRESULT hr{ S_OK };

	Microsoft::WRL::ComPtr<IDXGIDevice2> dxgiDevice2;
	hr = device.As(&dxgiDevice2);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	Microsoft::WRL::ComPtr<ID2D1Factory1> d2dFactory1;
	D2D1_FACTORY_OPTIONS factoryOptions{};
#ifdef _DEBUG
	factoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif // _DEBUG
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryOptions, d2dFactory1.GetAddressOf());

	Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice;
	hr = d2dFactory1->CreateDevice(dxgiDevice2.Get(), d2dDevice.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2d1DeviceContext.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = dxgiDevice2->SetMaximumFrameLatency(1);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	Microsoft::WRL::ComPtr<IDXGISurface2> dxgiSurface2;
	hr = swapchain->GetBuffer(0, IID_PPV_ARGS(dxgiSurface2.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dBitmap1;
	hr = d2d1DeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface2.Get(),
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), d2dBitmap1.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	d2d1DeviceContext->SetTarget(d2dBitmap1.Get());

	Microsoft::WRL::ComPtr<IDWriteFactory> dwriteFactory;
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(dwriteFactory.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr=dwriteFactory->CreateTextFormat(L"Meiryo", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 11, L"", dwriteTextFormats[0].ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	
	hr = dwriteTextFormats[0]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = dwriteFactory->CreateTextFormat(L"Impact", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 24, L"", dwriteTextFormats[1].ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	hr = dwriteTextFormats[1]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = d2d1DeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), d2dSolidColorBrushes[0].ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	hr = d2d1DeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), d2dSolidColorBrushes[1].ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}
#endif // ENABLE_DIRECT2D

void Graphics::CreateSwapChain(IDXGIFactory6* dxgiFactory6, const HWND hwnd)
{
	HRESULT hr{ S_OK };

	if (swapchain)
	{
		ID3D11RenderTargetView* nullRenderTargetView{};
		deviceContext->OMSetRenderTargets(1, &nullRenderTargetView, NULL);
		renderTargetView.Reset();
#if 0
		deviceContext->Flush();
		deviceContext->ClearState();
#endif
		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapchain->GetDesc(&swapChainDesc);
		hr = swapchain->ResizeBuffers(swapChainDesc.BufferCount, framebufferDimensions.cx, framebufferDimensions.cy, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTargetBuffer;
		hr = swapchain->GetBuffer(0, IID_PPV_ARGS(renderTargetBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		D3D11_TEXTURE2D_DESC texture2dDesc;
		renderTargetBuffer->GetDesc(&texture2dDesc);

		hr = device->CreateRenderTargetView(renderTargetBuffer.Get(), NULL, renderTargetView.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	else
	{
		BOOL allowTearing = FALSE;
		if (SUCCEEDED(hr))
		{
			hr = dxgiFactory6->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
		}
		tearingSupported = SUCCEEDED(hr) && allowTearing;

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc1{};
		swapChainDesc1.Width = framebufferDimensions.cx;
		swapChainDesc1.Height = framebufferDimensions.cy;
		swapChainDesc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc1.Stereo = FALSE;
		swapChainDesc1.SampleDesc.Count = 1;
		swapChainDesc1.SampleDesc.Quality = 0;
		swapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc1.BufferCount = 2;
		swapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc1.Flags = tearingSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
		hr = dxgiFactory6->CreateSwapChainForHwnd(device.Get(), hwnd, &swapChainDesc1, NULL, NULL, swapchain.ReleaseAndGetAddressOf());
#if 0
		swapChainDesc1.Flags |= DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
#endif
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = dxgiFactory6->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTargetBuffer;
		hr = swapchain->GetBuffer(0, IID_PPV_ARGS(renderTargetBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		hr = device->CreateRenderTargetView(renderTargetBuffer.Get(), NULL, renderTargetView.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer{};
	D3D11_TEXTURE2D_DESC texture2dDesc{};
	texture2dDesc.Width = framebufferDimensions.cx;
	texture2dDesc.Height = framebufferDimensions.cy;
	texture2dDesc.MipLevels = 1;
	texture2dDesc.ArraySize = 1;
	texture2dDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texture2dDesc.SampleDesc.Count = 1;
	texture2dDesc.SampleDesc.Quality = 0;
	texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2dDesc.CPUAccessFlags = 0;
	texture2dDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&texture2dDesc, NULL, depthStencilBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(framebufferDimensions.cx);
	viewport.Height = static_cast<float>(framebufferDimensions.cy);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &viewport);
}

void Graphics::FullscreenState(BOOL fullscreen, const HWND hwnd)
{
	fullscreenMode = fullscreen;
	if (fullscreen)
	{
		GetWindowRect(hwnd, &windowedRect);
		SetWindowLongPtrA(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

		RECT fullscreenWindowRect;

		HRESULT hr{ E_FAIL };
		if (swapchain)
		{
			Microsoft::WRL::ComPtr<IDXGIOutput> dxgiOutPut;
			hr = swapchain->GetContainingOutput(&dxgiOutPut);
			if (hr == S_OK)
			{
				DXGI_OUTPUT_DESC outPutDesc;
				hr = dxgiOutPut->GetDesc(&outPutDesc);
				if (hr == S_OK)
				{
					fullscreenWindowRect = outPutDesc.DesktopCoordinates;
				}
			}
		}
		if (hr != S_OK)
		{
			DEVMODE devmode = {};
			devmode.dmSize = sizeof(DEVMODE);
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);

			fullscreenWindowRect =
			{
				devmode.dmPosition.x,
				devmode.dmPosition.y,
				devmode.dmPosition.x + static_cast<LONG>(devmode.dmPelsWidth),
				devmode.dmPosition.y + static_cast<LONG>(devmode.dmPelsHeight)
			};
		}
		SetWindowPos(
			hwnd,
#ifdef _DEBUG
			NULL,
#else
			HWND_TOPMOST,
#endif
			fullscreenWindowRect.left,
			fullscreenWindowRect.top,
			fullscreenWindowRect.right,
			fullscreenWindowRect.bottom,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(hwnd, SW_MAXIMIZE);
	}
	else
	{
		SetWindowLongPtrA(hwnd, GWL_STYLE, windowedStyle);
		SetWindowPos(
			hwnd,
			HWND_NOTOPMOST,
			windowedRect.left,
			windowedRect.top,
			windowedRect.right - windowedRect.left,
			windowedRect.bottom - windowedRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(hwnd, SW_NORMAL);
	}
}

void Graphics::OnSizeChanged(UINT64 width, UINT height, const HWND hwnd)
{
	HRESULT hr{ S_OK };
	if (width != framebufferDimensions.cx || height != framebufferDimensions.cy)
	{
		framebufferDimensions.cx = static_cast<LONG>(width);
		framebufferDimensions.cy = height;

		// �����ŃV�F�[�_ ���\�[�X �r���[��ێ����Ă��邷�ׂẴI�u�W�F�N�g��������܂��B
#ifdef ENABLE_DIRECT2D
		d2d1DeviceContext.Reset();
#endif // ENABLE_DIRECT2D

		Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory6;
		hr = swapchain->GetParent(IID_PPV_ARGS(dxgiFactory6.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		CreateSwapChain(dxgiFactory6.Get(), hwnd);

		// �����ŃV�F�[�_ ���\�[�X �r���[��ێ����邷�ׂẴI�u�W�F�N�g���č쐬���܂��B
#ifdef ENABLE_DIRECT2D
		CreateDirext2dObjects();
#endif // ENABLE_DIRECT2D
	}
}
