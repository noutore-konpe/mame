#pragma once

#include <memory>

#include "Character.h"

#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

class BaseCharacterAI : public Character
{
public:
    BaseCharacterAI() {}
    virtual ~BaseCharacterAI();

    virtual void Initialize() {}                      // 初期化
    virtual void Finalize() {}                        // 終了化
    virtual void Begin() {}                           // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float elapsedTime);     // 更新処理
    virtual void End() {}                             // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float elapsedTime, const float scale); // 描画処理
    virtual void DrawDebug();                         // デバッグ描画

protected:
    std::unique_ptr<BehaviorTree>   behaviorTree_;
    std::unique_ptr<BehaviorData>   behaviorData_; // 主にシーケンスに使う
    std::unique_ptr<NodeBase>       activeNode_;

};

