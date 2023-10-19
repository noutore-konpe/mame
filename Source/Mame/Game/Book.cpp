#include "Book.h"
#include "BookState.h"

#include "../Graphics/Graphics.h"

#include "PlayerManager.h"
#include "ProjectileStraight.h"
#include "ProjectileHorming.h"

int Book::totalNum = 0;

// �R���X�g���N�^
Book::Book()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Item/Book.fbx");

    // �X�e�[�g�}�V��
    {
        stateMachine.reset(new StateMachine<State<Book>>);

        GetStateMachine()->RegisterState(new BookState::IdleState(this));
        GetStateMachine()->RegisterState(new BookState::OpenState(this));
        GetStateMachine()->RegisterState(new BookState::JabAttackState(this));
        GetStateMachine()->RegisterState(new BookState::CloseState(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::Idle));
    }

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
    GetTransform()->SetScaleFactor(3.0f);
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
    GetTransform()->SetScaleFactor(3.0f);

    // �X�e�[�g�}�V���X�V
    GetStateMachine()->Update(elapsedTime);

    // Transform�ݒ�
    SetTransform(elapsedTime);

    // �e�ۍX�V����
    projectileManager.Update(elapsedTime);

    // �A�j���[�V�����X�V
    UpdateAnimation(elapsedTime);
}

// Update�̌�ɌĂ΂��
void Book::End()
{
}

// �`�揈��
void Book::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Item::Render(scale, psShader);

    // �e�ە`�揈��
    projectileManager.Render(0.01f);
}

// ImGui�p
void Book::DrawDebug()
{
#ifdef USE_IMGUI
    // �z�[�~���O�e����
    if (ImGui::Button("Projectile Horming"))
    {
        using DirectX::XMFLOAT3;
        const XMFLOAT3& position = this->GetTransform()->GetPosition();
        const XMFLOAT3  forward  = this->GetTransform()->CalcForward();
        ProjectileHorming* projHorming = new ProjectileHorming(&projectileManager, false); // �łȂ�
        projHorming->Launch(position, forward);
    }

    if (ImGui::BeginMenu(GetName()))
    {
        Item::DrawDebug();

        projectileManager.DrawDebug();

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}

// �e����&����
bool Book::LaunchProjectile(const float& elapsedTime)
{
    if (launchTimer <= 0.0f)
    {
        DirectX::XMFLOAT3 bookPosition = GetTransform()->GetPosition();
        DirectX::XMFLOAT3 bookForward = PlayerManager::Instance().GetPlayer()->GetTransform()->CalcForward();

        float length = 0.3f;

        bookPosition = { bookPosition.x + bookForward.x * length, bookPosition.y + bookForward.y * length, bookPosition.z + bookForward.z * length };

        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(bookForward, bookPosition);

        // ���˂܂ł̎��Ԃ�ݒ�
        launchTimer = launchTime;

        return true; // ���˂���
    }
    launchTimer -= elapsedTime;

    return false; // ���˂��ĂȂ�
}


void Book::SetTransform(const float& elapsedTime)
{
    Transform* playerTransform = PlayerManager::Instance().GetPlayer()->GetTransform();
    DirectX::XMFLOAT3 playerPos = playerTransform->GetPosition();

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

    // ��]�l
    DirectX::XMFLOAT4 bookRot = GetTransform()->GetRotation();
    if (bookRot.x > DirectX::XMConvertToRadians(360.0f))bookRot.x -= DirectX::XMConvertToRadians(360.0f);
    if (bookRot.x < DirectX::XMConvertToRadians(0.0f))bookRot.x += DirectX::XMConvertToRadians(360.0f);
    if (bookRot.y > DirectX::XMConvertToRadians(360.0f))bookRot.y -= DirectX::XMConvertToRadians(360.0f);
    if (bookRot.y < DirectX::XMConvertToRadians(0.0f))bookRot.y += DirectX::XMConvertToRadians(360.0f);
    if (bookRot.z > DirectX::XMConvertToRadians(360.0f))bookRot.z -= DirectX::XMConvertToRadians(360.0f);
    if (bookRot.z < DirectX::XMConvertToRadians(0.0f))bookRot.z += DirectX::XMConvertToRadians(360.0f);
    GetTransform()->SetRotation(bookRot);
}