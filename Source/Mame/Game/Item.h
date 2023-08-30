#pragma once

#include "../Graphics/Model.h"

class Item
{
public: // enum関連
    enum class Tag
    {
        BOOK,   // 本
        NONE,   // なし
    };

public:
    Item();
    virtual ~Item() {}

    virtual void Initialize();                                          // 初期化
    virtual void Finalize();                                            // 終了化
    virtual void Begin();                                               // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime);                      // 更新処理
    virtual void End();                                                 // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float& elapsedTime, const float& scale);  // 描画処理
    virtual void DrawDebug();                                           // ImGui用

public: // 取得・設定　関連
    Transform* GetTransform() { return model->GetTransform(); }

    Tag* GetMyTag() { return &tag; }
    void SetMyTag(Tag t) { tag = t; }

public:
    std::unique_ptr<Model> model = nullptr;

    Tag tag = Tag::NONE;

public: // --- ImGui用 --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui用 --- //
    std::string name = {};
};

