#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <windows.h>
#include <memory>
#include <wrl.h>

#include "../Graphics/Camera.h"

#include "../Graphics/RenderContext.h"
#include "../Graphics/Model.h"

class Shader
{
public:
    Shader(ID3D11Device* device);
    ~Shader() {}

    void Initialize();

    // ï`âÊäJén
    void Begin(ID3D11DeviceContext* dc, const RenderContext& rc);
    void Begin(ID3D11DeviceContext* dc, const RenderContext& rc, int state);
    void SetState(ID3D11DeviceContext* dc,
        int RastarizeState, int DepthStencilState, int SamplerState);

    // ï`âÊ
    void Draw(ID3D11DeviceContext* dc, Model* model);
    
    // ï`âÊèIóπ
    void End(ID3D11DeviceContext* dc);

    void DrawDebug();

    struct View
    {
        float viewWitdh = 100;
        float viewHeight = 100;
        float nearZ = 0;
        float farZ = 100;

        DirectX::XMFLOAT4 position{ 0,0,-1,0 };
        DirectX::XMFLOAT4 camera{ 0,0,1,0 };
    }view;

private:
    //Camera camera;

    DirectX::XMFLOAT3 eye{ 0.0f,0.0f,-10.0f };
    DirectX::XMFLOAT3 focus{ 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 up{ 0.0f,1.0f,0.0f };


    static const int MaxBones = 128;

    struct SceneConstants
    {
        DirectX::XMFLOAT4X4 viewProjection;
        DirectX::XMFLOAT4 lightDirection;
        DirectX::XMFLOAT4 cameraPosition;
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> sceneConstantBuffer[2];

    struct LightConstants
    {
        DirectX::XMFLOAT4 ambientColor;
        DirectX::XMFLOAT4 directionalLightDirection;
        DirectX::XMFLOAT4 directionalLightColor;
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> lightConstantBuffer;

    DirectX::XMFLOAT4 ambientColor{ 0.2f, 0.2f ,0.2f, 0.2f };
    DirectX::XMFLOAT4 directionalLightDirection{ 0.0f,-1.0f,1.0f,1.0f };
    DirectX::XMFLOAT4 directionalLightColor{ 1.0f, 1.0f, 1.0f, 1.0f };

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

    Microsoft::WRL::ComPtr<ID3D11BlendState>        blendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   rasterizerState[4];
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState[4];

    Microsoft::WRL::ComPtr<ID3D11SamplerState>  samplerState[3];
};


HRESULT create_vs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);

HRESULT create_ps_from_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);