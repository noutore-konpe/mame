#include "Player.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "AbilityManager.h"

// �R���X�g���N�^
Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // ���f������
    {
        model = std::make_unique<Model>(graphics.GetDevice(),
            //"./Resources/Model/testModel/character.fbx");
            "./Resources/Model/sotai.fbx");
            //"./Resources/Model/sanaModel/mameoall.fbx");
            //"./Resources/Model/testModel/nico.fbx");
    }
}

// �f�X�g���N�^
Player::~Player()
{
}

// ������
void Player::Initialize()
{
    Character::Initialize();

    // �ҋ@�A�j���[�V�����ɐݒ肵�Ă�
    Character::PlayAnimation(0, true);
}

// �I����
void Player::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void Player::Begin()
{
}

// �X�V����
void Player::Update(const float elapsedTime)
{
    Character::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    GamePad& gamePad = Input::Instance().GetGamePad();

    float aLx = gamePad.GetAxisLX();
    float aLy = gamePad.GetAxisLY();

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();

    if (aLx > 0)pos.x += elapsedTime;
    if (aLx < 0)pos.x -= elapsedTime;
    if (aLy > 0)pos.z += elapsedTime;
    if (aLy < 0)pos.z -= elapsedTime;

    GetTransform()->SetPosition(pos);
}

// Update�̌�ɌĂ΂��
void Player::End()
{
}

// �`�揈��
void Player::Render(const float elapsedTime, const float scale)
{
    Character::Render(elapsedTime, scale);
}

// ImGui�p
void Player::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
