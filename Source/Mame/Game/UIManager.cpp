#include "UIManager.h"
#include "../Game/Common.h"

void UIManager::Initialize()
{
    SetUiCenter(false);
}

void UIManager::Finalize()
{
}



void UIManager::Update(const float& elapsedTime)
{
    for (UI*& ui : UIs)
    {
        // update
    }

    // 破棄処理
    for (UI* ui : removes)
    {
        // vectorから要素を削除する場合はイテレーターで削除
        std::vector<UI*>::iterator it = std::find(UIs.begin(), UIs.end(), ui);

        // std::vectorで管理されている要素を削除するにはerase()関数を使用する
        // (破棄リストのポインタからイテレーターを検索し、erase関数に渡す)
        if (it != UIs.end())
        {
            UIs.erase(it);
        }

        SafeDelete(ui);
    }
    removes.clear();
}



void UIManager::Render(const float& elapsedTime)
{
    for (UI*& ui : UIs)
    {
        if (ui->GetIsRender())ui->Render();
    }
}

void UIManager::DrawDebug()
{
    for (UI*& ui : UIs)
    {
        ui->DrawDebug();
    }
}

void UIManager::Register(UI* ui)
{
    UIs.emplace_back(ui);
}

void UIManager::Remove(UI* ui)
{
    removes.insert(ui);
}

void UIManager::Clear()
{
    for (UI*& ui : UIs)
    {
        SafeDelete(ui);
    }
    UIs.clear();
    UIs.shrink_to_fit();
}
