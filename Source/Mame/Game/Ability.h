#pragma once

#include "../Graphics/Model.h"

class AbilityManager;

class Ability
{
public: // enum�֘A
    enum class TAG
    {
        BOOK,       // �{
        BLACK_HOLE, // �u���b�N�z�[��
        NONE,       // �Ȃ�
    };

public:
    Ability(AbilityManager* abilityManager);
    virtual ~Ability() {}

    virtual void Initialize() {}                       // ������
    virtual void Finalize() {}                         // �I����
    virtual void Begin() {}                            // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float elapsedTime) {}   // �X�V����
    virtual void End() {}                              // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float scale, ID3D11PixelShader* psShader = nullptr);   // �`�揈��
    virtual void DrawDebug();                           // ImGui�p

    void Destroy();

public: // �擾�E�ݒ� �֘A
    Transform* GetTransform() { return model_->GetTransform(); }

    const TAG& GetMyTag() const { return tag_; }
    void SetMyTag(const TAG& tag) { tag_ = tag; }

    const std::string& GetName() const { return name_; }
    void SetName(const std::string& name) { name_ = name; }

public:
    std::unique_ptr<Model>  model_;
    TAG                     tag_ = TAG::NONE;

protected:
    AbilityManager* abilityManager_ = nullptr;
    std::string     name_           = "";

};

