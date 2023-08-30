#pragma once

#include "Character.h"

class Player : public Character
{
public: // enum関連
    // ステート
    enum class State
    {
        Idle,   // 待機
        Walk,   // 歩行
    };

private: // enum関連
    // アニメーション
    enum class Animation
    {
    };

public:
    Player();
    ~Player() override;

    void Initialize() override;                     // 初期化
    void Finalize();                                // 終了化

    void Begin();                                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End();                                     // 毎フレーム一番最後に呼ばれる
    
    void Render(const float& elapsedTime, const float& scale) override; // 描画処理
    
    void DrawDebug() override;  // ImGui用
};

