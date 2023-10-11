#pragma once
#include "Stone.h"

class StoneBall : public Stone
{
public:
    StoneBall();
    ~StoneBall() override;

    void Initialize() override;
    void Finalize() override;
    void Update(const float& elapsedTime) override;
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override;
    void DrawDebug() override;

private:
};

