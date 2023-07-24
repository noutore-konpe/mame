#include "CannonBallManager.h"
#include "Common.h"

// コンストラクタ
CannonBallManager::CannonBallManager()
{
}

// デストラクタ
CannonBallManager::~CannonBallManager()
{
    Clear();
}

// 更新処理
void CannonBallManager::Update(const float& elapsedTime)
{
    for (CannonBall*& cannonBall : cannonballs)
    {
        cannonBall->Update(elapsedTime);
    }

    for (CannonBall* cannonBall : removes)
    {
        std::vector<CannonBall*>::iterator it = std::find(cannonballs.begin(), cannonballs.end(), cannonBall);

        if (it != cannonballs.end())
        {
            cannonballs.erase(it);
        }

        SafeDelete(cannonBall);
    }
    removes.clear();
}

// 描画処理
void CannonBallManager::Render(const float& elapsedTime)
{
    for (CannonBall*& cannonBall : cannonballs)
    {
        cannonBall->Render(elapsedTime);
    }
}

void CannonBallManager::DrawDebug()
{
    for (CannonBall*& cannonBall : cannonballs)
    {
        cannonBall->DrawDebug();
    }
}

// 弾丸登録
void CannonBallManager::Register(CannonBall* cannonBall)
{
    cannonballs.emplace_back(cannonBall);
}

// 弾丸全削除
void CannonBallManager::Clear()
{
    for (CannonBall*& cannonBall : cannonballs)
    {
        SafeDelete(cannonBall);
    }
    cannonballs.clear();
    cannonballs.shrink_to_fit();
}

// 弾丸削除
void CannonBallManager::Remove(CannonBall* cannonBall)
{
    removes.insert(cannonBall);
}
