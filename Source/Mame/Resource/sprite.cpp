#include "sprite.h"
#include "../Other/misc.h"
#include <sstream>
 
#include "../../../External/DirectXTK-main/Inc/WICTextureLoader.h"

#include "texture.h"
#include "shader.h"
#include "../Graphics/Graphics.h"

// コンストラクタ
Sprite::Sprite(ID3D11Device* device, const wchar_t* filename, const char* psFilename)
{
    HRESULT hr{ S_OK };

    // 頂点情報のセット
    Vertex vertices[]
    {
        { { -1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } },
        { { +1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 1, 0 } },
        { { -1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
        { { +1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
    };

    // 頂点バッファオブジェクトの生成
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(vertices);           // バッファーのサイズ(バイト単位)
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;            // バッファーの読み取りと書き込み方法を特定します
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;   // バッファーをパイプラインにバインドする方法を特定する
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;// CPUアクセスフラグまた穂CPUアクセスが必要ない場合は０
    buffer_desc.MiscFlags = 0;                          // その他のフラグまたは未使用の場合は０
    buffer_desc.StructureByteStride = 0;                // バッファーが構造化バッファーを表す場合の
                                                        // バッファー構造内の各要素のサイズ

    // D3D11_SUBRESOURCE_DATA構造体 サブリソースを初期化するためのデータを指定します
    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = vertices;    // 初期化データへのポインター 
    subresource_data.SysMemPitch = 0;       // テクスチャの１行の先頭から次の行までの距離(バイト単位)
    subresource_data.SysMemSlicePitch = 0;  // 1つの深度レベルの先頭から次の深さレベルまでの距離(バイト単位)

    // <CreateBuffer> 
    // バッファー（頂点バッファー、インデックスバッファー、またはシェーダー定数バッファー）を作成します
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertexBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



    // 入力レイアウトオブジェクトの生成
    // D3D11_INPUT_ELEMENT_DESC構造体
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };


    // シェーダー関連

    create_vs_from_cso(device, "./resources/Shader/sprite_vs.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
    create_ps_from_cso(device,
        (psFilename != nullptr) ? psFilename : "./resources/Shader/sprite_ps.cso", pixelShader.GetAddressOf());

    //create_vs_from_cso(device, "UVScroll_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));

    //create_ps_from_cso(device, "UVScroll_ps.cso", pixel_shader.GetAddressOf());

    // テクスチャのロード
    load_texture_from_file(device, filename, shaderResourceView.GetAddressOf(), &texture2d_desc);
}

// デストラクタ
Sprite::~Sprite()
{
}

void Sprite::Render()
{
    Graphics& graphics = Graphics::Instance();

    Render(graphics.GetDeviceContext(), spriteTransform.GetPos(), spriteTransform.GetSize(), spriteTransform.GetColor(),
        spriteTransform.GetAngle(), spriteTransform.GetTexPos(), spriteTransform.GetTexSize());
}

void Sprite::Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size)
{
    Render(deviceContext, pos, size, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f),
        DirectX::XMFLOAT2(static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height)));
}

void Sprite::Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color)
{
    Render(deviceContext, pos, size, color, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f),
        DirectX::XMFLOAT2(static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height)));
}

void Sprite::Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color, float angle)
{
    Render(deviceContext, pos, size, color, angle, DirectX::XMFLOAT2(0.0f, 0.0f),
        DirectX::XMFLOAT2(static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height)));
}

// 描画する本体の関数
void Sprite::Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color, float angle/*degree*/, DirectX::XMFLOAT2 texPos, DirectX::XMFLOAT2 texSize)
{
    HRESULT hr{ S_OK };

    // スクリーン（ビューポート）のサイズを取得する
    D3D11_VIEWPORT viewport{};
    UINT numViewports{ 1 };
    deviceContext->RSGetViewports(&numViewports, &viewport);

    // 短形の角頂点の位置（スクリーン座標系）を計算する
    // (x0, y0) *----* (x1, y1)
    //          |   /|
    //          |  / |
    //          | /  |
    //          |/   |
    // (x2, y2) *----* (x3, y3)

    // left-top
    float x0{ pos.x };
    float y0{ pos.y };
    // right-top
    float x1{ pos.x + size.x };
    float y1{ pos.y };
    // left-bottom
    float x2{ pos.x };
    float y2{ pos.y + size.y };
    // right-bottom
    float x3{ pos.x + size.x };
    float y3{ pos.y + size.y };

    // 矩形回転
    auto rotate = [](float& x, float& y, float cx, float cy, float angle)
    {
        x -= cx;
        y -= cy;

        float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
        float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
        float tx{ x }, ty{ y };
        x = cos * tx + -sin * ty;
        y = sin * tx + cos * ty;

        x += cx;
        y += cy;
    };
    // 回転の中心を短形の中心点にした場合
    float cx = pos.x + size.x * 0.5f;
    float cy = pos.y + size.y * 0.5f;
    rotate(x0, y0, cx, cy, angle);
    rotate(x1, y1, cx, cy, angle);
    rotate(x2, y2, cx, cy, angle);
    rotate(x3, y3, cx, cy, angle);

    // スクリーン座標系からNDCへの座標変換をおこなう
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;

    // 計算結果で頂点バッファオブジェクトを更新する
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = deviceContext->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    Vertex* vertices{ reinterpret_cast<Vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0, 0 };
        vertices[1].position = { x1, y1, 0 };
        vertices[2].position = { x2, y2, 0 };
        vertices[3].position = { x3, y3, 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

        vertices[0].texcord = { texPos.x / texture2d_desc.Width, texPos.y / texture2d_desc.Height };
        vertices[1].texcord = { (texPos.x + texSize.x) / texture2d_desc.Width, texPos.y / texture2d_desc.Height };
        vertices[2].texcord = { texPos.x / texture2d_desc.Width, (texPos.y + texSize.y) / texture2d_desc.Height };
        vertices[3].texcord = { (texPos.x + texSize.x) / texture2d_desc.Width, (texPos.y + texSize.y) / texture2d_desc.Height };
    }
    deviceContext->Unmap(vertexBuffer.Get(), 0);

    UINT stride{ sizeof(Vertex) };
    UINT offset{ 0 };

    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    deviceContext->IASetInputLayout(inputLayout.Get());

    deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

    deviceContext->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());

    deviceContext->Draw(4, 0);
    //---これより下に何かいても意味ない---//
}

void Sprite::DrawDebug()
{
    ImGui::Begin("sprite");

    spriteTransform.DrawDebug();

    ImGui::End();
}

void Sprite::SpriteTransform::DrawDebug()
{
    ImGui::DragFloat2("pos", &pos.x);
    ImGui::DragFloat2("size", &size.x);
    ImGui::ColorEdit4("color", &color.x);
    ImGui::DragFloat("angle", &angle);
    ImGui::DragFloat2("texPos", &texPos.x);
    ImGui::DragFloat2("texSize", &texSize.x);
}
