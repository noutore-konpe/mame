#pragma once

#include "../Graphics/Model.h"

class Stage
{
public:
    Stage();
    Stage(const char* fbxFilename, const char* psFilename);
    ~Stage();

    void Initialize();                                          // ������
    void Finalize();                                            // �I����
    void Begin();                                               // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);                      // �X�V����
    void End();                                                 // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);  // �`�揈��
    void DrawDebug();                                           // �f�o�b�O�`��

public: // �擾�E�ݒ�@�֘A
    Transform* GetTransform() { return model->GetTransform(); }

private: // Model
    std::unique_ptr<Model> model = nullptr;

public: // --- ImGui�p --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui�p --- //
    std::string name = {};
};

