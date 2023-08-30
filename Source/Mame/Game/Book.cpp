#include "Book.h"

#include "../Graphics/Graphics.h"

#include "PlayerManager.h"
#include "ProjectileStraite.h"

int Book::totalNum = 0;

// �R���X�g���N�^
Book::Book()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), 
        "./Resources/Model/Item/Book.fbx");

    GetTransform()->SetScaleFactor(3.0f);

    // Tag��ݒ�
    tag = Tag::BOOK;

    // ���� "Book" ������������Ă邩�m�邽�߂̂���
    num = ++totalNum;

    // ImGui���O�ݒ�
    SetName("Book" + std::to_string(nameNum++));
}

// �f�X�g���N�^
Book::~Book()
{
}

// ������
void Book::Initialize()
{
    Item::Initialize();
}

// �I����
void Book::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void Book::Begin()
{
}

// �X�V����
void Book::Update(const float& elapsedTime)
{
    Item::Update(elapsedTime);

    // Transform�ݒ�
    SetTransform(elapsedTime);

    // �e�ۍX�V����
    projectileManager.Update(elapsedTime);

    if (launchTimer <= 0.0f)
    {
        DirectX::XMFLOAT3 bookPosition = GetTransform()->GetPosition();
        DirectX::XMFLOAT3 bookForward = PlayerManager::Instance().GetPlayer()->GetTransform()->CalcForward();

        float length = 0.3f;

        bookPosition = { bookPosition.x + bookForward.x * length, bookPosition.y + bookForward.y * length, bookPosition.z + bookForward.z * length };

        ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
        projectile->Launch(bookForward, bookPosition);

        launchTimer = 2.0f;
    }
    launchTimer -= elapsedTime;
}

// Update�̌�ɌĂ΂��
void Book::End()
{
}

// �`�揈��
void Book::Render(const float& elapsedTime, const float& scale)
{
    Item::Render(elapsedTime, scale);

    // �e�ە`�揈��
    projectileManager.Render(elapsedTime, 0.1f);
}

// ImGui�p
void Book::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Item::DrawDebug();

        projectileManager.DrawDebug();

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

void Book::SetTransform(const float& elapsedTime)
{
    Transform* playerTransform = PlayerManager::Instance().GetPlayer()->GetTransform();
    DirectX::XMFLOAT3 playerPos = playerTransform->GetPosition();
    DirectX::XMFLOAT4 playerRot = playerTransform->GetRotation();

    // �v���C���[�̉E�x�N�g��
    DirectX::XMFLOAT3 playerRightVec = playerTransform->CalcRight();
    // ��������
    float length = 0.6f;
    playerRightVec = { playerRightVec.x * length, playerRightVec.y * length, playerRightVec.z * length };

    // �����p
    float playerRightVecY = 1.0f;

    // �P�t���[���O�̈ʒu��ۑ�����
    prevPosition = GetTransform()->GetPosition();

    switch (num)
    {
    case 0: break; // �O�̎��͉������Ȃ�
    case 1: // �v���C���[�̉E
        // �v���C���[�̉E�̈ʒu���v�Z
        playerRightVec = { playerPos.x + playerRightVec.x, playerPos.y + playerRightVec.y , playerPos.z + playerRightVec.z };

        // ���������ɂȂ�悤�Ɉʒu����
        playerRightVec.y += playerRightVecY;

        break;

    case 2: // �v���C���[�̍�
        // �v���C���[�̍��̈ʒu���v�Z
        playerRightVec = { playerPos.x - playerRightVec.x, playerPos.y - playerRightVec.y , playerPos.z - playerRightVec.z };

        // ���������ɂȂ�悤�Ɉʒu����
        playerRightVec.y += playerRightVecY;

        break;

    case 3: // �v���C���[�̉E��
        // �v���C���[�̉E�̈ʒu���v�Z
        {
            float l = 0.5f;
            playerRightVec = { playerRightVec.x * l, playerRightVec.y * l, playerRightVec.z * l };
            playerRightVec = { playerPos.x + playerRightVec.x, playerPos.y + playerRightVec.y , playerPos.z + playerRightVec.z };
        }

        // ���������ɂȂ�悤�Ɉʒu����
        playerRightVec.y += playerRightVecY + 0.5f;

    break;

    case 4: // �v���C���[�̍���
        // �v���C���[�̍��̈ʒu���v�Z
        {
            float l = 0.5f;
            playerRightVec = { playerRightVec.x * l, playerRightVec.y * l, playerRightVec.z * l };
            playerRightVec = { playerPos.x - playerRightVec.x, playerPos.y - playerRightVec.y , playerPos.z - playerRightVec.z };
        }

        // ���������ɂȂ�悤�Ɉʒu����
        playerRightVec.y += playerRightVecY + 0.5f;

        break;
    }

    // �����ʒu
    float offset = 0.1f;

    createPosition = playerRightVec;
    createPosition.y = (prevPosition.y > playerRightVec.y - offset - 0.01f) ? prevPosition.y : playerRightVec.y;

    // �ӂ�ӂ킷�鏈��
    {
        float moveY = isMoveToUp ? 0.3f : -0.3f;

        createPosition.y += moveY * elapsedTime;

        // �܂�Ԃ�����
        if (isMoveToUp)
        {   // ���
            if (createPosition.y >= playerRightVec.y + offset)
            {
                createPosition.y = playerRightVec.y + offset;
                isMoveToUp = false;
            }
        }
        else
        {   // ����
            if (createPosition.y <= playerRightVec.y - offset)
            {
                createPosition.y = playerRightVec.y - offset;
                isMoveToUp = true;
            }
        }
    }

    // �ʒu�ݒ�
    GetTransform()->SetPosition(createPosition);

    // ��]���v���C���[�ɍ��킹��
    
    // �{�����������ɂȂ�悤�ɐݒ�
    playerRot.x += DirectX::XMConvertToRadians(50.0f);

    // ��]�l�ݒ�
    GetTransform()->SetRotation(playerRot);
}