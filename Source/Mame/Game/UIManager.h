#pragma once
#include "UI.h"

#include <memory>
#include <vector>
#include <set>

class UIManager
{
private:
    UIManager() = default;
    ~UIManager() = default;

public:
    static UIManager& Instance()
    {
        static UIManager instance;
        return instance;
    }

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    
    void Update(const float& elapsedTime);  // 更新処理
    
    void Render(const float& elapsedTime);  // 描画処理
    void DrawDebug();                       // デバッグ描画

public:
    void Register(UI* ui);  // UI登録 
    void Remove(UI* ui);    // UI削除  
    void Clear();              // UI全削除

public:
    const int GetUICount()const { return static_cast<int>(UIs.size()); }
    UI* GetUI(int index) { return UIs.at(index); }

    // hp真ん中に行く処理に使う
    void SetUiCenter(bool s) { isUiCenter = s; }
    bool GetUiCenter() { return isUiCenter; }
    bool isUiCenter = false;

    void SetUiTimer(float t) { timer = t; }
    float GetUiTimer() { return timer; }
    float timer = 0.0f;


private:
    std::vector<UI*> UIs;
    std::set<UI*>removes;
};

