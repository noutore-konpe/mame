#pragma once

#include "../../Taki174/OperatorXMFloat3.h"
#include "Character.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

class Enemy : public Character
{
public:
    enum class TYPE
    {
        Normal,
        Golem,
    };

public:
    Enemy() {}
    virtual ~Enemy();

    virtual void Initialize();                                          // 初期化
    virtual void Finalize() = 0;                                        // 終了化
    virtual void Begin()    = 0;                                           // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime);                      // 更新処理
    virtual void End()      = 0;                                             // 毎フレーム一番最後に呼ばれる
    //virtual void Render(const float& elapsedTime, const float& scale);  // 描画処理
    virtual void Render(const float& scale, ID3D11PixelShader* psShader);  // 描画処理
    virtual void DrawDebug() = 0;                                       // デバッグ描画

    virtual void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr) {}

    virtual int GetCurrentState() { return 0; }

public:
    const TYPE GetType() const { return type; }
    void SetType(const TYPE t) { type = t; }

    const DirectX::XMFLOAT3& GetVelocity() const { return velocity_; }
    void SetVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ = velocity; }
    void AddVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ += velocity; }

    const DirectX::XMFLOAT3& GetBlowOffVec() const { return blowOffVec_; }
    void  SetBlowOffVec(const DirectX::XMFLOAT3& blowOffVec) { blowOffVec_ = blowOffVec; }

    const float GetBlowOffForce() const { return blowOffForce_; }
    void SetBlowOffForce(const float blowOffForce) { blowOffForce_ = blowOffForce; }

    const float GetRunTimer() const { return runTimer_; }
    void SetRunTimer(const float runTimer) { runTimer_ = runTimer; }

    const float GetAnimationSpeed() const { return animationSpeed_; }
    void SetAnimationSpeed(const float animationSpeed) { animationSpeed_ = animationSpeed; }

    const int GetDropExpCount() const { return dropExpCount_; }
    void SetDropExpCount(const int dropExpCount) { dropExpCount_ = dropExpCount; }

    const int GetStep() const { return step_; }
    void SetStep(const int step) { step_ = step; }

    const bool GetEntryStageFlag() const { return entryStageFlag_; }
    void SetEntryStageFlag(const bool entryStageFlag) { entryStageFlag_ = entryStageFlag; }

    const bool GetIsFlinch() const { return isFlinch_; }
    void SetIsFlinch(const bool isFlinch) { isFlinch_ = isFlinch; }

    const bool GetIsBlowOff() const { return isBlowOff_; }
    void SetIsBlowOff(const bool isBlowOff) { isBlowOff_ = isBlowOff; }

    const bool GetIsWaveEnemy() const { return isWaveEnemy_; }
    void SetIsWaveEnemy(const bool isWaveEnemy) { isWaveEnemy_ = isWaveEnemy; }

public:
    // 実行タイマー経過
    void ElapseRunTimer(const float elapsedTime) { runTimer_ = (std::max)(0.0f, runTimer_ - elapsedTime); }

    // ひるませる
    void Flinch();

    // 吹っ飛ばす
    void BlowOff();

    // 消去
    void Destroy();

    // 検索する子ノードが存在するか検索する
    const bool IsExistChildNode(const std::string& findNodeName);

    // 子ノードの数を取得
    const size_t GetChildNodeCount() const
    {
        if (nullptr == behaviorTree_) return 0;

        return behaviorTree_->GetRoot()->GetChildren()->size();
    }

protected:
    std::unique_ptr<BehaviorTree>   behaviorTree_;
    std::unique_ptr<BehaviorData>   behaviorData_;  // 主にシーケンスに使う
    NodeBase*           activeNode_     = nullptr;  // BehaviorTreeのノードを指すだけのポインタなのでdeleteしない

    DirectX::XMFLOAT3   velocity_       = {};
    DirectX::XMFLOAT3   blowOffVec_     = {};       // 吹っ飛ぶ方向ベクトル(未正規化)
    float               blowOffForce_   = 10.0f;    // 吹っ飛ぶ力
    float               runTimer_       = 0.0f;     // 実行タイマー
    float               animationSpeed_ = 1.0f;     // アニメーション速度
    int                 dropExpCount_   = 5;        // ドロップする経験値の数
    int                 step_           = 0;        // 行動ステップ
    bool                entryStageFlag_ = false;    // ステージに入ったかどうかのフラグ
    bool                isFlinch_       = false;    // ひるみフラグ
    bool                isBlowOff_      = false;    // 吹っ飛びフラグ
    bool                isWaveEnemy_    = false;    // ウェーブで生成された敵であるか(デストラクタで使用)

private:
    TYPE type = TYPE::Normal;

};

