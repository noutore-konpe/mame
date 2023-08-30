#include "static_mesh.h"

#include <vector>
#include <fstream>
#include <filesystem>

#include "shader.h"
#include "../Other/misc.h"
#include "texture.h"

static_mesh::static_mesh(ID3D11Device* device, const wchar_t* obj_filename,bool frontTex)
{
    std::vector<vertex> vertices;   
    std::vector<uint32_t> indices;  
    uint32_t current_index{ 0 };

    std::vector<DirectX::XMFLOAT3> position;    // 位置
    std::vector<DirectX::XMFLOAT3> normals;     // 法線
    std::vector<DirectX::XMFLOAT2> texcoords;
    std::vector<std::wstring> mtl_filenames;

    std::wifstream fin(obj_filename);
    _ASSERT_EXPR(fin, L"OBJ file not found.");
    wchar_t command[256];




    while (fin)
    {
        fin >> command;
        if (0 == wcscmp(command, L"v"))
        {
            float x, y, z;
            fin >> x >> y >> z;
            position.push_back({ x,y,z });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vn"))
        {
            float i, j, k;
            fin >> i >> j >> k;
            normals.push_back({ i,j,k });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vt"))
        {
            float u, v;
            fin >> u >> v;
            if (frontTex)texcoords.push_back({ u,v });
            else texcoords.push_back({ u,1.0f - v });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"f"))
        {
            for (size_t i = 0; i < 3; i++)
            {
                vertex vertex;
                size_t v, vt, vn;

                fin >> v;
                vertex.position = position.at(v - 1);
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
                indices.push_back(current_index++);
            }
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"mtllib"))
        {
            wchar_t mtllib[256];
            fin >> mtllib;
            mtl_filenames.push_back(mtllib);
        }
        else if (0 == wcscmp(command, L"usemtl"))
        {
            wchar_t usemtl[MAX_PATH]{ 0 };
            fin >> usemtl;
            subsets.push_back({ usemtl,static_cast<uint32_t>(indices.size()),0 });
        }
        else
        {
            fin.ignore(1024, L'\n');
        }


    }
    fin.close();

    std::vector<subset>::reverse_iterator iterator = subsets.rbegin();
    iterator->index_count = static_cast<uint32_t>(indices.size()) - iterator->index_start;
    for (iterator = subsets.rbegin() + 1; iterator != subsets.rend(); ++iterator)
    {
        iterator->index_count = (iterator - 1)->index_start - iterator->index_start;

    }


    std::filesystem::path mtl_filename(obj_filename);
    mtl_filename.replace_filename(std::filesystem::path(mtl_filenames[0]).filename());

    fin.open(mtl_filename);
    //_ASSERT_EXPR(fin, L"'MTL file not found");
    


    while (fin)
    {
        fin >> command;
        if (0 == wcscmp(command, L"map_Kd"))
        {
            fin.ignore();
            wchar_t map_kd[256];
            fin >> map_kd;

            std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_kd).filename());
            materials.rbegin()->texture_filename[0] = path;
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"map_bmp") || 0 == wcscmp(command, L"bump"))
        {
            fin.ignore();
            wchar_t map_bump[256];
            fin >> map_bump;
            std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_bump).filename());
            materials.rbegin()->texture_filename[1] = path;
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"newmtl"))
        {
            fin.ignore();
            wchar_t newmtl[256];
            material material;
            fin >> newmtl;
            material.name = newmtl;
            materials.push_back(material);
        }
        else if(0==wcscmp(command,L"Kd"))
        {
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Kd = { r,g,b,1 };
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"Ka"))
        {
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Ka = { r,g,b,1 };
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"Ks"))
        {
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Ks = { r,g,b,1 };
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();



    D3D11_TEXTURE2D_DESC texture2d_desc{};
    //load_texture_from_file(device, texture_filename.c_str(),
    //    shader_resource_view.GetAddressOf(), &texture2d_desc);
    if (materials.size() == 0)
    {
        // ダミーマテリアルの追加
        for (const subset& subset : subsets)
        {
            materials.push_back({ subset.usemtl });
        }
        for (material& material : materials)
        {
            make_dummy_texture(device, material.shader_resource_view[0].GetAddressOf(),
                0xFFFFFFFF, 16);
            make_dummy_texture(device, material.shader_resource_view[1].GetAddressOf(),
                0xFFFF7F7F, 16);
        }
    }
    else
    {
        for (material& material : materials)
        {
            load_texture_from_file(device, material.texture_filename[0].c_str(),
                material.shader_resource_view[0].GetAddressOf(), &texture2d_desc);
            load_texture_from_file(device, material.texture_filename[1].c_str(),
                material.shader_resource_view[1].GetAddressOf(), &texture2d_desc);
        }
    }


    create_com_buffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());


    HRESULT hr{ S_OK };

    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
    };
    CreateVsFromCso(device, "static_mesh_vs.cso", vertex_shader.GetAddressOf(),
        input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    CreatePsFromCso(device, "static_mesh_ps.cso", pixel_shader.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


}

void static_mesh::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
    uint32_t stride{ sizeof(vertex) };
    uint32_t offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(input_layout.Get());


    // シェーダー関連
    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

    


    for (const material& material : materials)
    {
        immediate_context->PSSetShaderResources(0, 1, material.shader_resource_view[0].GetAddressOf());
        immediate_context->PSSetShaderResources(1, 1, material.shader_resource_view[1].GetAddressOf());

        constants data{ world,material_color };

        //DirectX::XMLoadFloat4(&material_color)* DirectX::XMLoadFloat4(&material.Kd)
        // ↓
        DirectX::XMFLOAT4 mat = { material_color.x * material.Kd.x ,material_color.y * material.Kd.y ,
        material_color.z * material.Kd.z ,material_color.w * material.Kd.w };

        DirectX::XMStoreFloat4(&data.material_color, DirectX::XMLoadFloat4(&mat));
        immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
        immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

        for (const subset& subset : subsets)
        {
            if (material.name == subset.usemtl)
            {
                immediate_context->DrawIndexed(subset.index_count, subset.index_start, 0);
            }
        }

    }
    //immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
    //immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

    //D3D11_BUFFER_DESC buffer_desc{};
    //index_buffer->GetDesc(&buffer_desc);
    //immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);

    //immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
}

void static_mesh::create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
{
    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC buffer_desc{};
    D3D11_SUBRESOURCE_DATA subresource_data{};
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    subresource_data.pSysMem = vertices;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresource_data.pSysMem = indices;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

