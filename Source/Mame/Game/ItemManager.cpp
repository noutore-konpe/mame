#include "ItemManager.h"

 //初期化
void ItemManager::Initialize()
{
    for (Item*& item : items)
    {
        item->Initialize();
    }
}

// 終了化
void ItemManager::Finalize()
{
    for (Item*& item : items)
    {
        item->Finalize();
    }

    Clear();
}

// Updateの前に呼ばれる
void ItemManager::Begin()
{
    for (Item*& item : items)
    {
        item->Begin();
    }
}

// 更新処理
void ItemManager::Update(const float& elapsedTime)
{
    // 更新処理
    for (Item*& item : items)
    {
        item->Update(elapsedTime);
    }

    // 破棄処理
    {
        for (Item* item : removes)
        {
            // vectorから要素を削除する場合はイテレーターで削除
            std::vector<Item*>::iterator it = std::find(items.begin(), items.end(), item);

            // std::vectorで管理されている要素を削除するにはerase()関数を使用する
            // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
            if (it != items.end())
            {
                items.erase(it);
            }

            // アイテムの破棄
            delete item;
        }
        // 破棄リストをクリア
        removes.clear();
    }
}

// Updateの後に呼ばれる
void ItemManager::End()
{
    for (Item*& item : items)
    {
        item->End();
    }
}

// 描画処理
void ItemManager::Render(const float& elapsedTime, const float& scale)
{
    for (Item*& item : items)
    {
        item->Render(elapsedTime, scale);
    }
}

// ImGui用
void ItemManager::DrawDebug()
{
    for (Item*& item : items)
    {
        item->DrawDebug();
    }
}

// 登録
void ItemManager::Register(Item* item)
{
    items.emplace_back(item);
}

// 削除
void ItemManager::Remove(Item* item)
{
    // 破棄リストに追加
    removes.insert(item);
}

// 全削除
void ItemManager::Clear()
{
    for (Item*& item : items)
    {
        delete item;
    }
    items.clear();
    items.shrink_to_fit();  // vectorの余分なメモリを開放する関数(C++11)
}
