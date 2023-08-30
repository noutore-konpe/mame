#include "GltfModel.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_EXTERANL_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT

#include "../../../tinygltf-release/tiny_gltf.h"

#include <stack>
#include <filesystem>

#include "../Other/misc.h"
#include "../Resource/shader.h"
#include "../Resource/texture.h"
#include "../Graphics/Graphics.h"

// コンストラクタ
GltfModel::GltfModel(ID3D11Device* device, const std::string& filename) : filename(filename)
{
    tinygltf::Model gltfModel;

    tinygltf::TinyGLTF tinyGltf;
    std::string error, warning;
    bool succeeded{ false };
    if (filename.find(".glb") != std::string::npos)
    {
        succeeded = tinyGltf.LoadBinaryFromFile(&gltfModel, &error, &warning, filename.c_str());
    }
    else if (filename.find(".gltf") != std::string::npos)
    {
        succeeded = tinyGltf.LoadASCIIFromFile(&gltfModel, &error, &warning, filename.c_str());
    }

    _ASSERT_EXPR_A(warning.empty(), warning.c_str());
    _ASSERT_EXPR_A(error.empty(), warning.c_str());
    _ASSERT_EXPR_A(succeeded, L"Failed to load glTF file");
    for (std::vector<tinygltf::Scene>::const_reference gltfScene : gltfModel.scenes)
    {
        Scene& scene{ scenes.emplace_back() };
        scene.name = gltfModel.scenes.at(0).name;
        scene.nodes = gltfModel.scenes.at(0).nodes;
    }

    FetchNodes(gltfModel);
    FetchMeshes(device, gltfModel);
    FetchMatetials(device, gltfModel);
    FetchTexture(device, gltfModel);
    FetchAnimation(gltfModel);

    // TODO: This is a force-brute programming, may cause bugs.
    const std::map<std::string, BufferView>& vertexBufferViews{
        meshes.at(0).primitives.at(0).vertexBufferViews };
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
    {
        { "POSITION", 0, vertexBufferViews.at("POSITION").format, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, vertexBufferViews.at("NORMAL").format, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, vertexBufferViews.at("TANGENT").format, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, vertexBufferViews.at("TEXCOORD_0").format, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "JOINTS", 0, vertexBufferViews.at("JOINTS_0").format, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "WEIGHTS", 0, vertexBufferViews.at("WEIGHTS_0").format, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    CreateVsFromCso(device, "./resources/Shader/gltfModelVs.cso", vertexShader.ReleaseAndGetAddressOf(),
        inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, _countof(inputElementDesc));
    CreatePsFromCso(device, "./resources/Shader/gltfModelPs.cso", pixelShader.ReleaseAndGetAddressOf());

    // 定数バッファ生成
    {
        HRESULT hr;
        D3D11_BUFFER_DESC bufferDesc{};

        // primitiveCbuffer
        bufferDesc.ByteWidth = sizeof(primitiveConstants);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        hr = device->CreateBuffer(&bufferDesc, nullptr, primitiveCbuffer.ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        
        // primitiveJointCbuffer(ボーン行列)
        bufferDesc.ByteWidth = sizeof(PrimitiveJointConstants);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        hr = device->CreateBuffer(&bufferDesc, nullptr, primitiveJointCbuffer.ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }
}

// 更新
void GltfModel::Update(const float& elapsedTime)
{
    animationTime += elapsedTime;
    Animate(GetAnimationIndex(), animationTime, nodes);
}

// 外での呼び出し用
void GltfModel::Render(float scaleFactor)
{
    Graphics& graphics = Graphics::Instance();
    DirectX::XMMATRIX W = transform.CalcWorldMatrix(scaleFactor);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, W);
    
    Render(graphics.GetDeviceContext(), world, nodes);
}

// 本体
void GltfModel::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMFLOAT4X4& world, const std::vector<Node>& animatedNodes)
{
    deviceContext->PSSetShaderResources(0, 1, materialResourceView.GetAddressOf());
    deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
    deviceContext->IASetInputLayout(inputLayout.Get());
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    std::function<void(int)> traverse{ [&](int nodeIndex)->void {
        const Node& node{nodes.at(nodeIndex)};
        if (node.mesh > -1)
        {
            const Mesh& mesh{ meshes.at(node.mesh) };
            for (std::vector<Mesh::Primitive>::const_reference primitive : mesh.primitives)
            {
                ID3D11Buffer* vertexBuffers[]
                {
                    primitive.vertexBufferViews.at("POSITION").buffer.Get(),
                    primitive.vertexBufferViews.at("NORMAL").buffer.Get(),
                    primitive.vertexBufferViews.at("TANGENT").buffer.Get(),
                    primitive.vertexBufferViews.at("TEXCOORD_0").buffer.Get(),
                    primitive.vertexBufferViews.at("JOINTS_0").buffer.Get(),
                    primitive.vertexBufferViews.at("WEIGHTS_0").buffer.Get(),
                };
                UINT strides[]
                {
                    static_cast<UINT>(primitive.vertexBufferViews.at("POSITION").strideInBytes),
                    static_cast<UINT>(primitive.vertexBufferViews.at("NORMAL").strideInBytes),
                    static_cast<UINT>(primitive.vertexBufferViews.at("TANGENT").strideInBytes),
                    static_cast<UINT>(primitive.vertexBufferViews.at("TEXCOORD_0").strideInBytes),
                    static_cast<UINT>(primitive.vertexBufferViews.at("JOINTS_0").strideInBytes),
                    static_cast<UINT>(primitive.vertexBufferViews.at("WEIGHTS_0").strideInBytes),
                };
                UINT offsets[_countof(vertexBuffers)]{ 0 };
                deviceContext->IASetVertexBuffers(0, _countof(vertexBuffers), vertexBuffers, strides, offsets);
                deviceContext->IASetIndexBuffer(primitive.indexBufferView.buffer.Get(),
                    primitive.indexBufferView.format, 0);

                primitiveConstants primitiveData{};
                primitiveData.material = primitive.material;
                primitiveData.hasTangent = primitive.vertexBufferViews.at("TANGENT").buffer != NULL;
                primitiveData.skin = node.skin;
                DirectX::XMStoreFloat4x4(&primitiveData.world,
                    DirectX::XMLoadFloat4x4(&node.globalTransform) * DirectX::XMLoadFloat4x4(&world));
                deviceContext->UpdateSubresource(primitiveCbuffer.Get(), 0, 0, &primitiveData, 0, 0);
                deviceContext->VSSetConstantBuffers(0, 1, primitiveCbuffer.GetAddressOf());
                deviceContext->PSSetConstantBuffers(0, 1, primitiveCbuffer.GetAddressOf());

                // texture
                {
                    const Material& material{ materials.at(primitive.material) };
                    const int textureIndices[]
                    {
                        material.data.pbrMetallicRoughness.baseColorTexture.index,
                        material.data.pbrMetallicRoughness.metallicRoughnessTexture.index,
                        material.data.normalTexture.index,
                        material.data.emissiveTexture.index,
                        material.data.occlusionTexture.index,
                    };
                    ID3D11ShaderResourceView* nullShaderResourceView{};
                    std::vector<ID3D11ShaderResourceView*> shaderResourceViews(_countof(textureIndices));
                    for (int textureIndex = 0; textureIndex < shaderResourceViews.size(); ++textureIndex)
                    {
                        shaderResourceViews.at(textureIndex) = textureIndices[textureIndex] > -1 ?
                            textureResourceViews.at(textures.at(textureIndices[textureIndex]).source).Get() :
                            nullShaderResourceView;
                    }
                    deviceContext->PSSetShaderResources(1, static_cast<UINT>(shaderResourceViews.size()),
                        shaderResourceViews.data());
                }

                // animation
                {
                    if (node.skin > -1)
                    {
                        const Skin& skin{ skins.at(node.skin) };
                        PrimitiveJointConstants primitiveJointData{};
                        for (size_t jointIndex = 0; jointIndex < skin.joints.size(); ++jointIndex)
                        {
                            DirectX::XMStoreFloat4x4(&primitiveJointData.matrices[jointIndex],
                                DirectX::XMLoadFloat4x4(&skin.inverseBindMatrices.at(jointIndex)) *
                                DirectX::XMLoadFloat4x4(&animatedNodes.at(skin.joints.at(jointIndex)).globalTransform) *
                                DirectX::XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&node.globalTransform))
                            );
                        }
                        deviceContext->UpdateSubresource(primitiveJointCbuffer.Get(), 0, 0, &primitiveJointData, 0, 0);
                        deviceContext->VSSetConstantBuffers(2, 1, primitiveJointCbuffer.GetAddressOf());
                    }
                }

                deviceContext->DrawIndexed(static_cast<UINT>(primitive.indexBufferView.count()), 0, 0);
            }
        }
        for (std::vector<int>::value_type childIndex : node.children)
        {
            traverse(childIndex);
        }
    } };
    for (std::vector<int>::value_type nodeIndex : scenes.at(0).nodes)
    {
        traverse(nodeIndex);
    }
}

void GltfModel::DrawDebug()
{
#ifdef USE_IMGUI
    GetTransform()->DrawDebug();
#endif // USE_IMGUI
}

void GltfModel::Animate(size_t animationIndex, float time, std::vector<Node>& animatedNodes, bool loopback)
{
    std::function<size_t(const std::vector<float>&, float, float&, bool)> indexof
    {
        [](const std::vector<float>& timelines, float time, float& interpolationFactor, bool loopback)->size_t
        {
            const size_t keyframeCount{timelines.size()};
            if (time > timelines.at(keyframeCount - 1))
            {
                if (loopback)
                {
                    //if(time>=1.0f)
                    //if (timelines.at(keyframeCount - 1) != 0)
                        time = fmodf(time, timelines.at(keyframeCount - 1));
                }
                else
                {
                    interpolationFactor = 1.0f;
                    return keyframeCount - 2;
                }
            }
            else if (time < timelines.at(0))
            {
                interpolationFactor = 0.0f;
                return 0;
            }
            size_t keyframeIndex{ 0 };
            for (size_t timeIndex = 1; timeIndex < keyframeCount; ++timeIndex)
            {
                if (time < timelines.at(timeIndex))
                {
                    keyframeIndex = std::max<size_t>(0LL, timeIndex - 1);
                    break;
                }
            }
            interpolationFactor = (time - timelines.at(keyframeIndex + 0)) /
                (timelines.at(keyframeIndex + 1) - timelines.at(keyframeIndex + 0));
            return keyframeIndex;
        }
    };

    if (animations.size() > 0)
    {
        const Animation& animation{ animations.at(animationIndex) };
        for (std::vector<Animation::Channel>::const_reference channel : animation.channels)
        {
            const Animation::Sampler& sampler{ animation.samplers.at(channel.sampler) };
            const std::vector<float>& timeline{ animation.timelines.at(sampler.input) };
            if (timeline.size() == 0)
            {
                continue;
            }
            float interpolationFactor{};
            size_t keyframeIndex{ indexof(timeline, time, interpolationFactor, loopback) };
            if (channel.targetPath == "scale")
            {
                const std::vector<DirectX::XMFLOAT3>& scales{ animation.scales.at(sampler.output) };
                DirectX::XMStoreFloat3(&animatedNodes.at(channel.targetNode).scale,
                    DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&scales.at(keyframeIndex + 0)),
                        DirectX::XMLoadFloat3(&scales.at(keyframeIndex + 1)), interpolationFactor));
            }
            else if (channel.targetPath == "rotation")
            {
                const std::vector<DirectX::XMFLOAT4>& rotations{ animation.rotations.at(sampler.output) };
                DirectX::XMStoreFloat4(&animatedNodes.at(channel.targetNode).rotation,
                    DirectX::XMQuaternionNormalize(DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&rotations.at(keyframeIndex + 0)),
                        DirectX::XMLoadFloat4(&rotations.at(keyframeIndex + 1)), interpolationFactor)));
            }
            else if (channel.targetPath == "translation")
            {
                const std::vector<DirectX::XMFLOAT3>& translations{ animation.translations.at(sampler.output) };
                DirectX::XMStoreFloat3(&animatedNodes.at(channel.targetNode).translation,
                    DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&translations.at(keyframeIndex + 0)),
                        DirectX::XMLoadFloat3(&translations.at(keyframeIndex + 1)), interpolationFactor));
            }
        }
        CumulateTransforms(animatedNodes);
    }
    else
    {
        animatedNodes = nodes;
    }
}

// ノードデータ取得
void GltfModel::FetchNodes(const tinygltf::Model& gltfModel)
{
    for (std::vector<tinygltf::Node>::const_reference gltfNode : gltfModel.nodes)
    {
        Node& node{ nodes.emplace_back() };
        node.name = gltfNode.name;
        node.skin = gltfNode.skin;
        node.mesh = gltfNode.mesh;
        node.children = gltfNode.children;
        if (!gltfNode.matrix.empty())
        {
            DirectX::XMFLOAT4X4 matrix;
            for (size_t row = 0; row < 4; ++row)
            {
                for (size_t column = 0; column < 4; ++column)
                {
                    matrix(row, column) = static_cast<float>(gltfNode.matrix.at(4 * row + column));
                }
            }

            DirectX::XMVECTOR S, T, R;
            bool succeed = DirectX::XMMatrixDecompose(&S, &R, &T, DirectX::XMLoadFloat4x4(&matrix));
            _ASSERT_EXPR(succeed, L"Failed to decompose matrix");

            DirectX::XMStoreFloat3(&node.scale, S);
            DirectX::XMStoreFloat4(&node.rotation, R);
            DirectX::XMStoreFloat3(&node.translation, T);
        }
        else
        {
            if (gltfNode.scale.size() > 0)
            {
                node.scale.x = static_cast<float>(gltfNode.scale.at(0));
                node.scale.y = static_cast<float>(gltfNode.scale.at(1));
                node.scale.z = static_cast<float>(gltfNode.scale.at(2));
            }
            if (gltfNode.translation.size() > 0)
            {
                node.translation.x = static_cast<float>(gltfNode.translation.at(0));
                node.translation.y = static_cast<float>(gltfNode.translation.at(1));
                node.translation.z = static_cast<float>(gltfNode.translation.at(2));
            }
            if (gltfNode.rotation.size() > 0)
            {
                node.rotation.x = static_cast<float>(gltfNode.rotation.at(0));
                node.rotation.y = static_cast<float>(gltfNode.rotation.at(1));
                node.rotation.z = static_cast<float>(gltfNode.rotation.at(2));
                node.rotation.w = static_cast<float>(gltfNode.rotation.at(3));
            }
        }
    }
    CumulateTransforms(nodes);
}

// メッシュデータ取得
void GltfModel::FetchMeshes(ID3D11Device* device, const tinygltf::Model& gltfModel)
{
    HRESULT hr;
    for (std::vector<tinygltf::Mesh>::const_reference gltfMesh : gltfModel.meshes)
    {
        Mesh& mesh{ meshes.emplace_back() };
        mesh.name = gltfMesh.name;
        for (std::vector<tinygltf::Primitive>::const_reference gltfPrimitive : gltfMesh.primitives)
        {
            Mesh::Primitive& primitive{ mesh.primitives.emplace_back() };
            primitive.material = gltfPrimitive.material;

            // Create index buffer
            const tinygltf::Accessor& gltfAccessor{ gltfModel.accessors.at(gltfPrimitive.indices) };
            const tinygltf::BufferView& gltfBufferView{ gltfModel.bufferViews.at(gltfAccessor.bufferView) };

            primitive.indexBufferView = MakeBufferView(gltfAccessor);
            D3D11_BUFFER_DESC bufferDesc{};
            bufferDesc.ByteWidth = static_cast<UINT>(primitive.indexBufferView.sizeInBytes);
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            D3D11_SUBRESOURCE_DATA subresourceData{};
            subresourceData.pSysMem = gltfModel.buffers.at(gltfBufferView.buffer).data.data()
                + gltfBufferView.byteOffset + gltfAccessor.byteOffset;
            hr = device->CreateBuffer(&bufferDesc, &subresourceData,
                primitive.indexBufferView.buffer.ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

            // Create vertex buffers
            for (std::map<std::string, int>::const_reference gltfAttribute : gltfPrimitive.attributes)
            {
                const tinygltf::Accessor& gltfAccessor{ gltfModel.accessors.at(gltfAttribute.second) };
                const tinygltf::BufferView& gltfBufferView{ gltfModel.bufferViews.at(gltfAccessor.bufferView) };

                BufferView vertexBufferView{ MakeBufferView(gltfAccessor) };

                D3D11_BUFFER_DESC bufferDesc{};
                bufferDesc.ByteWidth = static_cast<UINT>(vertexBufferView.sizeInBytes);
                bufferDesc.Usage = D3D11_USAGE_DEFAULT;
                bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                D3D11_SUBRESOURCE_DATA subresourceData{};
                subresourceData.pSysMem = gltfModel.buffers.at(gltfBufferView.buffer).data.data()
                    + gltfBufferView.byteOffset + gltfAccessor.byteOffset;
                hr = device->CreateBuffer(&bufferDesc, &subresourceData,
                    vertexBufferView.buffer.ReleaseAndGetAddressOf());
                _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

                primitive.vertexBufferViews.emplace(std::make_pair(gltfAttribute.first, vertexBufferView));
            }

            // Add dummy attributes if any are amissing.
            const std::unordered_map<std::string, BufferView> attributes{
                { "TANGENT", { DXGI_FORMAT_R32G32B32A32_FLOAT } },
                { "TEXCOORD_0", { DXGI_FORMAT_R32G32_FLOAT } },
                { "JOINTS_0", { DXGI_FORMAT_R16G16B16A16_UINT } },
                { "WEIGHTS_0", { DXGI_FORMAT_R32G32B32A32_FLOAT } },
            };
            for (std::unordered_map<std::string, BufferView>::const_reference attribute : attributes)
            {
                if (primitive.vertexBufferViews.find(attribute.first) == primitive.vertexBufferViews.end())
                {
                    primitive.vertexBufferViews.insert(std::make_pair(attribute.first, attribute.second));
                }
            }
        }
    }
}

// マテリアルの取得
void GltfModel::FetchMatetials(ID3D11Device* device, const tinygltf::Model& gltfModel)
{
    for (std::vector<tinygltf::Material>::const_reference gltfMaterial : gltfModel.materials)
    {
        std::vector<Material>::reference material = materials.emplace_back();

        material.name = gltfMaterial.name;

        material.data.emissiveFactor[0] = static_cast<float>(gltfMaterial.emissiveFactor.at(0));
        material.data.emissiveFactor[1] = static_cast<float>(gltfMaterial.emissiveFactor.at(1));
        material.data.emissiveFactor[2] = static_cast<float>(gltfMaterial.emissiveFactor.at(2));

        material.data.alphaMode = gltfMaterial.alphaMode == "OPAQUE" ?
            0 : gltfMaterial.alphaMode == "MASK" ? 1 : gltfMaterial.alphaMode == "BLEND" ? 2 : 0;
        material.data.alphaCutoff = static_cast<float>(gltfMaterial.alphaCutoff);
        material.data.doubleSided = gltfMaterial.doubleSided ? 1 : 0;

        material.data.pbrMetallicRoughness.baseColorFactor[0] =
            static_cast<float>(gltfMaterial.pbrMetallicRoughness.baseColorFactor.at(0));
        material.data.pbrMetallicRoughness.baseColorFactor[1] =
            static_cast<float>(gltfMaterial.pbrMetallicRoughness.baseColorFactor.at(1));
        material.data.pbrMetallicRoughness.baseColorFactor[2] =
            static_cast<float>(gltfMaterial.pbrMetallicRoughness.baseColorFactor.at(2));
        material.data.pbrMetallicRoughness.baseColorFactor[3] =
            static_cast<float>(gltfMaterial.pbrMetallicRoughness.baseColorFactor.at(3));
        material.data.pbrMetallicRoughness.baseColorTexture.index =
            gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
        material.data.pbrMetallicRoughness.baseColorTexture.texcoord =
            gltfMaterial.pbrMetallicRoughness.baseColorTexture.texCoord;
        material.data.pbrMetallicRoughness.metallicFactor =
            static_cast<float>(gltfMaterial.pbrMetallicRoughness.metallicFactor);
        material.data.pbrMetallicRoughness.roughnessFactor =
            static_cast<float>(gltfMaterial.pbrMetallicRoughness.roughnessFactor);
        material.data.pbrMetallicRoughness.metallicRoughnessTexture.index =
            gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index;
        material.data.pbrMetallicRoughness.metallicRoughnessTexture.texcoord =
            gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.texCoord;

        material.data.normalTexture.index = gltfMaterial.normalTexture.index;
        material.data.normalTexture.texcoord = gltfMaterial.normalTexture.texCoord;
        material.data.normalTexture.scale = static_cast<float>(gltfMaterial.normalTexture.scale);

        material.data.occlusionTexture.index = gltfMaterial.occlusionTexture.index;
        material.data.occlusionTexture.texcoord = gltfMaterial.occlusionTexture.texCoord;
        material.data.occlusionTexture.strength =
            static_cast<float>(gltfMaterial.occlusionTexture.strength);

        material.data.emissiveTexture.index = gltfMaterial.emissiveTexture.index;
        material.data.emissiveTexture.texcoord = gltfMaterial.emissiveTexture.texCoord;
    }

    // Create material data as shader resource view on GPU
    std::vector<Material::Cbuffer> materialData;
    for (std::vector<Material>::const_reference material : materials)
    {
        materialData.emplace_back(material.data);
    }

    HRESULT hr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> materialBuffer;
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Material::Cbuffer) * materialData.size());
    bufferDesc.StructureByteStride = sizeof(Material::Cbuffer);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    D3D11_SUBRESOURCE_DATA subresourceData{};
    subresourceData.pSysMem = materialData.data();
    hr = device->CreateBuffer(&bufferDesc, &subresourceData, materialBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
    shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    shaderResourceViewDesc.Buffer.NumElements = static_cast<UINT>(materialData.size());
    hr = device->CreateShaderResourceView(materialBuffer.Get(),
        &shaderResourceViewDesc, materialResourceView.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

// テクスチャの取得
void GltfModel::FetchTexture(ID3D11Device* device, const tinygltf::Model& gltfModel)
{
    HRESULT hr{ S_OK };

    for (const tinygltf::Texture& gltfTexture : gltfModel.textures)
    {
        Texture& texture{ textures.emplace_back() };
        texture.name = gltfTexture.name;
        texture.source = gltfTexture.source;
    }
    for (const tinygltf::Image& gltfImage : gltfModel.images)
    {
        Image& image{ images.emplace_back() };
        image.name = gltfImage.name;
        image.width = gltfImage.width;
        image.height = gltfImage.height;
        image.component = gltfImage.component;
        image.bits = gltfImage.bits;
        image.pixelType = gltfImage.pixel_type;
        image.bufferView = gltfImage.bufferView;
        image.mimeType = gltfImage.mimeType;
        image.uri = gltfImage.uri;
        image.asIs = gltfImage.as_is;

        if (gltfImage.bufferView > -1)
        {
            const tinygltf::BufferView& bufferView{ gltfModel.bufferViews.at(gltfImage.bufferView) };
            const tinygltf::Buffer& buffer{ gltfModel.buffers.at(bufferView.buffer) };
            const byte* data = buffer.data.data() + bufferView.byteOffset;

            ID3D11ShaderResourceView* textureResourceView{};
            hr = load_texture_from_memory(device, data, bufferView.byteLength, &textureResourceView);
            if (hr == S_OK)
            {
                textureResourceViews.emplace_back().Attach(textureResourceView);
            }
        }
        else
        {
            const std::filesystem::path path(filename);
            ID3D11ShaderResourceView* shaderResourceView{};
            D3D11_TEXTURE2D_DESC texture2dDesc;
            std::wstring filename
            {
                path.parent_path().concat(L"/").wstring() +
                std::wstring(gltfImage.uri.begin(),gltfImage.uri.end())
            };
            hr = load_texture_from_file(device, filename.c_str(), &shaderResourceView, &texture2dDesc);
            if (hr == S_OK)
            {
                textureResourceViews.emplace_back().Attach(shaderResourceView);
            }
        }
    }
}

// アニメーションの取得
void GltfModel::FetchAnimation(const tinygltf::Model& gltfModel)
{
    // TODO:アニメーション
    // アニメーションキーフレーム問題(fmodfあのままのコードの場合動かなかったので対処したが他に方法があれば。。)
    // アニメーションがつぶれてるから、fetchAnimationのskinとかボーンとか取るところミスってる可能性大(もしかしたら、transformとかかも、、)

    for (std::vector<tinygltf::Skin>::const_reference transmissionSkin : gltfModel.skins)
    {
        Skin& skin{ skins.emplace_back() };
        const tinygltf::Accessor& gltfAccessor{ gltfModel.accessors.at(transmissionSkin.inverseBindMatrices) };
        const tinygltf::BufferView& gltfBufferView{ gltfModel.bufferViews.at(gltfAccessor.bufferView) };
        skin.inverseBindMatrices.resize(gltfAccessor.count);
        std::memcpy(skin.inverseBindMatrices.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
            gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(DirectX::XMFLOAT4X4));
        skin.joints = transmissionSkin.joints;
    }

    for (std::vector<tinygltf::Animation>::const_reference gltfAnimation : gltfModel.animations)
    {
        Animation& animation{ animations.emplace_back() };
        animation.name = gltfAnimation.name;
        for (std::vector<tinygltf::AnimationSampler>::const_reference gltfSampler : gltfAnimation.samplers)
        {
            Animation::Sampler& sampler{ animation.samplers.emplace_back() };
            sampler.input = gltfSampler.input;
            sampler.output = gltfSampler.output;
            sampler.interpolation = gltfSampler.interpolation;

            const tinygltf::Accessor& gltfAccessor{ gltfModel.accessors.at(gltfSampler.input) };
            const tinygltf::BufferView& gltfBufferView{ gltfModel.bufferViews.at(gltfAccessor.bufferView) };
            std::pair<std::unordered_map<int, std::vector<float>>::iterator, bool>& timelines
            {
                animation.timelines.emplace(gltfSampler.input, gltfAccessor.count)
            };
            if (timelines.second)
            {
                std::memcpy(timelines.first->second.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
                    gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(FLOAT));
            }
        }

        for (std::vector<tinygltf::AnimationChannel>::const_reference gltfChannel : gltfAnimation.channels)
        {
            Animation::Channel& channel{ animation.channels.emplace_back() };
            channel.sampler = gltfChannel.sampler;
            channel.targetNode = gltfChannel.target_node;
            channel.targetPath = gltfChannel.target_path;

            const tinygltf::AnimationSampler& gltfSampler{ gltfAnimation.samplers.at(gltfChannel.sampler) };
            const tinygltf::Accessor& gltfAccessor{ gltfModel.accessors.at(gltfSampler.output) };
            const tinygltf::BufferView& gltfBufferView{ gltfModel.bufferViews.at(gltfAccessor.bufferView) };
            if (gltfChannel.target_path == "scale")
            {
                std::pair<std::unordered_map<int, std::vector<DirectX::XMFLOAT3>>::iterator, bool>& scales
                {
                    animation.scales.emplace(gltfSampler.output, gltfAccessor.count)
                };
                if (scales.second)
                {
                    std::memcpy(scales.first->second.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
                        gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(DirectX::XMFLOAT3));
                }
            }
            else if (gltfChannel.target_path == "rotation")
            {
                std::pair<std::unordered_map<int, std::vector<DirectX::XMFLOAT4>>::iterator, bool>& rotations
                {
                    animation.rotations.emplace(gltfSampler.output, gltfAccessor.count)
                };
                if (rotations.second)
                {
                    std::memcpy(rotations.first->second.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
                        gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(DirectX::XMFLOAT4));
                }
            }
            else if (gltfChannel.target_path == "translation")
            {
                std::pair<std::unordered_map<int, std::vector<DirectX::XMFLOAT3>>::iterator, bool>& translations
                {
                    animation.translations.emplace(gltfSampler.output,gltfAccessor.count)
                };
                if (translations.second)
                {
                    std::memcpy(translations.first->second.data(), gltfModel.buffers.at(gltfBufferView.buffer).data.data() +
                        gltfBufferView.byteOffset + gltfAccessor.byteOffset, gltfAccessor.count * sizeof(DirectX::XMFLOAT3));
                }
            }
        }
    }
}

void GltfModel::CumulateTransforms(std::vector<Node>& nodes)
{
    std::stack<DirectX::XMFLOAT4X4> parentGlobalTransforms;
    std::function<void(int)> traverse{ [&](int nodeIndex)->void
        {
            Node& node{nodes.at(nodeIndex)};
            DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z) };
            DirectX::XMMATRIX R{ DirectX::XMMatrixRotationQuaternion(
            DirectX::XMVectorSet(node.rotation.x, node.rotation.y, node.rotation.z, node.rotation.w)) };
            DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z) };
            DirectX::XMStoreFloat4x4(&node.globalTransform, S * R * T * DirectX::XMLoadFloat4x4(&parentGlobalTransforms.top()));
            for (int childIndex : node.children)
            {
                parentGlobalTransforms.push(node.globalTransform);
                traverse(childIndex);
                parentGlobalTransforms.pop();
            }
    } };
    for (std::vector<int>::value_type nodeIndex : scenes.at(0).nodes)
    {
        parentGlobalTransforms.push(
            {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1
            }
        );
        traverse(nodeIndex);
        parentGlobalTransforms.pop();
    }
}

GltfModel::BufferView GltfModel::MakeBufferView(const tinygltf::Accessor& accessor)
{
    BufferView bufferView;
    switch (accessor.type)
    {
    case TINYGLTF_TYPE_SCALAR:
        switch (accessor.componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            bufferView.format = DXGI_FORMAT_R16_UINT;
            bufferView.strideInBytes = sizeof(USHORT);
            break;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            bufferView.format = DXGI_FORMAT_R32_UINT;
            bufferView.strideInBytes = sizeof(UINT);
            break;
        default:
            _ASSERT_EXPR(FALSE, L"This accessor component type is not supported.");
            break;
        }
        break;
    case TINYGLTF_TYPE_VEC2:
        switch (accessor.componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            bufferView.format = DXGI_FORMAT_R32G32_FLOAT;
            bufferView.strideInBytes = sizeof(FLOAT) * 2;
            break;
        default:
            _ASSERT_EXPR(FALSE, L"This accessor component type is not supported.");
            break;
        }
        break;
    case TINYGLTF_TYPE_VEC3:
        switch (accessor.componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            bufferView.format = DXGI_FORMAT_R32G32B32_FLOAT;
            bufferView.strideInBytes = sizeof(FLOAT) * 3;
            break;
        default:
            _ASSERT_EXPR(FALSE, L"This accessor component type is not supported.");
            break;
        }
        break;
    case TINYGLTF_TYPE_VEC4:
        switch (accessor.componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            bufferView.format = DXGI_FORMAT_R16G16B16A16_UINT;
            bufferView.strideInBytes = sizeof(USHORT) * 4;
            break;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            bufferView.format = DXGI_FORMAT_R32G32B32A32_UINT;
            bufferView.strideInBytes = sizeof(UINT) * 4;
            break;
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            bufferView.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            bufferView.strideInBytes = sizeof(FLOAT) * 4;
            break;
        default:
            _ASSERT_EXPR(FALSE, L"This accessor component type is not suppoeted.");
            break;
        }
        break;
    default:
        _ASSERT_EXPR(FALSE, L"This accessor type is not supported.");
        break;
    }
    bufferView.sizeInBytes = static_cast<UINT>(accessor.count * bufferView.strideInBytes);
    return bufferView;
}