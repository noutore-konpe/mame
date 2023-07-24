#pragma once
#include "TitleCharacter.h"

class TitleEnemyTofu : public TitleCharacter
{
public:
    TitleEnemyTofu();
    ~TitleEnemyTofu() override {};

    void Initialize();                      // 初期化
    void Finalize() {}                      // 終了化
    void Begin()    {}                      // 毎フレーム一番最初に呼ばれる
    void Update(const float elapsedTime);   // 更新処理
    void End()      {}                      // 毎フレーム一番最後に呼ばれる
    void Render(const float elapsedTime);   // 描画処理

    void DrawDebug() override;              // デバッグ描画

private:
    enum Animation
    {
        Anim_Walk,  // 歩行
        //Anim_Turn,  // 旋回 
    };

};

