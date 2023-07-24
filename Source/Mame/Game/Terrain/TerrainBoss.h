#pragma once

#include "Terrain.h"

class TerrainBoss : public Terrain
{
public:
    TerrainBoss(const char* const filename);
    ~TerrainBoss() override;

    void Initialize()                       override; // ‰Šú‰»
    void Finalize()                         override; // I—¹‰»
    void Update(const float& elapsedTime)   override; // XVˆ—
    void Render(const float& elapsedTime)   override; // •`‰æˆ—
    void DrawDebug()                        override;
};

