#pragma once
#define NOMINMAX
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "../../../tinygltf-release/tiny_gltf.h"

#include "../Transform.h"

class GltfModel
{
public:
    GltfModel(ID3D11Device* device, const std::string& filename);
    virtual ~GltfModel() = default;

public:
    struct Scene
    {
        std::string name;
        std::vector<int> nodes; // Array of 'root' nodes
    };
    std::vector<Scene> scenes;

    // ノード
    struct Node
    {
        std::string name;
        int skin{ -1 }; // index of skin referenced by this node
        int mesh{ -1 }; // index of mesh referenced by this node

        std::vector<int> children; // An array of indices of child nodes of this node

        // Local Transforms
        DirectX::XMFLOAT4 rotation{ 0,0,0,1 };
        DirectX::XMFLOAT3 scale{ 1,1,1 };
        DirectX::XMFLOAT3 translation{ 0,0,0 };

        DirectX::XMFLOAT4X4 globalTransform
        {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    };
    std::vector<Node> nodes;

    // メッシュ
    struct BufferView
    {
        DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        size_t strideInBytes{ 0 };
        size_t sizeInBytes{ 0 };
        size_t count()const
        {
            return sizeInBytes / strideInBytes;
        }
    };
    struct Mesh
    {
        std::string name;
        struct Primitive
        {
            int material;
            std::map<std::string, BufferView> vertexBufferViews;
            BufferView indexBufferView;
        };
        std::vector<Primitive> primitives;
    };
    std::vector<Mesh> meshes;

    // マテリアル
    struct TextureInfo
    {
        int index = -1;
        int texcoord = 0;
    };
    struct NormalTextureInfo
    {
        int index = -1;
        int texcoord = 0;
        float scale = 1;
    };
    struct OcclusionTextureInfo
    {
        int index = -1;
        int texcoord = 0;
        float strength = 1;
    };
    struct PbrMetaricRoughness
    {
        float baseColorFactor[4] = { 1, 1, 1, 1 };
        TextureInfo baseColorTexture;
        float metallicFactor = 1;
        float roughnessFactor = 1;
        TextureInfo metallicRoughnessTexture;
    };
    struct Material
    {
        std::string name;

        struct Cbuffer
        {
            float emissiveFactor[3] = { 0, 0, 0 };
            int alphaMode = 0; // "OPAQUE" : 0, "MASK" : 1, "BLEND" : 2
            float alphaCutoff = 0.5f;
            bool doubleSided = false;

            PbrMetaricRoughness pbrMetallicRoughness;

            NormalTextureInfo normalTexture;
            OcclusionTextureInfo occlusionTexture;
            TextureInfo emissiveTexture;
        };
        Cbuffer data;
    };
    std::vector<Material> materials;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> materialResourceView;

    // テクスチャ
    struct Texture
    {
        std::string name;
        int source{ -1 };
    };
    std::vector<Texture> textures;
    struct Image
    {
        std::string name;
        int width{ -1 };
        int height{ -1 };
        int component{ -1 };
        int bits{ -1 };
        int pixelType{ -1 };
        int bufferView;
        std::string mimeType;
        std::string uri;
        bool asIs{ false };
    };
    std::vector<Image> images;
    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureResourceViews;

    // アニメーション
    struct Skin
    {
        std::vector<DirectX::XMFLOAT4X4> inverseBindMatrices; // 逆バインド行列
        std::vector<int> joints;
    };
    std::vector<Skin> skins;
    struct Animation
    {
        std::string name;

        struct Channel
        {
            int sampler{ -1 };
            int targetNode{ -1 };
            std::string targetPath;
        };
        std::vector<Channel> channels;

        struct Sampler
        {
            int input{ -1 };
            int output{ -1 };
            std::string interpolation;  // 補間
        };
        std::vector<Sampler> samplers;

        std::unordered_map<int, std::vector<float>> timelines;
        std::unordered_map<int, std::vector<DirectX::XMFLOAT3>> scales;
        std::unordered_map<int, std::vector<DirectX::XMFLOAT4>> rotations;
        std::unordered_map<int, std::vector<DirectX::XMFLOAT3>> translations;
    };
    std::vector<Animation> animations;

    // ボーン行列
    static const size_t PRIMITIVE_MAX_JOINT = 512;
    struct PrimitiveJointConstants
    {
        DirectX::XMFLOAT4X4 matrices[PRIMITIVE_MAX_JOINT];
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> primitiveJointCbuffer;

public:
    void Update(const float& elapsedTime);
    void Render(float scaleFactor);
    void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMFLOAT4X4& world,
        const std::vector<Node>& animatedNodes);
    void DrawDebug();

    void Animate(size_t animationIndex, float time, std::vector<Node>& animatedNodes, bool loopback = true);


    Transform* GetTransform() { return &transform; }

    // アニメーション関連
    void SetAnimationIndex(int index) { animationIndex = index; }
    int GetAnimationIndex() { return animationIndex; }

private:
    void FetchNodes(const tinygltf::Model& gltfModel);
    void FetchMeshes(ID3D11Device* device, const tinygltf::Model& gltfModel);
    void FetchMatetials(ID3D11Device* device, const tinygltf::Model& gltfModel);
    void FetchTexture(ID3D11Device* device, const tinygltf::Model& gltfModel);
    void FetchAnimation(const tinygltf::Model& gltfModel);

    void CumulateTransforms(std::vector<Node>& nodes);
    GltfModel::BufferView MakeBufferView(const tinygltf::Accessor& accessor);

private:
    std::string filename;

    Transform transform{};

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    struct primitiveConstants
    {
        DirectX::XMFLOAT4X4 world;
        int material{ -1 };
        int hasTangent{ 0 };
        int skin{ -1 };
        int pad;
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> primitiveCbuffer;

    float animationTime = 0;
    int animationIndex = 0;
};