#include "AbilityManager.h"

#include "../../Taki174/Common.h"

// 初期化
void AbilityManager::Initialize()
{
    for (Ability*& ability : abilitys_)
    {
        ability->Initialize();
    }
}

// 終了化
void AbilityManager::Finalize()
{
    for (Ability*& ability : abilitys_)
    {
        ability->Finalize();
    }
    Clear();
}

// Updateの前に呼ばれる
void AbilityManager::Begin()
{
    for (Ability*& ability : abilitys_)
    {
        ability->Begin();
    }
}

// 更新処理
void AbilityManager::Update(const float elapsedTime)
{
    // 更新処理
    for (Ability*& ability : abilitys_)
    {
        ability->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (Ability* ability : removes_)
        {
            auto it = std::find(abilitys_.begin(), abilitys_.end(), ability);

            if (it != abilitys_.end())
            {
                abilitys_.erase(it);
            }
            SafeDeletePtr(ability);
        }
        removes_.clear();
    }

}

// Updateの後に呼ばれる
void AbilityManager::End()
{
    for (Ability*& ability : abilitys_)
    {
        ability->End();
    }
}

// 描画処理
void AbilityManager::Render(const float scale)
{
    for (Ability*& ability : abilitys_)
    {
        ability->Render(scale);
    }
}

// ImGui用
void AbilityManager::DrawDebug()
{
    for (Ability*& ability : abilitys_)
    {
        ability->DrawDebug();
    }
}

// 登録
void AbilityManager::Register(Ability* ability)
{
    abilitys_.emplace_back(ability);
}

// 削除
void AbilityManager::Remove(Ability* ability)
{
    // 破棄リストに追加
    removes_.insert(ability);
}

// 全削除
void AbilityManager::Clear()
{
    for (Ability*& ability : abilitys_)
    {
        SafeDeletePtr(ability);
    }
    abilitys_.clear();
    abilitys_.shrink_to_fit(); // vectorの余分なメモリを開放する関数(C++11)
}
