#pragma once
#include "BaseScene.h"

#include "../Resource/sprite.h"
#include "../Resource/GltfModel.h"
#include "../Graphics/Model.h"
#include "../Graphics/Effect.h"

#include <memory>

class SceneDemo : public Mame::Scene::BaseScene
{
public:
    SceneDemo() {}
    ~SceneDemo()override {}

    void CreateResource()override;                  // ���\�[�X����
    void Initialize()   override;                   // ������
    void Finalize()     override;                   // �I����
    void Begin()        override;                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End()          override;                   // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime) override; // �`�揈��

    void DrawDebug()    override;

public:
    

private:
    bool isDebugCamera = false;

private:
    // GltfModel test
    std::unique_ptr<GltfModel> gltfModels[8];
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceViews[8];

    // Model test
    std::unique_ptr<Model> model;

    // Sprite test
    std::unique_ptr<Sprite>sprite;

    // Effekt test
    std::unique_ptr<Effect> effect[4];
};

