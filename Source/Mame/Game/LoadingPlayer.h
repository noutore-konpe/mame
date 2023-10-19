#pragma once
#include "Character.h"

class LoadingPlayer : public Character
{
public:
    LoadingPlayer();
    ~LoadingPlayer() override {}

    void Initialize() override;                     // ������
    void Finalize();                                // �I����

    void Update(const float elapsedTime) override; // �X�V����
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr) override; // �`�揈��

    void DrawDebug() override;  // ImGui�p

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> loadPS;

    std::unique_ptr<Model> swordModel;
};

