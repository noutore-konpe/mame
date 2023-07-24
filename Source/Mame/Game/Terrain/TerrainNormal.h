#pragma once

#include "Terrain.h"

class TerrainNormal : public Terrain
{
public:
    TerrainNormal(const char* const filename);
    ~TerrainNormal() override;

    void Initialize()                       override; // ‰Šú‰»
    void Finalize()                         override; // I—¹‰»
    void Update(const float& elapsedTime)   override; // XVˆ—
    void Render(const float& elapsedTime)   override; // •`‰æˆ—
    void DrawDebug()                        override;
};

