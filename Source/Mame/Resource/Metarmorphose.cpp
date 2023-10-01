#include "Metarmorphose.h"

#include <fstream>
#include <vector>

#include "texture.h"
#include "shader.h"
#include "../Other/Common.h"

Metamorphose::Metamorphose(ID3D11Device* device, const char* filename)
{
    HRESULT hr{ S_OK };

    std::vector<Vertex> vertices;
    
    std::vector<DirectX::XMFLOAT3> positions;
    std::vector<DirectX::XMFLOAT2> texcoords;
    std::vector<DirectX::XMFLOAT3> normals;

    std::wifstream fin(filename);
    _ASSERT_EXPR(fin, L"file not found.");
    wchar_t command[256];
    while (fin)
    {
        fin >> command;
        if (0 == wcscmp(command, L"v"))
        {
            float x, y, z;
            fin >> x >> y >> z;
            positions.push_back({ x,y,z });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vt"))
        {
            float u, v;
            fin >> u >> v;
            texcoords.push_back({ u,1.0f - v });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vn"))
        {
            float i, j, k;
            fin >> i >> j >> k;
            normals.push_back(DirectX::XMFLOAT3(i, j, k));
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"f"))
        {
            for (size_t i = 0; i < 3; ++i)
            {
                Vertex vertex;
                size_t v, vt, vn;

                fin >> v;
                vertex.texcoord = texcoords.at(v - 1);
                if (L'/' == fin.peek())
                {
                    fin.ignore(1);
                    if (L'/' != fin.peek())
                    {
                        fin >> vt;
                        vertex.texcoord = texcoords.at(vt - 1);
                    }
                    if (L'/' == fin.peek())
                    {
                        fin.ignore(1);
                        fin >> vn;
                        vertex.normal = normals.at(vn - 1);
                    }
                }
                vertices.push_back(vertex);
            }
            fin.ignore(1024, L'\n');
        }
        else
        {
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();

    vertexCount = static_cast<UINT>(vertices.size());

    D3D11_BUFFER_DESC bufferDesc{};
    D3D11_SUBRESOURCE_DATA subresourceData{};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    subresourceData.pSysMem = vertices.data();
    subresourceData.SysMemPitch = 0;
    subresourceData.SysMemSlicePitch = 0;
    hr = device->CreateBuffer(&bufferDesc, &subresourceData, vertexBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    bufferDesc.ByteWidth = sizeof(PrimitiveConstants);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    CreateVsFromCso(device, "./Resources/Shader/MetamorphoseVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
    CreateHsFromCso(device, "./Resources/Shader/MetamorphoseHS.cso", hullShader.GetAddressOf());
    CreateDsFromCso(device, "./Resources/Shader/MetamorphoseDS.cso", domainShader.GetAddressOf());
    CreateGsFromCso(device, "./Resources/Shader/MetamorphoseGS.cso", geometryShader.GetAddressOf());
    CreatePsFromCso(device, "./Resources/Shader/MetamorphosePS.cso", pixelShader.GetAddressOf());

    hr = load_texture_from_file(device, L"./Rhino/sai_H.dds", shaderResourceViews[0].ReleaseAndGetAddressOf(),&textture2dDesc);
    hr = load_texture_from_file(device, L"./Rhino/sai_D.dds", shaderResourceViews[1].ReleaseAndGetAddressOf(),&textture2dDesc);
    hr = load_texture_from_file(device, L"./Rhino/sai_N.dds", shaderResourceViews[2].ReleaseAndGetAddressOf(),&textture2dDesc);
    //textture2dDesc=
}

void Metamorphose::Render(ID3D11DeviceContext* deviceContext)
{
}
