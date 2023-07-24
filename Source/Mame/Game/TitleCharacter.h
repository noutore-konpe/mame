#pragma once

#include "../Mame/Graphics/Model.h"
#include <memory>

// タイトル用キャラクター
class TitleCharacter
{
public:
    TitleCharacter() {}
    virtual ~TitleCharacter() {}

    virtual void Render(const float elapsedTime);   // 描画処理
    virtual void DrawDebug();                       // デバッグ描画

public:  
    Transform* GetTransform() const { return model_->GetTransform(); } // トランスフォーム取得

    // 回転値設定
    void SetRotationY(const float rotationY) { rotationY_ = rotationY; };

protected:
    // 円運動行動処理
    virtual void CircularMotion(const float elapsedTime);

    virtual void Turn(
        const float elapsedTime,
        NO_CONST float vx,
        NO_CONST float vz,
        NO_CONST float turnSpeed
    );

protected:
    // XMFLOATのメンバ変数がYとZで紛らわしいので作成
    struct Vector2D_XZ
    {
        float x = 0.0f;
        float z = 0.0f;
    };

    // 円
    struct Circle
    {
        Vector2D_XZ center = { 0,0 };   // 円の中心
        float       radius = 0.0f;      // 円の半径
    };

protected:
    std::unique_ptr<Model> model_ = nullptr;        // モデル

    Circle  circle_         = { { 0,0 }, 20.0f };   // 円運動の円
    float   rotationY_      = ToRadian(0.0f);       // 回転値(トランスフォームの角度を直接いじると移動速度も影響を受けて爆速になってしまうので別で用意している)
    float   rotationSpeedY_ = ToRadian(-45.0f);     // 回転速度Y（移動速度）
};

