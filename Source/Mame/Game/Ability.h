#pragma once

#include "../Graphics/Model.h"

class AbilityManager;

class Ability
{
public: // enum関連
    enum class TAG
    {
        BOOK,       // 本
        BLACK_HOLE, // ブラックホール
        NONE,       // なし
    };

public:
    Ability(AbilityManager* abilityManager);
    virtual ~Ability() {}

    virtual void Initialize() {}                       // 初期化
    virtual void Finalize() {}                         // 終了化
    virtual void Begin() {}                            // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float elapsedTime) {}   // 更新処理
    virtual void End() {}                              // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float scale, ID3D11PixelShader* psShader = nullptr);   // 描画処理
    virtual void DrawDebug();                           // ImGui用

    void Destroy();

public: // 取得・設定 関連
    Transform* GetTransform() { return model_->GetTransform(); }

    const TAG& GetMyTag() const { return tag_; }
    void SetMyTag(const TAG& tag) { tag_ = tag; }

    const std::string& GetName() const { return name_; }
    void SetName(const std::string& name) { name_ = name; }

public:
    std::unique_ptr<Model>  model_;
    TAG                     tag_ = TAG::NONE;

protected:
    AbilityManager* abilityManager_ = nullptr;
    std::string     name_           = "";

};

