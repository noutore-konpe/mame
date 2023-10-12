#pragma once

#include "Ability.h"
#include <vector>
#include <set>

class AbilityManager
{
// キャラクターに持たせるのでインスタンス化なし
//private:
//    AbilityManager() {}
//    ~AbilityManager() {}
//
//public:
//    // 唯一のインスタンス取得
//    static AbilityManager& Instance()
//    {
//        static AbilityManager instance;
//        return instance;
//    }
public:
    AbilityManager() {}
    ~AbilityManager() {}

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Begin();                           // 毎フレーム一番最初に呼ばれる
    void Update(const float elapsedTime);   // 更新処理
    void End();                             // 毎フレーム一番最後に呼ばれる
    void Render(const float scale);         // 描画処理
    void DrawDebug();                       // ImGui用

public:
    void Register(Ability* ability);    // 登録
    void Remove(Ability* ability);      // 削除
    void Clear();                       // 全削除

public: // 取得・設定　関連
    const size_t GetAbilityCount() const { return abilitys_.size(); } // アビリティ数取得
    Ability* GetAbility(const size_t index) { return abilitys_.at(index); } // アビリティ取得

private:
    std::vector<Ability*>  abilitys_    = {};
    std::set<Ability*>     removes_     = {};

};

