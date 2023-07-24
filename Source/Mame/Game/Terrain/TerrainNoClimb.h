#pragma once

#include "Terrain.h"

class TerrainNoClimb : public Terrain
{
public:
    TerrainNoClimb(const char* const filename);
    ~TerrainNoClimb() override;

    void Initialize()                       override; // ‰Šú‰»
    void Finalize()                         override; // I—¹‰»
    void Update(const float& elapsedTime)   override; // XVˆ—
    void Render(const float& elapsedTime)   override; // •`‰æˆ—
    void DrawDebug()                        override;
};

