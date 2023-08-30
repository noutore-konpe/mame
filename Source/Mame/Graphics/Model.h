#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>

#include "../Transform.h"

#include "../Resource/skinned_mesh.h"

class Model
{
public:
    Model(ID3D11Device* device, const char* fbx_filename, bool triangulate = true, float sampling_rate = 0);
    Model(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate = true, float sampling_rate = 0);
    ~Model() {}

    void Render(const float& scale);
    void Render(const float& scale, int rastarizeState);
     
    void DrawDebug();

    Transform* GetTransform() { return &transform; }
    
    void SetModelColor(DirectX::XMFLOAT4 c) { color = c; }
    DirectX::XMFLOAT4 GetModelColor() { return color; }
    
public: // 取得・設定関数関連
    // アニメーションデータ取得
    std::vector<animation>* GetAnimation() { return &skinned_meshes->animation_clips; };

    // 現在のアニメーション番号の取得・設定
    const int GetCurrentAnimationIndex()& { return currentAnimationIndex; }
    void SetCurrentAnimationIndex(const int& animationIndex) { currentAnimationIndex = animationIndex; }

    // アニメーション再生速度設定（途中で再生速度を変えたいときなどに）
    void SetAnimationSpeed(const float& speed) { animationSpeed = speed; }

public: // アニメーション関数関連
    // アニメーション再生設定
    // (アニメーション番号・ループするかどうか・アニメーション再生速度・スムーズ切り替え時間（速度）)
    void PlayAnimation(
        const int&   index,
        const bool&  loop,
        const float& speed        = 1.0f,
        const float& blendSeconds = 1.0f
    );

    // ブレンド率の計算更新処理
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // アニメーション更新処理
    void UpdateAnimation(const float& elapsedTime);

    // アニメーションが再生中かどうか
    bool IsPlayAnimation() const;


public: // デバッグ確認用
    bool isDebugBlendAnimation = true;     // アニメーションブレンドオンオフ

public:
    std::shared_ptr<skinned_mesh> skinned_meshes;
    Transform    transform;

    animation::keyframe keyframe = {};
    DirectX::XMFLOAT4 color = { 1,1,1,1 };

private:
    float       animationSpeed          = 1.0f;     // アニメーション再生速度
    float       currentAnimationSeconds = 0.0f;     // 現在のアニメーション再生時間

    float       animationBlendTime      = 0.0f;     // 現在のブレンドタイマー
    float       animationBlendSeconds   = 0.0f;     // ブレンド時間

    int         currentAnimationIndex   = -1;	    // 現在のアニメーション番号

    bool        animationLoopFlag       = false;    // アニメーションをループ再生するか
    bool        animationEndFlag        = false;    // アニメーションが終了したか

};

