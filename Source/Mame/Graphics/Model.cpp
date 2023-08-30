#include "Model.h"
#include "ResourceManager.h"
#include "Graphics.h"

// TODO: リソースマネージャーの使用
#define USE_RESOURCE_MANAGER

Model::Model(ID3D11Device* device, const char* fbx_filename, bool triangulate, float sampling_rate)
{   
#ifdef USE_RESOURCE_MANAGER // リソースマネージャーあり
    skinned_meshes = ResourceManager::Instance().LoadModelResource(
        device, 
        fbx_filename, 
        triangulate, 
        sampling_rate
    );
#else // リソースマネージャーなし
    skinned_meshes = std::make_unique<skinned_mesh>(device, fbx_filename, triangulate, sampling_rate);
#endif

}

Model::Model(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate, float sampling_rate)
{
    skinned_meshes = ResourceManager::Instance().LoadModelResource(
        device, fbx_filename, 
        animation_filenames, 
        triangulate, 
        sampling_rate
    );
}

// 描画
void Model::Render(const float& scale)
{
    Render(scale, 3);
}

void Model::Render(const float& scale, int rastarizeState)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // world行列更新
    DirectX::XMFLOAT4X4 world = {};
    DirectX::XMStoreFloat4x4(&world, GetTransform()->CalcWorldMatrix(scale));

    // ステートセット
    shader->SetState(graphics.GetDeviceContext(), rastarizeState, 0, 0);

    // Model描画
    if (&keyframe)
    {
        skinned_meshes->render(graphics.GetDeviceContext(), world, GetModelColor(), &keyframe);
    }
    else
    {
        skinned_meshes->render(graphics.GetDeviceContext(), world, GetModelColor(), nullptr);
    }
}

void Model::DrawDebug()
{
    GetTransform()->DrawDebug();
}


void Model::PlayAnimation(
    const int& index, const bool& loop,
    const float& speed, const float& blendSeconds)
{
    // 設定用のアニメーション番号が現在のアニメーション番号と同じ場合はreturn
    if (currentAnimationIndex == index) return;

    currentAnimationIndex   = index;    // 再生するアニメーション番号を設定
    currentAnimationSeconds = 0.0f;     // アニメーション再生時間リセット

    animationLoopFlag       = loop;     // ループさせるか
    animationEndFlag        = false;    // 再生終了フラグをリセット

    animationSpeed          = speed;    // アニメーション再生速度

    animationBlendTime      = 0.0f;
    animationBlendSeconds   = blendSeconds;
}


void Model::UpdateBlendRate(float blendRate, const float& elapsedTime)
{
    if (animationBlendTime < animationBlendSeconds)
    {
        animationBlendTime += elapsedTime;
        if (animationBlendTime >= animationBlendSeconds)
        {
            animationBlendTime = animationBlendSeconds;
        }
        blendRate = animationBlendTime / animationBlendSeconds;
        blendRate *= blendRate;
    }
}


void Model::UpdateAnimation(const float& elapsedTime)
{
    // 再生中でないなら処理しない
    if (!IsPlayAnimation()) return;

    // 最終フレーム処理(再生終了フラグが立っていれば再生終了)
    if (animationEndFlag)
    {
        animationEndFlag = false; // 終了フラグをリセット
        currentAnimationIndex = -1;    // アニメーション番号リセット
        return;
    }

    // アニメーション再生時間経過
    currentAnimationSeconds += elapsedTime;

    // 指定のアニメーションデータを取得
    animation& animation = GetAnimation()->at(currentAnimationIndex);

    // 現在のフレームを取得
    const float  frameIndex_float = (currentAnimationSeconds * animation.sampling_rate) * animationSpeed; // 警告がじゃまなので一時的にfloat変数に格納
    const size_t frameIndex = static_cast<const size_t>(frameIndex_float);

    // 最後のフレームを取得
    const size_t frameEnd = (animation.sequence.size() - 1);


    // アニメーションが再生しきっていた場合
    if (frameIndex > frameEnd)
    {
        // ループフラグが立っていれば再生時間を巻き戻す
        if (animationLoopFlag)
        {
            currentAnimationSeconds = 0.0f;
            return;
        }
        // ループなしなら再生終了フラグを立てる
        else
        {
            animationEndFlag = true;
            return;
        }
    }
    // キーフレームが更新されていてアニメーションが再生しきっていないときはアニメーションをスムーズに切り替える
    else if (isDebugBlendAnimation && (keyframe.nodes.size() > 0) && frameIndex < frameEnd)
    {
        // ブレンド率の計算
        float blendRate = 1.0f;
        UpdateBlendRate(blendRate, elapsedTime);

        // キーフレーム取得
        const std::vector<animation::keyframe>& keyframes = animation.sequence;

        // 現在の前後のキーフレームを取得
        const animation::keyframe* keyframeArr[2] = {
            &keyframe,
            &keyframes.at(frameIndex + 1)
        };

        // アニメーションを滑らかに切り替える
        skinned_meshes->blend_animations(keyframeArr, blendRate, keyframe);

        // アニメーショントランスフォーム更新
        skinned_meshes->update_animation(keyframe);
    }
    // キーフレームが一度も更新されていなくてアニメーションが再生しきっていなければ現在のフレームを保存
    else
    {
        keyframe = animation.sequence.at(frameIndex);
    }
}


bool Model::IsPlayAnimation() const
{
    if (currentAnimationIndex < 0) return false;

    const int animationIndexEnd = static_cast<int>(skinned_meshes->animation_clips.size());
    if (currentAnimationIndex >= animationIndexEnd) return false;

    return true;
}
