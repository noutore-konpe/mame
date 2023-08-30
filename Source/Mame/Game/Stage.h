#pragma once

#include "../Graphics/Model.h"

class Stage
{
public:
    Stage();
    ~Stage();

    void Initialize();                                          // 初期化
    void Finalize();                                            // 終了化
    void Begin();                                               // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);                      // 更新処理
    void End();                                                 // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime, const float& scale);  // 描画処理
    void DrawDebug();                                           // デバッグ描画

public: // 取得・設定　関連
    Transform* GetTransform() { return model->GetTransform(); }

private: // Model
    std::unique_ptr<Model> model = nullptr;

public: // --- ImGui用 --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui用 --- //
    std::string name = {};
};

