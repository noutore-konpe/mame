#include "GeometricPrimitive.h"
#include "shader.h"
#include "../Other/misc.h"



// コンストラクタ
GeometricPrimitive::GeometricPrimitive(ID3D11Device* device)
{
    // 正方形
#if 1
    DirectX::XMFLOAT3 position[]
    {
        { -1.0f, -1.0f, -1.0f },    // 0
        { 1.0f, -1.0f, -1.0f },     // 1
        { 1.0f, 1.0f, -1.0f },      // 2
        { -1.0f, 1.0f, -1.0f },     // 3

        { -1.0f, -1.0f, 1.0f },     // 4

        { 1.0f, -1.0f, 1.0f },      // 5
        { 1.0f, 1.0f, 1.0f },       // 6
        
        { -1.0f, 1.0f, 1.0f },      // 7
    };

    vertex vertices[24]
    {
#if 1
        // front
        { position[0], { 0, 0, -1 } },  // 0        
        { position[1], { 0, 0, -1 } },  // 1
        { position[2], { 0, 0, -1 } },  // 2
        { position[3], { 0, 0, -1 } },  // 3

        // right
        { position[1], { 1, 0, 0 } },   // 1
        { position[5], { 1, 0, 0 } },   // 5
        { position[6], { 1, 0, 0 } },   // 6
        { position[2], { 1, 0, 0 } },   // 2

        // back
        { position[5], { 0, 0, 1 } },   // 5
        { position[4], { 0, 0, 1 } },   // 4
        { position[7], { 0, 0, 1} },    // 7
        { position[6], { 0, 0, 1} },    // 6

        // left        
        { position[4], { -1, 0, 0 } },  // 4
        { position[0], { -1, 0, 0 } },  // 0
        { position[3], { -1, 0, 0 } },  // 3
        { position[7], { -1, 0, 0} },   // 7

        // top
        { position[3], { 0, 1, 0 } },   // 3
        { position[2], { 0, 1, 0 } },   // 2
        { position[6], { 0, 1, 0 } },   // 6
        { position[7], { 0, 1, 0 } },   // 7

        // bottom
        { position[4], { 0, -1, 0 } },  // 4
        { position[5], { 0, -1, 0 } },  // 5
        { position[1], { 0, -1, 0 } },  // 1
        { position[0], { 0, -1, 0 } },  // 0
#endif

    };
    // サイズが1.0の正立方体データを作成する(重心を原点にする)。正立方体のコントロールポイント数は8個、
    // 1つのコントロールポイントの位置には法線の向きが違う頂点が3個あるので頂点情報の総数は8x3=24個
    // 頂点情報配列(vertices)にすべて頂点の位置・法線場を格納する。

    uint32_t indices[36]
    {
#if 1
        // front
        0,3,2,
        0,2,1,
        // right
        4,7,6,
        4,6,5,
        // back
        8,11,10, 
        8,10,9,
        // left
        12,15,14, 
        12,14,13,
        // top
        16,19,18, 
        16,18,17,
        //bottom
        20,23,22,
        20,22,21
#endif
    };
    // 正立方体は6面持ち、1つの面は2つの三角形ポリゴンで構成されるので三角形ポリゴンの総数は6x2=12個、
    // 正立方体を描画するために12回の三角形ポリゴン描画が必要、よって参照される頂点情報は12x3=36回、
    // 三角形ポリゴンが参照する頂点情報のインデックス（頂点番号）をbっ洋画順に配列（indices）に格納する。
    // 時計回りが表面になるように格納すること。

    // 正方形
    create_com_buffers(device, vertices, 24, indices, 36);
#endif

    HRESULT hr{ S_OK };

    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},

    };
    CreateVsFromCso(device, "./resources/Shader/geometric_primitive_vs.cso", vertex_shader.GetAddressOf(),
        input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    CreatePsFromCso(device, "./resources/Shader/geometric_primitive_ps.cso", pixel_shader.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

GeometricPrimitive::GeometricPrimitive(ID3D11Device* device, DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 range)
{

    DirectX::XMFLOAT3 position[]
    {
        { center.x - range.x, center.y, center.z - range.z },
        { center.x + range.x, center.y, center.z - range.z },
        { center.x + range.x, center.y + range.y * 2, center.z - range.z },
        { center.x - range.x, center.y + range.y * 2, center.z - range.z },

        { center.x - range.x, center.y, center.z + range.z },

        { center.x + range.x, center.y, center.z + range.z },
        { center.x + range.x, center.y + range.y * 2, center.z + range.z },

        { center.x - range.x, center.y + range.y * 2, center.z + range.z },


        //{ -1.0f, -1.0f, -1.0f },    // 0
        //{ 1.0f, -1.0f, -1.0f },     // 1
        //{ 1.0f, 1.0f, -1.0f },      // 2
        //{ -1.0f, 1.0f, -1.0f },     // 3

        //{ -1.0f, -1.0f, 1.0f },     // 4

        //{ 1.0f, -1.0f, 1.0f },      // 5
        //{ 1.0f, 1.0f, 1.0f },       // 6

        //{ -1.0f, 1.0f, 1.0f },      // 7
    };

    vertex vertices[24]
    {

        // front
        { position[0], { 0, 0, -1 } },  // 0        
        { position[1], { 0, 0, -1 } },  // 1
        { position[2], { 0, 0, -1 } },  // 2
        { position[3], { 0, 0, -1 } },  // 3

        // right
        { position[1], { 1, 0, 0 } },   // 1
        { position[5], { 1, 0, 0 } },   // 5
        { position[6], { 1, 0, 0 } },   // 6
        { position[2], { 1, 0, 0 } },   // 2

        // back
        { position[5], { 0, 0, 1 } },   // 5
        { position[4], { 0, 0, 1 } },   // 4
        { position[7], { 0, 0, 1} },    // 7
        { position[6], { 0, 0, 1} },    // 6

        // left        
        { position[4], { -1, 0, 0 } },  // 4
        { position[0], { -1, 0, 0 } },  // 0
        { position[3], { -1, 0, 0 } },  // 3
        { position[7], { -1, 0, 0} },   // 7

        // top
        { position[3], { 0, 1, 0 } },   // 3
        { position[2], { 0, 1, 0 } },   // 2
        { position[6], { 0, 1, 0 } },   // 6
        { position[7], { 0, 1, 0 } },   // 7

        // bottom
        { position[4], { 0, -1, 0 } },  // 4
        { position[5], { 0, -1, 0 } },  // 5
        { position[1], { 0, -1, 0 } },  // 1
        { position[0], { 0, -1, 0 } },  // 0
    };

    uint32_t indices[36]
    {
        // front
        0,3,2,
        0,2,1,
        // right
        4,7,6,
        4,6,5,
        // back
        8,11,10,
        8,10,9,
        // left
        12,15,14,
        12,14,13,
        // top
        16,19,18,
        16,18,17,
        //bottom
        20,23,22,
        20,22,21
    };

    create_com_buffers(device, vertices, 24, indices, 36);

    HRESULT hr{ S_OK };

    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},

    };
    CreateVsFromCso(device, "./resources/Shader/geometric_primitive_vs.cso", vertex_shader.GetAddressOf(),
        input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    CreatePsFromCso(device, "./resources/Shader/geometric_primitive_ps.cso", pixel_shader.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

// render
void GeometricPrimitive::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
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

    constants data{ world,material_color };
    immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
    immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    index_buffer->GetDesc(&buffer_desc);
    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);

        
}


void GeometricPrimitive::create_com_buffers(
    ID3D11Device* device,
    const vertex* vertices, const size_t& vertex_count,
    const uint32_t* indices, const size_t& index_count)
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

    //HRESULT hr{ S_OK };

    //D3D11_BUFFER_DESC buffer_desc{};
    //D3D11_SUBRESOURCE_DATA subresource_data{};
    //buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
    //buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    //buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //buffer_desc.CPUAccessFlags = 0;
    //buffer_desc.MiscFlags = 0;
    //buffer_desc.StructureByteStride = 0;
    //subresource_data.pSysMem = vertices;
    //subresource_data.SysMemPitch = 0;
    //subresource_data.SysMemSlicePitch = 0;
    //hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    //buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    //buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    //buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    //subresource_data.pSysMem = indices;
    //hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}


GeometricAABB::GeometricAABB(
    ID3D11Device* device,
    const DirectX::XMFLOAT3& min,
    const DirectX::XMFLOAT3& max)
    : GeometricPrimitive(device)
{
    const size_t vertexCount = 24;
    const size_t indexCount = 36;

    const vertex vertices[vertexCount] = {
        // top
        { { min.x, max.y, max.z }, {  0.0f,  1.0f,  0.0f } }, // 0
        { { max.x, max.y, max.z }, {  0.0f,  1.0f,  0.0f } }, // 1
        { { min.x, max.y, min.z }, {  0.0f,  1.0f,  0.0f } }, // 2
        { { max.x, max.y, min.z }, {  0.0f,  1.0f,  0.0f } }, // 3
        // bottom                                    
        { { min.x, min.y, min.z }, {  0.0f, -1.0f,  0.0f } }, // 4
        { { max.x, min.y, min.z }, {  0.0f, -1.0f,  0.0f } }, // 5
        { { min.x, min.y, max.z }, {  0.0f, -1.0f,  0.0f } }, // 6
        { { max.x, min.y, max.z }, {  0.0f, -1.0f,  0.0f } }, // 7
        // front                                          
        { { min.x, max.y, min.z }, {  0.0f,  0.0f, -1.0f } }, // 8
        { { max.x, max.y, min.z }, {  0.0f,  0.0f, -1.0f } }, // 9
        { { min.x, min.y, min.z }, {  0.0f,  0.0f, -1.0f } }, // 10
        { { max.x, min.y, min.z }, {  0.0f,  0.0f, -1.0f } }, // 11
        // back                                            
        { { max.x, max.y, max.z }, {  0.0f,  0.0f,  1.0f } }, // 12
        { { min.x, max.y, max.z }, {  0.0f,  0.0f,  1.0f } }, // 13
        { { max.x, min.y, max.z }, {  0.0f,  0.0f,  1.0f } }, // 14
        { { min.x, min.y, max.z }, {  0.0f,  0.0f,  1.0f } }, // 15
        // left                                          
        { { min.x, max.y, max.z }, { -1.0f,  0.0f,  0.0f } }, // 16
        { { min.x, max.y, min.z }, { -1.0f,  0.0f,  0.0f } }, // 17
        { { min.x, min.y, max.z }, { -1.0f,  0.0f,  0.0f } }, // 18
        { { min.x, min.y, min.z }, { -1.0f,  0.0f,  0.0f } }, // 19
        // right                                           
        { { max.x, max.y, min.z }, {  1.0f,  0.0f,  0.0f } }, // 20
        { { max.x, max.y, max.z }, {  1.0f,  0.0f,  0.0f } }, // 21
        { { max.x, min.y, min.z }, {  1.0f,  0.0f,  0.0f } }, // 22
        { { max.x, min.y, max.z }, {  1.0f,  0.0f,  0.0f } }, // 23
    };

    const uint32_t indices[indexCount] = {
        // 0------1    ・-----・         ・
        // |    ／|         ／            |
        // | ／   |       ／              |
        // 2------3     ↙         ←-----・

        // top
        0, 1, 2, 1, 3, 2,
        // bottom
        4, 5, 6, 5, 7, 6,
        // front
        8, 9, 10, 9, 11, 10,
        // back
        12, 13, 14, 13, 15, 14,
        // left
        16, 17, 18, 17, 19, 18,
        // right
        20, 21, 22, 21, 23, 22
    };

    create_com_buffers(device, vertices, vertexCount, indices, indexCount);
}
