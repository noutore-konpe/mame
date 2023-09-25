#include "AbilityManager.h"

// 初期化
void AbilityManager::Initialize()
{
    for (Ability*& ability : abilitys)
    {
        ability->Initialize();
    }
}

// 終了化
void AbilityManager::Finalize()
{
    for (Ability*& ability : abilitys)
    {
        ability->Finalize();
    }

    Clear();
}

// Updateの前に呼ばれる
void AbilityManager::Begin()
{
    for (Ability*& ability : abilitys)
    {
        ability->Begin();
    }
}

// 更新処理
void AbilityManager::Update(const float& elapsedTime)
{
    // 更新処理
    for (Ability*& ability : abilitys)
    {
        ability->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (Ability*& ability : abilitys)
        {
            // vectorから要素を削除する場合はイテレーターで削除
            std::vector<Ability*>::iterator it = std::find(abilitys.begin(), abilitys.end(), ability);

            // std::vectorで管理されている要素を削除するにはerase()関数を使用する
            // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
            if (it != abilitys.end())
            {
                abilitys.erase(it);
            }

            // アビリティの破棄
            delete ability;
        }
        // 破棄リストをクリア
        removes.clear();
    }
}

// Updateの後に呼ばれる
void AbilityManager::End()
{
    for (Ability*& ability : abilitys)
    {
        ability->End();
    }
}

// 描画処理
void AbilityManager::Render(const float& scale)
{
    for (Ability*& ability : abilitys)
    {
        ability->Render(scale);
    }
}

// ImGui用
void AbilityManager::DrawDebug()
{
    for (Ability*& ability : abilitys)
    {
        ability->DrawDebug();
    }
}

// 登録
void AbilityManager::Register(Ability* ability)
{
    abilitys.emplace_back(ability);
}

// 削除
void AbilityManager::Remove(Ability* ability)
{
    // 破棄リストに追加
    removes.insert(ability);
}

// 全削除
void AbilityManager::Clear()
{
    for (Ability*& ability : abilitys)
    {
        delete ability;
    }
    abilitys.clear();
    abilitys.shrink_to_fit(); // vectorの余分なメモリを開放する関数(C++11)
}
