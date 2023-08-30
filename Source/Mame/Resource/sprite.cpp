#include "sprite.h"
#include "../Other/misc.h"
#include <sstream>
 
#include "../../../External/DirectXTK-main/Inc/WICTextureLoader.h"

#include "texture.h"
#include "shader.h"
#include "../Graphics/Graphics.h"

//---ImGui名前かぶり防止用---//
int Sprite::nameNum = 0;
//---ImGui名前かぶり防止用---//

// コンストラクタ
Sprite::Sprite(ID3D11Device* device, const wchar_t* filename, const char* psFilename)
{
    HRESULT hr{ S_OK };
    Graphics& graphics = Graphics::Instance();

    // Animation
    animationTime = 0.0f;
    animationFrame = 0.0f;

    // Dissolve
    if (psFilename == "./Resources/Shader/sprite_dissolve_ps.cso")
    {
        isDissolve = true;
    }

    // 頂点情報のセット
    Vertex vertices[]
    {
        { { -1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } },
        { { +1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 1, 0 } },
        { { -1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
        { { +1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
    };

    // 定数バッファ生成
    {
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.ByteWidth = sizeof(vertices);           
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;            
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;   
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0;                          
        bufferDesc.StructureByteStride = 0;                
                                                            
        D3D11_SUBRESOURCE_DATA subresource_data{};
        subresource_data.pSysMem = vertices;  
        subresource_data.SysMemPitch = 0;     
        subresource_data.SysMemSlicePitch = 0;

        hr = device->CreateBuffer(&bufferDesc, &subresource_data, vertexBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // spriteDissolve
        if (isDissolve)
        {
            bufferDesc.ByteWidth = sizeof(DissolveConstants);
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bufferDesc.CPUAccessFlags = 0;
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = 0;
            hr = graphics.GetDevice()->CreateBuffer(&bufferDesc, nullptr, dissolveConstantBuffer.GetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }
    }


    // 入力レイアウトオブジェクトの生成
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
    CreateVsFromCso(device, "./Resources/Shader/sprite_vs.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
    CreatePsFromCso(device,
        (psFilename != nullptr) ? psFilename : "./Resources/Shader/sprite_ps.cso", pixelShader.GetAddressOf());

    //create_vs_from_cso(device, "UVScroll_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
    //create_ps_from_cso(device, "UVScroll_ps.cso", pixel_shader.GetAddressOf());

    // テクスチャのロード
    {
        load_texture_from_file(device, filename, shaderResourceView.GetAddressOf(), &texture2dDesc);

        // spriteDissolve
        if (isDissolve)
        {
            load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/Mask/dissolve_animation.png",
                maskTexture[0].GetAddressOf(), &maskTexture2dDesc[0]);
            load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/Mask/dissolve_animation2.png",
                maskTexture[1].GetAddressOf(), &maskTexture2dDesc[1]);

            load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/Mask/noise.png",
                maskTexture[2].GetAddressOf(), &maskTexture2dDesc[2]);
            load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/Mask/noise1.png",
                maskTexture[3].GetAddressOf(), &maskTexture2dDesc[3]);
            load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/Mask/noise2.png",
                maskTexture[4].GetAddressOf(), &maskTexture2dDesc[4]);
            load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/Mask/noise3.png",
                maskTexture[5].GetAddressOf(), &maskTexture2dDesc[5]);
            load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/Mask/noise4.png",
                maskTexture[6].GetAddressOf(), &maskTexture2dDesc[6]);

            load_texture_from_file(graphics.GetDevice(), L"./Resources/Image/Mask/noise10.png",
                maskTexture[7].GetAddressOf(), &maskTexture2dDesc[7]);
        }
    }

    //---ImGui名前かぶり防止用---//
    name = "Sprite" + std::to_string(nameNum++);
    SetName(name.c_str());    
    //---ImGui名前かぶり防止用---//
}

// デストラクタ
Sprite::~Sprite()
{
}

// 初期化
void Sprite::Initialize()
{
    animationTime = 0.0f;
    animationFrame = 0.0f;
}

// spriteTransform指定できる初期化
void Sprite::Initialize(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 texSize, DirectX::XMFLOAT2 texPos, DirectX::XMFLOAT4 color, float angle)
{
    Initialize();
    spriteTransform.SetPos(pos);
    spriteTransform.SetSize(size);
    spriteTransform.SetTexPos(texPos);
    spriteTransform.SetTexSize(texSize);
    spriteTransform.SetColor(color);
    spriteTransform.SetAngle(angle);
}

void Sprite::Update(const float& elapsedTime)
{
    if (isDissolve)UpdateSpriteDissolve(elapsedTime);
}

void Sprite::UpdateSpriteDissolve(const float& elapsedTime)
{
    if (isFade)
    {
        if (spriteDissolve.GetDissolveType() == 0)
        {
            if (FadeIn(elapsedTime))SetIsFade(false);
        }
        else
        {
            if (FadeOut(elapsedTime))SetIsFade(false);
        }
    }

    // いい感じにFadeになるように設定してる
    spriteDissolve.SetDissolveValue(spriteDissolve.GetDissolveBlackValue() - spriteDissolve.GetDelay());
}

bool Sprite::FadeIn(const float& elapsedTime)
{
    spriteDissolve.SubtractDissolveBlackValue(elapsedTime);

    return spriteDissolve.GetDissolveBlackValue() < 0.0f;
}

bool Sprite::FadeOut(const float& elapsedTime)
{
    spriteDissolve.AddDissolveBlackValue(elapsedTime);

    return spriteDissolve.GetDissolveBlackValue() > 2.0f;
}



// アニメーション関数
void Sprite::PlayAnimation(const float elapsedTime, const float frameSpeed, const float totalAnimationFrame, const bool animationVertical)
{
    animationTime += frameSpeed * elapsedTime;

    const int frame = static_cast<int>(animationTime); // 小数点切り捨て
    animationFrame = static_cast<float>(frame);

    if (animationFrame > totalAnimationFrame)
    {
        animationFrame = 0.0f;
        animationTime = 0.0f;
    }

    DirectX::XMFLOAT2 texPos = spriteTransform.GetTexPos();
    const DirectX::XMFLOAT2 texSize = spriteTransform.GetTexSize();

    if (animationVertical) texPos.y = texSize.y * animationFrame;
    else                   texPos.x = texSize.x * animationFrame;

    spriteTransform.SetTexPos(texPos);
}

void Sprite::Render()
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    shader->SetState(graphics.GetDeviceContext(), 3, 3, 0);

    // spriteDissolve
    if (isDissolve)
    {
        graphics.GetDeviceContext()->PSSetShaderResources(1, 1,
            maskTexture[spriteDissolve.GetMaskTextureValue()].GetAddressOf());
        // 定数バッファの更新
        {
            DissolveConstants dissolve{};
            dissolve.parameters.x = spriteDissolve.GetDissolveValue();
            dissolve.parameters.y = spriteDissolve.GetDissolveBlackValue();
            dissolve.parameters.z = spriteDissolve.GetEdgeThreshold();
            dissolve.edgeColor = spriteDissolve.GetEdgeColor();
            graphics.GetDeviceContext()->UpdateSubresource(dissolveConstantBuffer.Get(), 0, 0, &dissolve, 0, 0);
            graphics.GetDeviceContext()->VSSetConstantBuffers(3, 1, dissolveConstantBuffer.GetAddressOf());
            graphics.GetDeviceContext()->PSSetConstantBuffers(3, 1, dissolveConstantBuffer.GetAddressOf());
        }
    }

    // 描画
    Render(graphics.GetDeviceContext(), spriteTransform.GetPos(), spriteTransform.GetSize(), spriteTransform.GetColor(),
        spriteTransform.GetAngle(), spriteTransform.GetTexPos(), spriteTransform.GetTexSize());
}

void Sprite::Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size)
{
    Render(deviceContext, pos, size, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f),
        DirectX::XMFLOAT2(static_cast<float>(texture2dDesc.Width), static_cast<float>(texture2dDesc.Height)));
}

void Sprite::Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color)
{
    Render(deviceContext, pos, size, color, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f),
        DirectX::XMFLOAT2(static_cast<float>(texture2dDesc.Width), static_cast<float>(texture2dDesc.Height)));
}

void Sprite::Render(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color, float angle)
{
    Render(deviceContext, pos, size, color, angle, DirectX::XMFLOAT2(0.0f, 0.0f),
        DirectX::XMFLOAT2(static_cast<float>(texture2dDesc.Width), static_cast<float>(texture2dDesc.Height)));
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

        vertices[0].texcord = { texPos.x / texture2dDesc.Width, texPos.y / texture2dDesc.Height };
        vertices[1].texcord = { (texPos.x + texSize.x) / texture2dDesc.Width, texPos.y / texture2dDesc.Height };
        vertices[2].texcord = { texPos.x / texture2dDesc.Width, (texPos.y + texSize.y) / texture2dDesc.Height };
        vertices[3].texcord = { (texPos.x + texSize.x) / texture2dDesc.Width, (texPos.y + texSize.y) / texture2dDesc.Height };
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

// debug用関数
void Sprite::DrawDebug()
{
    if (ImGui::BeginMenu(GetName()))
    {
        spriteTransform.DrawDebug();

        if (isDissolve)spriteDissolve.DrawDebug();

        ImGui::DragFloat("animationFrame", &animationFrame);

        if (ImGui::Button("Fade"))SetIsFade(true);

        ImGui::EndMenu();
    }
}

// SpriteTransform
void Sprite::SpriteTransform::DrawDebug()
{
    if (ImGui::TreeNode("spriteTransform"))
    {
        ImGui::DragFloat2("pos", &pos.x);
        ImGui::DragFloat2("size", &size.x);
        ImGui::ColorEdit4("color", &color.x);
        ImGui::DragFloat("angle", &angle);
        ImGui::DragFloat2("texPos", &texPos.x);
        ImGui::DragFloat2("texSize", &texSize.x);
        ImGui::TreePop();
    }
}

// SpriteDissolve
void Sprite::SpriteDissolve::DrawDebug()
{
    if (ImGui::TreeNode("spriteDissolve"))
    {
        ImGui::SliderInt("textureValue", &maskTextureValue, 0, 7);
        ImGui::SliderFloat("dissolveValue", &dissolveValue, 0.0f, 2.0f);
        ImGui::SliderFloat("dissolveBlackValue", &dissolveBlackValue, 0.0f, 2.0f);
        ImGui::SliderFloat("edgeThreshold", &edgeThreshold, 0.0f, 1.0f);
        ImGui::ColorEdit4("edgeColor", &edgeColor.x);
        ImGui::SliderFloat("delay", &delay, 0.0f, 1.0f);
        ImGui::SliderInt("dissolveType", &dissolveType, 0, 1);
        ImGui::TreePop();
    }
}
