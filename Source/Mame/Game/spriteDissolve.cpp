#include "spriteDissolve.h"

#include "../Mame/Graphics/Graphics.h"
#include "../texture.h"
#include "../misc.h"


SpriteDissolve::SpriteDissolve()
{
    Graphics& graphics = Graphics::Instance();

    dissolve_sprite = std::make_unique<Sprite>(graphics.GetDevice(), L"./resources/load.png", "./resources/Shader/sprite_dissolve_ps.cso");
}

SpriteDissolve::SpriteDissolve(const wchar_t* filename)
{
    Graphics& graphics = Graphics::Instance();

    dissolve_sprite = std::make_unique<Sprite>(graphics.GetDevice(), filename);
}

void SpriteDissolve::Initialize()
{
    //spr_dissolve.dissolve_state = state;

    Graphics& graphics = Graphics::Instance();

    HRESULT hr{ S_OK };

    // 定数バッファ
    {
        D3D11_BUFFER_DESC buffer_desc{};
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;
        buffer_desc.ByteWidth = sizeof(dissolve_constants);
        hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, dissolve_constant_buffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    
    
    // mask
    {
        D3D11_TEXTURE2D_DESC mask_texture2d_desc;
        load_texture_from_file(graphics.GetDevice(), L"./resources/mask/dissolve_animation.png",
            mask_texture[0].GetAddressOf(), &mask_texture2d_desc);
        load_texture_from_file(graphics.GetDevice(), L"./resources/mask/dissolve_animation2.png",
            mask_texture[1].GetAddressOf(), &mask_texture2d_desc);

        load_texture_from_file(graphics.GetDevice(), L"./resources/mask/noise.png",
            mask_texture[2].GetAddressOf(), &mask_texture2d_desc);
        load_texture_from_file(graphics.GetDevice(), L"./resources/mask/noise1.png",
            mask_texture[3].GetAddressOf(), &mask_texture2d_desc);
        load_texture_from_file(graphics.GetDevice(), L"./resources/mask/noise2.png",
            mask_texture[4].GetAddressOf(), &mask_texture2d_desc);
        load_texture_from_file(graphics.GetDevice(), L"./resources/mask/noise3.jpg",
            mask_texture[5].GetAddressOf(), &mask_texture2d_desc);
        load_texture_from_file(graphics.GetDevice(), L"./resources/mask/noise4.png",
            mask_texture[6].GetAddressOf(), &mask_texture2d_desc);
        load_texture_from_file(graphics.GetDevice(), L"./resources/mask/noise5.png",
            mask_texture[7].GetAddressOf(), &mask_texture2d_desc);
    }

    // dissolve_sprite
    {
        // シェーダー関連
        //create_vs_from_cso(graphics.GetDevice(), "sprite_vs.cso", dissolve_sprite->GetVertexShaderAddress(), dissolve_sprite->GetInputLayoutAddress(), input_element_desc, _countof(input_element_desc));
        //create_ps_from_cso(graphics.GetDevice(), "sprite_ps.cso", dissolve_sprite->GetPixelShaderAddress());

        

        // mask
        //create_vs_from_cso(graphics.GetDevice(), "./resources/Shader/sprite_dissolve_vs.cso", dissolve_sprite->GetVertexShaderAddress(), dissolve_sprite->GetInputLayoutAddress(), input_element_desc, _countof(input_element_desc));
        //create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/sprite_dissolve_ps.cso", dissolve_sprite->GetPixelShaderAddress());

        //create_vs_from_cso(graphics.GetDevice(), "sprite_fade_vs.cso", dissolve_sprite->GetVertexShaderAddress(), dissolve_sprite->GetInputLayoutAddress(), input_element_desc, ARRAYSIZE(input_element_desc));
        //create_ps_from_cso(graphics.GetDevice(), "sprite_fade_ps.cso", dissolve_sprite->GetPixelShaderAddress());
        
        /*switch (spr_dissolve.dissolve_state)
        {
            case static_cast<int>(Dissolve::Fade):
            case static_cast<int>(Dissolve::Fire):
                break;
                break;
        }*/
    }

}

void SpriteDissolve::Update()
{
    // いい感じにfadeになるように設定してる
    spr_dissolve.dissolve_value = spr_dissolve.dissolve_value1 - spr_dissolve.delay;
}

void SpriteDissolve::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    Shader* shader = graphics.GetShader();
    shader->SetState(graphics.GetDeviceContext(), 3, 3, 0);

    if (dissolve_sprite)
    {
        immediate_context->IASetInputLayout(dissolve_sprite->GetInputLayout());
        immediate_context->VSSetShader(dissolve_sprite->GetVertexShader(), nullptr, 0);
        immediate_context->PSSetShader(dissolve_sprite->GetPixelShader(), nullptr, 0);
        //immediate_context->PSSetSamplers(0, 1, samplerState.GetAddressOf());
        immediate_context->PSSetShaderResources(1, 1, mask_texture[spr_dissolve.mask_texture_value].GetAddressOf());
        // 定数バッファの更新
        {
            dissolve_constants dissolve{};
            dissolve.parameters.x = spr_dissolve.dissolve_value;
            dissolve.parameters.y = spr_dissolve.dissolve_value1;
            dissolve.parameters.z = spr_dissolve.edge_threshold;
            dissolve.edgeColor = spr_dissolve.edgeColor;
            immediate_context->UpdateSubresource(dissolve_constant_buffer.Get(), 0, 0, &dissolve, 0, 0);
            immediate_context->VSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
            immediate_context->PSSetConstantBuffers(3, 1, dissolve_constant_buffer.GetAddressOf());
        }

        dissolve_sprite->render(immediate_context, spr_dissolve.pos.x, spr_dissolve.pos.y, spr_dissolve.posD.x, spr_dissolve.posD.y);
    }
}

void SpriteDissolve::DrawDebug()
{
    ImGui::Begin("sprite_dissolve");

    ImGui::DragFloat2("pos", &spr_dissolve.pos.x);
    ImGui::DragFloat2("posD", &spr_dissolve.posD.x);

    ImGui::SliderFloat("dissolve_value", &spr_dissolve.dissolve_value, 0.0f, 1.0f);
    ImGui::SliderFloat("dissolve_value1", &spr_dissolve.dissolve_value1, 0.0f, 2.0f);

    {
        ImGui::SliderFloat("delay", &spr_dissolve.delay, 0.0f, 1.0f);
    }

    ImGui::SliderInt("mask_texture", &spr_dissolve.mask_texture_value, 0, 11);

    ImGui::ColorEdit4("test", &spr_dissolve.edgeColor.x);

    ImGui::SliderFloat("edge_threshold", &spr_dissolve.edge_threshold, 0.0f, 1.0f);

    // デバッグ用
    ImGui::SliderInt("dissolve_type", &spr_dissolve.dissolve_state, 0, 1);
    
    ImGui::End();
}

// フェイドインを自動的にしてくれる
void SpriteDissolve::FadeIn(float elapsedTime)
{
    spr_dissolve.dissolve_value1 -= elapsedTime;
}

// フェイドアウトを自動的にしてくれる
void SpriteDissolve::FadeOut(float elapsedTime)
{
    spr_dissolve.dissolve_value1 += elapsedTime;

    // いい感じにfadeになるように設定してる
    spr_dissolve.dissolve_value = spr_dissolve.dissolve_value1 - spr_dissolve.delay;
}
