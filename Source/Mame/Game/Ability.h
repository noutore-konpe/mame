#pragma once

#include "../Graphics/Model.h"

class Ability
{
public: // enum�֘A
    enum class Tag
    {
        BOOK,   // �{
        NONE,   // �Ȃ�
    };

public:
    Ability() {}
    virtual ~Ability() {}

    virtual void Initialize() {};                       // ������
    virtual void Finalize() {};                         // �I����
    virtual void Begin() {};                            // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float& elapsedTime) {};   // �X�V����
    virtual void End() {};                              // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);   // �`�揈��
    virtual void DrawDebug();                           // ImGui�p

public: // �擾�E�ݒ�@�֘A
    Transform* GetTransform() { return model->GetTransform(); }

    Tag* GetMyTag() { return &tag; }
    void SetMyTag(Tag t) { tag = t; }

public:
    std::unique_ptr<Model> model = nullptr;

    Tag tag = Tag::NONE;

public: // --- ImGui�p --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui�p --- //
    std::string name = {};
};

