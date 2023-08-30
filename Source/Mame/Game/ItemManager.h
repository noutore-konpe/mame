#pragma once

#include "Item.h"
#include <vector>
#include <set>

class ItemManager
{
private:
    ItemManager() {}
    ~ItemManager() {}

public:
    // 唯一のインスタンス取得
    static ItemManager& Instance()
    {
        static ItemManager instance;
        return instance;
    }

    void Initialize();                                          // 初期化
    void Finalize();                                            // 終了化
    void Begin();                                               // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);                      // 更新処理
    void End();                                                 // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime, const float& scale);  // 描画処理
    void DrawDebug();                                           // ImGui用

public:
    void Register(Item* item);  // 登録
    void Remove(Item* item);    // 削除
    void Clear();               // 全削除

public: // 取得・設定　関連
    const int GetItemCount() const { return static_cast<int>(items.size()); } // アイテム数取得

    Item* GetItem(int index) { return items.at(index); } // アイテム取得

private:
    std::vector<Item*>  items   = {};
    std::set<Item*>     removes = {};
};

