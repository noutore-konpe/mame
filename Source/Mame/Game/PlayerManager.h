#pragma once

#include "Player.h"

class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {}

public:
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    void Initialize();                                                  // 初期化
    void Finalize();                                                    // 終了化
    void Begin();                                                       // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);                              // 更新処理
    void End();                                                         // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime, const float& scale);          // 描画処理
    void DrawDebug();                                                   // デバッグ描画

public: // 取得・設定 関連
    std::unique_ptr<Player>& GetPlayer() { return player; }
    
private:
    std::unique_ptr<Player> player = nullptr;
};

