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

class Shader
{
public: // enum�֘A
    enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC, LINEAR_BORDER_BLACK, LINEAR_BORDER_WHITE, COMPARISON_LINEAR_BORDER_WHITE/*SHADOW*/ };
    enum class DEPTH_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF };
    enum class BLEND_STATE { NONE, ALPHA, ADD, MULTIPLY };
    enum class RASTER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE };

    enum class CONSTANT_BUFFER { SCENE_CONSTANT, CONSTANT_BUFFER_PARAMETRIC, LIGHT_CONSTANT };

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
        // SHADOW
        DirectX::XMFLOAT4X4 lightViewProjection;
    };

    // DIRECTION_LIGHT
    struct DirectionLight
    {
        DirectX::XMFLOAT4 direction;    // ���C�g�̕���
        DirectX::XMFLOAT4 color;        // ���C�g�̈ʒu
    };

    // POINT_LIGHT
    struct PointLight
    {
        DirectX::XMFLOAT4 position = {};    // �|�C���g���C�g�̈ʒu
        DirectX::XMFLOAT4 color = {};       // �|�C���g���C�g�̐F
        float range = 0;                    // �|�C���g���C�g�̉e���͈�
        DirectX::XMFLOAT3 step = {};        // �_�~�[
    };
    std::unique_ptr<Model>pointLightModel = nullptr;

    // SPOT_LIGHT
    struct SpotLight
    {
        DirectX::XMFLOAT4 position = {};    // �X�|�b�g���C�g�̈ʒu
        DirectX::XMFLOAT4 color = {};       // �X�|�b�g���C�g�̐F
        float range = 0;                    // �X�|�b�g���C�g�̉e���͈�
        DirectX::XMFLOAT3 step = {};        // �_�~�[
        DirectX::XMFLOAT3 direction = {};   // �X�|�b�g���C�g�̎ˏo����
        float angle = 0;                    // �X�|�b�g���C�g�̎ˏo�p�x
    };

    // HEMISPHERE_LIGHT
    struct HemisphereLight
    {
        DirectX::XMFLOAT4 groundColor;  // �n�ʐF
        DirectX::XMFLOAT4 skyColor;     // �V���F
        DirectX::XMFLOAT4 groundNormal; // �n�ʂ̖@��
    };

    struct LightConstants
    {
        DirectionLight directionLight;      // �f�B���N�V�������C�g
        PointLight pointLight;              // �|�C���g���C�g
        SpotLight spotLight;                // �X�|�b�g���C�g
        HemisphereLight hemisphereLight;    // �������C�g
    }lightConstant;

public:
    Shader(ID3D11Device* device);
    ~Shader() {}

    void Initialize();

    // �`��J�n
    void Begin(ID3D11DeviceContext* dc, const RenderContext& rc);
    void Begin(ID3D11DeviceContext* dc, const RenderContext& rc, const SceneConstants& sceneConstant);
    void SetState(ID3D11DeviceContext* dc,
        int RastarizeState, int DepthStencilState, int SamplerState);

    // �`��
    void Draw(ID3D11DeviceContext* dc, Model* model);
    
    // �`��I��
    void End(ID3D11DeviceContext* dc);

    void DrawDebug();



private:
    DirectX::XMFLOAT3 eye{ 0.0f,0.0f,-10.0f };
    DirectX::XMFLOAT3 focus{ 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 up{ 0.0f,1.0f,0.0f };

    static const int MaxBones = 128;

    Microsoft::WRL::ComPtr<ID3D11Buffer> sceneConstantBuffer[5];



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

    Microsoft::WRL::ComPtr<ID3D11SamplerState>  samplerState[6];
};


HRESULT CreateVsFromCso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);

HRESULT CreatePsFromCso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);