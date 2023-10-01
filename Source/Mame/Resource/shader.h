#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <windows.h>
#include <memory>
#include <wrl.h>

#include "../Graphics/Camera.h"

#include "../Graphics/RenderContext.h"
#include "../Graphics/Model.h"

// SHADOW
#include "../Graphics/ShadowMap.h"

#define POINT_LIGHT_ONE 0

#if !POINT_LIGHT_ONE
const int pointLightMax = 8;
#endif

class Shader
{
public: // enum関連
    enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC, LINEAR_BORDER_BLACK, LINEAR_BORDER_WHITE, COMPARISON_LINEAR_BORDER_WHITE/*SHADOW*/ };
    enum class DEPTH_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF };
    enum class BLEND_STATE { NONE, ALPHA, ADD, MULTIPLY };
    enum class RASTER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE };

    enum class CONSTANT_BUFFER 
    {
        SCENE_CONSTANT,
        CONSTANT_BUFFER_PARAMETRIC,
        LIGHT_CONSTANT,
        FOG_CONSTANT,
        SHADOW_CONSTANT,
        POST_EFFECT_CONSTANT,
    };

public:
    struct View
    {
        float viewWitdh = 100;
        float viewHeight = 100;
        float nearZ = 0;
        float farZ = 100;

        DirectX::XMFLOAT4 position{ 0, -4, -5, 0 };
        DirectX::XMFLOAT4 camera{ 0, 0, 1, 0 };
    }view;

    struct SceneConstants
    {
        DirectX::XMFLOAT4X4 viewProjection;
        DirectX::XMFLOAT4 lightDirection;
        DirectX::XMFLOAT4 cameraPosition;

        // FOG
        DirectX::XMFLOAT4X4 inverseProjection;
        DirectX::XMFLOAT4X4 inverseViewProjection;
        float time;

        float pads[3];
    };

    // SHADOW
    struct ShadowConstants
    {
        DirectX::XMFLOAT4X4 viewProjection;
        DirectX::XMFLOAT4 lightDirection;
        DirectX::XMFLOAT4 cameraPosition;
        // SHADOW
        DirectX::XMFLOAT4X4 lightViewProjection;
    };

    // FOG
    struct FogConstants
    {
        DirectX::XMFLOAT4 fogColor = { 1.000f, 1.000f, 1.000f, 0.894f }; // w: fog intensuty
        float fogDensity = 0.0005f;
        //float fogHeightFalloff = 0.9313f;
        float fogHeightFalloff = 10.0f;
        float startDistance = 5.00f;
        float fogCutoffDistance = 500.0f;
        float timeScale = 0.5f;
        float speedScale = 0.2f;
        float pads[2];
    }fogConstants;

    // DIRECTION_LIGHT
    struct DirectionLight
    {
        DirectX::XMFLOAT4 direction;    // ライトの方向
        DirectX::XMFLOAT4 color;        // ライトの位置
    };

    // POINT_LIGHT
    struct PointLight
    {
        DirectX::XMFLOAT4 position = {};    // ポイントライトの位置
        DirectX::XMFLOAT4 color = {};       // ポイントライトの色
        float range = 0;                    // ポイントライトの影響範囲
        DirectX::XMFLOAT3 step = {};        // ダミー
    };
    std::unique_ptr<Model>pointLightModel = nullptr;

    // SPOT_LIGHT
    struct SpotLight
    {
        DirectX::XMFLOAT4 position = {};    // スポットライトの位置
        DirectX::XMFLOAT4 color = {};       // スポットライトの色
        float range = 0;                    // スポットライトの影響範囲
        DirectX::XMFLOAT3 step = {};        // ダミー
        DirectX::XMFLOAT3 direction = {};   // スポットライトの射出方向
        float angle = 0;                    // スポットライトの射出角度
    };

    // HEMISPHERE_LIGHT
    struct HemisphereLight
    {
        DirectX::XMFLOAT4 groundColor;  // 地面色
        DirectX::XMFLOAT4 skyColor;     // 天球色
        DirectX::XMFLOAT4 groundNormal; // 地面の法線
    };

    struct LightConstants
    {
        DirectionLight directionLight;      // ディレクションライト
        
#if POINT_LIGHT_ONE
        PointLight pointLight;              // ポイントライト
#else
        PointLight pointLight[pointLightMax];
#endif// POINT_LIGHT_ONE

        SpotLight spotLight;                // スポットライト
        HemisphereLight hemisphereLight;    // 半球ライト
    }lightConstant;

    struct PostEffectConstants
    {
        DirectX::XMFLOAT4 shiftSize = {};

        DirectX::XMFLOAT4 noiseColor{ 1.0f,1.0f,1.0f,1.0f };
        float noiseTimer = 0.0f;
        float scanLineTimer = 0.0f;
        DirectX::XMFLOAT2 dummy{};
    }postEffectConstants;

public:
    Shader(ID3D11Device* device);
    ~Shader() {}

    void Initialize();

    // 描画開始
    void Begin(ID3D11DeviceContext* dc, const RenderContext& rc);
    void Begin(ID3D11DeviceContext* dc, const RenderContext& rc, const ShadowConstants& shadowConstant);
    void SetState(ID3D11DeviceContext* dc,
        int RastarizeState, int DepthStencilState, int SamplerState);

    void SetState(ID3D11DeviceContext* deviceContext,
        int SamplerState, int DepthStencileState, int BlendState, int RasterizerState);

    // 描画
    void Draw(ID3D11DeviceContext* dc, Model* model);
    
    // 描画終了
    void End(ID3D11DeviceContext* dc);

    void DrawDebug();

public:// 各種ステート設定
    void SetDepthStencileState(int depthStencileState);
    void SetBlendState(int blendState);
    void SetRasterizerState(int rasterizerState);


public:
    void EntryLight(); // 登場演出に使う
    void EntryLight2();
    bool isEntryLight = false;

public://todo:どうにかしたい
    void GSSetConstantBuffer();

private:
    DirectX::XMFLOAT3 eye{ 0.0f,0.0f,-10.0f };
    DirectX::XMFLOAT3 focus{ 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 up{ 0.0f,1.0f,0.0f };

    static const int MaxBones = 128;

    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer[10];



    struct CBMesh
    {
        DirectX::XMFLOAT4X4 boneTransformas[MaxBones];
    };

    struct CBSubset
    {
        DirectX::XMFLOAT4 materialColor;
    };

    struct CBParametric
    {
        float extraction_threshold{ 0.8f };
        float gaussian_sigma{ 1.0f };
        float bloom_intensity{ 1.0f };
        float exposure{ 1.0f };
    };



    Microsoft::WRL::ComPtr<ID3D11Buffer> meshConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> subsetConstantBuffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader>  vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   inputLayout;

    Microsoft::WRL::ComPtr<ID3D11BlendState>        blendStates[4];
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   rasterizerStates[4];
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[4];

    Microsoft::WRL::ComPtr<ID3D11SamplerState>  samplerState[6];
};


HRESULT CreateVsFromCso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);
HRESULT CreatePsFromCso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);

HRESULT CreateGsFromCso(ID3D11Device* device, const char* csoName, ID3D11GeometryShader** geometryShader);
HRESULT CreateCsFromCso(ID3D11Device* device, const char* csoName, ID3D11ComputeShader** computeShader);

HRESULT CreateDsFromCso(ID3D11Device* device, const char* csoName, ID3D11DomainShader** domainShader);
HRESULT CreateHsFromCso(ID3D11Device* device, const char* csoName, ID3D11HullShader** hullShader);