#include "Terrain.h"
#include "../../Mame/Graphics/Graphics.h"
#include "../PlayerManager.h"
#include "TerrainManager.h"

int   Terrain::nameNum             = 0;
float Terrain::renderLengthXLimit_ = 40.0f;

Terrain::Terrain(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    {
        model_ = std::make_unique<Model>(graphics.GetDevice(), filename, true);

        create_ps_from_cso(graphics.GetDevice(), "./resources/Shader/wireframe.cso", pixel_shaders.GetAddressOf());

        defaultMin_ = model_->skinned_meshes->boundingBox[0] * 0.01f;
        defaultMax_ = model_->skinned_meshes->boundingBox[1] * 0.01f;
        geometricAABB_ = std::make_unique<GeometricAABB>(graphics.GetDevice(), defaultMin_, defaultMax_);
        UpdateAABB();
    }

    // imgui名前かぶり起きないように...
    name = "Terrain" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}


void Terrain::Initialize()
{
    originPosition_ = GetTransform()->GetPosition();    // 初期位置を保存
}


void Terrain::Render(const float& elapsedTime)
{
    // DrawCollを少なくするためにplayerから近いものだけ描画する
    {
        const float terrainPosX = GetTransform()->GetPosition().x;
        const float playerPosX  = PlayerManager::Instance().GetPlayer()->GetTransform()->GetPosition().x;
        const float lengthX     = fabsf(terrainPosX - playerPosX);

        if (lengthX > renderLengthXLimit_) return;
    }


    Graphics& graphics = Graphics::Instance();

    // world行列更新
    NO_CONST DirectX::XMFLOAT4X4 transform = {};
    DirectX::XMStoreFloat4x4(&transform, model_->GetTransform()->CalcWorldMatrix(0.01f));

    // model描画
    if (&model_->keyframe)
    {
        model_->skinned_meshes->render(graphics.GetDeviceContext(), transform, GetMaterialColor(), &model_->keyframe);
    }
    else
    {
        model_->skinned_meshes->render(graphics.GetDeviceContext(), transform, GetMaterialColor(), nullptr);
    }


#if _DEBUG

    // デバッグモデルでの描画
#if 0
    DirectX::XMFLOAT4X4 debugTransform = {};

    // ワールド行列の取得とスケール調整
    DirectX::XMStoreFloat4x4(&debugTransform, debugModel->GetTransform()->CalcWorldMatrix(0.01f));

    // ワールド行列設定
    debugTransform = SetDebugModelTransform(debugTransform);

    // 描画
    debugModel->skinned_meshes.render(graphics.GetDeviceContext(), debugTransform, { 1.0f, 0.0f, 0.0f, 0.2f }, nullptr);
#endif

    // ラスタライザステート作成・設定
#if 0
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state = nullptr;

    // ラスタライザステート作成
    NO_CONST HRESULT hr = {};
    NO_CONST D3D11_RASTERIZER_DESC rasterizer_desc = {};
    //rasterizer_desc.FillMode                = D3D11_FILL_WIREFRAME; // 塗りつぶし
    rasterizer_desc.FillMode = D3D11_FILL_SOLID; // 塗りつぶし
    //rasterizer_desc.CullMode                = D3D11_CULL_BACK;	    // 背面カリング（裏面を描画しない）
    rasterizer_desc.CullMode = D3D11_CULL_NONE;	    // カリングなし

    // ?
    //rasterizer_desc.FrontCounterClockwise   = FALSE;			    // 三角形をを反時計回り（裏側）にするか
    rasterizer_desc.FrontCounterClockwise = TRUE;			    // 三角形をを反時計回り（裏側）にするか

    rasterizer_desc.DepthBias = 0;	                // 指定されたピクセルに追加された深度値
    rasterizer_desc.DepthBiasClamp = 0;	                // ピクセルの最大深度バイアス
    rasterizer_desc.SlopeScaledDepthBias = 0;	                // 指定されたピクセルの傾きのスカラー
    rasterizer_desc.DepthClipEnable = TRUE;	                // 距離によるクリッピングを行うかのフラグ
    rasterizer_desc.ScissorEnable = FALSE;                // シザー矩形カリングを行うかのフラグ
    rasterizer_desc.MultisampleEnable = FALSE;                // マルチサンプリングアンチエイリアス(略称：MSAA)のレンダーターゲットを使用している時、四辺形ラインアンチエイリアスを行うか、アルファラインアンチエイリアスを行うかを決めるフラグ
    rasterizer_desc.AntialiasedLineEnable = FALSE;                // MSAAのレンダーターゲットを使用している時、線分描画でMultisampleEnableがfalseのとき、アンチエイリアスを有効にします
    hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizer_state.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    // ラスタライザステート設定(ワイヤーフレーム)
    graphics.GetDeviceContext()->RSSetState(rasterizer_state.Get());

#else
    // RS番号
    {
        // 0 ソリッド・後ろカリング
        // 1 ワイヤーフレーム・後ろカリング
        // 2 ワイヤーフレーム・カリングなし
        // 3 ソリッド・カリングなし
    }

    // ラスタライザ設定(ソリッド・カリングなし)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 3, 0, 0);

#endif
    // 回転なしワールド行列の作成
    NO_CONST  DirectX::XMFLOAT4X4 noRotationTransform = {};
    {
        const DirectX::XMFLOAT3  scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
        const DirectX::XMFLOAT3& position = GetTransform()->GetPosition();
        const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
        const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMStoreFloat4x4(&noRotationTransform, S * T);
    }

    // カラー設定
    //const DirectX::XMFLOAT4 materialColor = { 1, 0, 0, 0.4f };

    // AABB描画
    //geometricAABB_->render(graphics.GetDeviceContext(), noRotationTransform, debugMaterialColor);

    // ラスタライザ再設定(ソリッド・後ろカリング)
    graphics.GetShader()->SetState(graphics.GetDeviceContext(), 0, 0, 0);

#endif // _DEBUG
}


void Terrain::DrawDebug()
{
    // ImGui描画
    GetTransform()->DrawDebug();
}


void Terrain::Destroy()
{
    TerrainManager::Instance().Remove(this); // 自分を削除リストに追加
}

void Terrain::UpdateAABB()
{
    const DirectX::XMFLOAT3& position = GetTransform()->GetPosition();

    aabb_.min = position + defaultMin_;
    aabb_.max = position + defaultMax_;
}
