#include "ProjectileStraight.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../Graphics/Graphics.h"

int ProjectileStraight::nameNum_ = 0;

// �R���X�g���N�^
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
    : Projectile(manager)
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    // ImGui���O�ݒ�
    SetName("ProjectileStraite_" + std::to_string(nameNum_++));
}


ProjectileStraight::~ProjectileStraight()
{
    --nameNum_;
}


// ������
void ProjectileStraight::Initialize()
{
    // �Ƃ肠����Launch�֐��ŌĂяo���Ă���

    using DirectX::XMFLOAT3;

    //lifeTimer_ = 10.0f;
    constexpr float scale = 100.0f;
    GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));
}

// �X�V����
void ProjectileStraight::Update(const float& elapsedTime)
{
    // ��������
    {
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) Destroy();
    }

    // �ړ�
    {
        const float speed = speed_ * elapsedTime;
        GetTransform()->AddPosition(direction_ * speed);
    }

}

// �`�揈��
void ProjectileStraight::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Projectile::Render(scale, psShader);
}

// ImGui�p
void ProjectileStraight::DrawDebug()
{
#ifdef USE_IMGUI

    Projectile::DrawDebug();

    if (ImGui::BeginMenu(GetName()))
    {
        ImGui::EndMenu();
    }

#endif // USE_IMGUI
}

// ����
void ProjectileStraight::Launch(
    const DirectX::XMFLOAT3& direction,
    const DirectX::XMFLOAT3& position)
{
    Initialize();

    direction_ = direction;
    GetTransform()->SetPosition(position);
}
