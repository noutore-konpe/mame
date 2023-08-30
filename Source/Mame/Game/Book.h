#pragma once

#include "Item.h"

#include "ProjectileManager.h"

class Book : public Item
{
public:
    Book();
    ~Book() override;

    void Initialize()                                           override; // 初期化
    void Finalize()                                             override; // 終了化
    void Begin()                                                override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)                       override; // 更新処理
    void End()                                                  override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime, const float& scale)   override; // 描画処理
    void DrawDebug()                                            override; // ImGui用

public:
    void SetTransform(const float& elapsedTime);

private:
    // ----- 制作に必要な情報 -----
    static int totalNum;    // 現在 "Book" が何個生成されてるか知るためのもの
    int num;                // 自分が今何個目か
    // ----- 制作に必要な情報 -----

    ProjectileManager projectileManager;

    float launchTimer = 0.0f; // 発射時間

    DirectX::XMFLOAT3 createPosition{}; // 生成位置
    DirectX::XMFLOAT3 prevPosition{};   // １フレーム前の位置
    bool isMoveToUp = true;             // 上に移動するか
};