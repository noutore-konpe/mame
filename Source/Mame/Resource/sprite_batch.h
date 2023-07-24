#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include <wrl.h>

class sprite_batch
{
public:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;  // 頂点シェーダーインターフェイス
    // 頂点シェーダーインターフェイスは、頂点シェーダーステージを制御する
    // 実行可能プログラム(頂点シェーダー)を管理します。
    // <頂点シェーダー> 頂点データを受け取ってスクリーン座標に変換する計算をする
    // <頂点データ> 座標、色などのポリゴンを構成する為に必要な情報

    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;    // ピクセルシェーダーインターフェイス
    Microsoft::WRL::ComPtr<ID3D11PixelShader> c_pixel_shader;    // ピクセルシェーダーインターフェイス
    // ピクセルシェーダーインターフェイスは、ピクセルシェーダーステージを制御する
    // 実行可能プログラム(ピクセルシェーダー)を管理します
    // <ピクセルシェーダー> ポリゴンの色をピクセル単位で計算する

    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;    // 入力レイアウトインターフェイス
    // 入力レイアウトインターフェイスは、メモリ内にレイアウトされている頂点データを
    // グラフィックスパイプラインの入力アセンブラーステージにフィードする方法の定義を保持します
    // <入力レイアウト> 頂点データにどのような情報があるのか定義し、シェーダーに教えるやつ

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;        // バッファーインターフェイス
    // バッファーインターフェイスは、非構造化メモリであるバッファーリソースにアクセスします
    // バッファーは通常、頂点データまたはインデックスデータを格納します
    // <頂点バッファ> 頂点データをGPUで扱う為の格納用バッファ

    // <描画ステート> ポリゴンの表示方法や、奥行比較方法などの様々な設定をする
    //      今回はラスタライザステートを設定する

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view[2]; // シェーダーリソースビューインターフェイス
    D3D11_TEXTURE2D_DESC texture2d_desc;

    // 頂点フォーマット
    struct vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcord;
    };

    const size_t max_vertices;
    std::vector<vertex>vertices;

public:
    void render(ID3D11DeviceContext* immediate_context);
    void render(ID3D11DeviceContext* immediate_context,
        float dx, float dy, // 短形の左上の座標(スクリーン座標系)
        float dw, float dh, // 短形のサイズ(スクリーン座標系)
        float r, float g, float b, float a, // 色
        float angle/*degree*/);
    void render(ID3D11DeviceContext* immediate_context,
        float dx, float dy, float dw, float dh,
        float r, float g, float b, float a,
        float angle/*degree*/,
        float sx, float sy, float sw, float sh);

    void render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh);

    sprite_batch(ID3D11Device* device, const wchar_t* filename, size_t max_sprites);
    ~sprite_batch();

    void begin(ID3D11DeviceContext* immediate_context);
    void end(ID3D11DeviceContext* immediate_context);
    void change(ID3D11DeviceContext* immediate_context);

    inline auto rotate(float& x, float& y, float cx, float cy, float angle)
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
};
