#pragma once

#include <vector>
#include <set>
#include "CannonBall.h"


class CannonBallManager
{
public:
    CannonBallManager();
    ~CannonBallManager();

    void Update(const float& elapsedTime);  // XVˆ—   
    void Render(const float& elapsedTime);  // •`‰æˆ—
    void DrawDebug();                       // ƒfƒoƒbƒO•`‰æˆ—

public:  
    void Register(CannonBall* cannonBall);  // ’eŠÛ“o˜^
    void Clear();                           // ’eŠÛ‘Síœ  
    void Remove(CannonBall* cannonBall);    // ’eŠÛíœ

public:
    // ’eŠÛ”æ“¾
    const int GetCannonBallCount() const { return static_cast<int>(cannonballs.size()); }

    // ’eŠÛæ“¾
    CannonBall* GetCannonBall(const int& index) const { return cannonballs.at(index); }

private:
    std::vector<CannonBall*> cannonballs    = {};
    std::set<CannonBall*>    removes        = {};
};

