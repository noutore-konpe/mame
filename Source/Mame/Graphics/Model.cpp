#include "Model.h"
#include "ResourceManager.h"
#include "Graphics.h"

// TODO: リソースマネージャーの使用
#define USE_RESOURCE_MANAGER

Model::Model(ID3D11Device* device, const char* fbx_filename, const char* psFilename, bool triangulate, float sampling_rate)
{   
#ifdef USE_RESOURCE_MANAGER // リソースマネージャーあり
    skinned_meshes = ResourceManager::Instance().LoadModelResource(
        device, 
        fbx_filename, 
        psFilename,
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
void Model::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // world行列更新
    DirectX::XMFLOAT4X4 world = {};
    DirectX::XMStoreFloat4x4(&world, GetTransform()->CalcWorldMatrix(scale));

    // Model描画
    if (&keyframe)
    {
        skinned_meshes->render(graphics.GetDeviceContext(), world, GetModelColor(), &keyframe, psShader);
    }
    else
    {
        skinned_meshes->render(graphics.GetDeviceContext(), world, GetModelColor(), nullptr, psShader);
    }
}

void Model::DrawDebug()
{
    GetTransform()->DrawDebug();

    if (ImGui::TreeNode("Animation"))
    {
        ImGui::SliderFloat("weight", &weight, 0.0f, 1.0f);
        ImGui::SliderFloat("threshold", &blendThreshold, 0.0f, 1.0f);
        ImGui::InputInt("Current Index", &currentAnimationIndex);

        ImGui::SliderFloat("Animation Speed", &animationSpeed, 0.0f, 3.0f);

        ImGui::SliderFloat("BlendRate", &blendRate, 0.0f, 1.0f);

        ImGui::TreePop();
    }
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

void Model::PlayBlendAnimation(const int& index1, const int& index2, const bool& loop, const float& speed)
{
    // 設定用のアニメーション番号が現在のアニメーション番号と同じ場合はreturn
    if (blendAnimationIndex1 == index1 && blendAnimationIndex2 == index2) return;

    blendAnimationIndex1 = index1;    // 再生するアニメーション番号を設定
    blendAnimationIndex2 = index2;    // 再生するアニメーション番号を設定
    blendAnimationSeconds = 0.0f;     // アニメーション再生時間リセット

    animationLoopFlag = loop;     // ループさせるか
    animationEndFlag = false;    // 再生終了フラグをリセット

    animationSpeed = speed;    // アニメーション再生速度

    animationBlendTime = 0.0f;
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

    //ブレンドアニメーションの再生（再生中の場合これ以降の通常再生の処理はしない）
    if (UpdateBlendAnimation(elapsedTime))return;

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

bool Model::UpdateBlendAnimation(const float& elapsedTime)
{
    if(blendAnimationIndex1 < 0)return false;

    if (animationEndFlag)
    {
        animationEndFlag = false; // 終了フラグをリセット
        blendAnimationIndex1 = -1;    // アニメーション番号リセット
        blendAnimationIndex2 = -1;
        return false;
    }

    //アニメーション再生時間更新
    blendAnimationSeconds += elapsedTime;

    animation& bAnimation1 = GetAnimation()->at(blendAnimationIndex1);
    animation& bAnimation2 = GetAnimation()->at(blendAnimationIndex2);

    const size_t frameCount1 = bAnimation1.sequence.size();
    const size_t frameCount2 = bAnimation2.sequence.size();

    //基準となるアニメーションにフレーム数を合わせる
    size_t maxFrameCount = frameCount1;
    if (blendThreshold < weight)maxFrameCount = frameCount2;

    const float frameIndex = (blendAnimationSeconds * bAnimation1.sampling_rate) * animationSpeed;

    //アニメーションが再生しきっている場合
    if (frameIndex > maxFrameCount)
    {
        if (animationLoopFlag)
        {
            blendAnimationSeconds = 0.0f;
            return true;
        }
        else
        {
            animationEndFlag = true;
            return true;
        }
    }

    //再生フレームを正規化して再生時間の長さを合わせる
    UINT64 frameIndex1 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount1 - 1));
    UINT64 frameIndex2 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount2 - 1));

#if 0

    const animation::keyframe* keyframeArr[2] = {
             &bAnimation1.sequence.at(frameIndex1),
             &bAnimation2.sequence.at(frameIndex2)
    };
    animation::keyframe keyframe1;
    skinned_meshes->blend_animations(keyframeArr, weight, keyframe1);

    //フレーム補間するためのキーフレーム
    const animation::keyframe* lerpKeyframeArr[2] = {
             &bAnimation1.sequence.at(frameIndex1 + 1),
             &bAnimation2.sequence.at(frameIndex2 + 1)
    };
    animation::keyframe keyframe2;
    skinned_meshes->blend_animations(lerpKeyframeArr, weight, keyframe2);

    // ブレンド率の計算
    blendRate = frameIndex - static_cast<int>(frameIndex);


    const animation::keyframe* resultKeyframeArr[2] = { &keyframe1 ,&keyframe2 };
    skinned_meshes->blend_animations(resultKeyframeArr, blendRate, keyframe);

#else//なんか上手くできやんので別バージョン

    blendRate = 1.0f;
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

    // キーフレーム取得
    const std::vector<animation::keyframe>& keyframes1 = bAnimation1.sequence;

    // 現在の前後のキーフレームを取得
    const animation::keyframe* keyframeArr1[2] = {
        &keyframes1.at(frameIndex1),
        &keyframes1.at(frameIndex1 + 1)
    };

    // アニメーションを滑らかに切り替える
    animation::keyframe keyframe1;
    skinned_meshes->blend_animations(keyframeArr1, blendRate, keyframe1);

    // キーフレーム取得
    const std::vector<animation::keyframe>& keyframes2 = bAnimation2.sequence;

    // 現在の前後のキーフレームを取得
    const animation::keyframe* keyframeArr2[2] = {
        &keyframes2.at(frameIndex2),
        &keyframes2.at(frameIndex2 + 1)
    };

    // アニメーションを滑らかに切り替える
    animation::keyframe keyframe2;
    skinned_meshes->blend_animations(keyframeArr2, blendRate, keyframe2);

    const animation::keyframe* resultKeyframeArr[2] = { &keyframe1 ,&keyframe2 };
    skinned_meshes->blend_animations(resultKeyframeArr, weight, keyframe);
#endif // 0



    skinned_meshes->update_animation(keyframe);

    return true;
}


bool Model::IsPlayAnimation() const
{
    if (currentAnimationIndex < 0 && blendAnimationIndex1 < 0) return false;

    const int animationIndexEnd = static_cast<int>(skinned_meshes->animation_clips.size());
    if (currentAnimationIndex >= animationIndexEnd && blendAnimationIndex1 >= animationIndexEnd) return false;

    return true;
}
