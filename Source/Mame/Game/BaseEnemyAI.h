#pragma once

#include "Character.h"

#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

class BaseEnemyAI : public Character
{
public:
    BaseEnemyAI() {}
    virtual ~BaseEnemyAI();

    virtual void Initialize() {}                      // 初期化
    virtual void Finalize() {}                        // 終了化
    virtual void Begin() {}                           // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float elapsedTime);     // 更新処理
    virtual void End() {}                             // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float elapsedTime, const float scale); // 描画処理
    virtual void DrawDebug();                         // デバッグ描画

protected:
    std::unique_ptr<BehaviorTree>   aiTree_;
    std::unique_ptr<BehaviorData>   behaviorData_;
    NodeBase*                       activeNode_ = nullptr;

};

