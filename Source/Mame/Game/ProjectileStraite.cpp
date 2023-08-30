#include "ProjectileStraite.h"

#include "../Graphics/Graphics.h"

#include "ItemManager.h"

// �R���X�g���N�^
ProjectileStraite::ProjectileStraite(ProjectileManager* manager)
    :Projectile(manager)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./Resources/Model/Projectile/sqhere.fbx");

    lifeTimer = 10.0f;

    // ImGui���O�ݒ�
    SetName("ProjectileStraite" + std::to_string(nameNum++));
}

// ������
void ProjectileStraite::Initialize()
{
}

// �X�V����
void ProjectileStraite::Update(const float& elapsedTime)
{
    // ��������
    lifeTimer -= elapsedTime;
    if (lifeTimer < 0)
    {
        // �������폜
        Destroy();
    }

    DirectX::XMFLOAT3 position = GetTransform()->GetPosition();

    // �ړ�
    {
        float speed = this->speed * elapsedTime;
        position.x += direction.x * speed;
        position.y += direction.y * speed;
        position.z += direction.z * speed;
    }

    GetTransform()->SetPosition(position);
}

// �`�揈��
void ProjectileStraite::Render(const float& elapsedTime, const float& scale)
{
    Projectile::Render(elapsedTime, scale);
}

// ImGui�p
void ProjectileStraite::DrawDebug()
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
void ProjectileStraite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    GetTransform()->SetPosition(position);
}
