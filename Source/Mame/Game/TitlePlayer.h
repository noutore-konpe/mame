#pragma once

#include "TitleCharacter.h"

// タイトル用プレイヤー
class TitlePlayer : public TitleCharacter
{
public:
    TitlePlayer();
    ~TitlePlayer() override {};

    void Initialize();                      // 初期化
    void Finalize() {};                     // 終了化
    void Begin()    {}                      // 毎フレーム一番最初に呼ばれる
    void Update(const float elapsedTime);   // 更新処理
    void End()      {}                         // 毎フレーム一番最後に呼ばれる
    void Render(const float elapsedTime);   // 描画処理

    void DrawDebug() override;              // デバッグ描画

private:
    enum Animation
    {
        //Anim_Idle,      // 待機
        //Anim_Dash,      // ダッシュ（一時的な急加速）
        Anim_Run = 2,       // 走行
        //Anim_Break,     // ブレーキ
        //Anim_JumpInit,  // ジャンプ開始
        //Anim_Jump,      // ジャンプ
        //Anim_Fall,      // 落下
        //Anim_JumpEnd,   // 着地（ジャンプ終了）
        //Anim_HipDrop,   // ヒップドロップ
        //Anim_Walk,      // 歩行
        //Anim_Max,       // アニメーション最大数
    };

};