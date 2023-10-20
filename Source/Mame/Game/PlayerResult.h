#pragma once
#include "Character.h"

class PlayerResult : public Character
{
public:
    PlayerResult();
    ~PlayerResult() override {}

    void Initialize() override;
    void Update(const float elapsedTime) override;
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr, bool isSowrd = true);
    void DrawDebug() override;

    std::unique_ptr<Model> swordModel;
private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> loadPS;

};

