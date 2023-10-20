#include "Book.h"
#include "BookState.h"

#include "../../Taki174/FunctionXMFloat3.h"
#include "../Graphics/Graphics.h"

#include "ProjectileStraight.h"
#include "ProjectileHorming.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "Collision.h"

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

    // �e�ۂƓG�Ƃ̏Փˏ���
    CollisionProjectileVsEnemies();
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

void Book::CollisionProjectileVsEnemies()
{
    using DirectX::XMFLOAT3;

    EnemyManager& enmManager = EnemyManager::Instance();

    const size_t projCount = projectileManager.GetProjectileCount();
    for (size_t projIndex = 0; projIndex < projCount; ++projIndex)
    {
        // �e�ێ擾
        Projectile* proj = projectileManager.GetProjectile(projIndex);

        const size_t enmCount = enmManager.GetEnemyCount();
        for (size_t enmIndex = 0; enmIndex < enmCount; ++enmIndex)
        {
            // �G�擾
            Enemy* enemy = enmManager.GetEnemy(enmIndex);

            // �G������ł�����continue
            if (true == enemy->GetIsDead()) continue;

            // �Փ˃t���O
            bool isHit = false;

            const size_t enmHitColliderCount = enemy->GetHitColliderCount();
            for (size_t enmHitColliderIndex = 0; enmHitColliderIndex < enmHitColliderCount; ++enmHitColliderIndex)
            {
                // �G�̐H�炢����擾
                const Character::SphereCollider enmHitCol = enemy->GetHitColliderAt(enmHitColliderIndex);

                const XMFLOAT3& projPos              = proj->GetPosition();
                const XMFLOAT3& enmHitColliderPos    = enmHitCol.position;
                const float     projRadius           = proj->GetRadius();
                const float     enmHitColliderRadius = enmHitCol.radius;

                isHit = Collision::IntersectSphereVsSphere(
                    projPos, projRadius, enmHitColliderPos, enmHitColliderRadius
                );

                // �������ĂȂ����continue
                if (false == isHit) continue;

                // �G�̐�����я���ۑ�
                const XMFLOAT3 vec = enmHitColliderPos - projPos;
                enemy->SaveBlowOffInfo(vec, proj->GetInflictBlowOffForceLevel());

                // �G�Ƀ_���[�W��^����
                enemy->ApplyDamage(proj->GetAttack());

                // �e����������
                proj->Destroy();

                // ���������̂�break
                break;
            }

            // �������Ă�����break
            if (true == isHit) break;
        }
    }
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
    using DirectX::XMFLOAT3;

    Transform* playerT = PlayerManager::Instance().GetPlayer()->GetTransform();

    // �v���C���[�̈ʒu
    const XMFLOAT3& plPos = playerT->GetPosition();

    // �v���C���[�̉E�x�N�g��
    const XMFLOAT3 plRightVecN = playerT->CalcRight();

    // �ړ���
    static const float MOVEMENT = 0.6f;

    // �v���C���[�̉E�x�N�g�������ɐL�т��l(�v���C���[�̈ʒu�ɑ����l)
    const XMFLOAT3 addPlPos = plRightVecN * MOVEMENT;

    // �ʒu�����l
    static const float MODIFY_POS_Y = 1.0f; // Y�ʒu�����l
    static const float MODIFY_POS_X = 0.5f; // X�ʒu�����l

    // �P�t���[���O�̈ʒu��ۑ�����
    prevPosition = GetTransform()->GetPosition();

    // ���̖{�̈ʒu
    XMFLOAT3 bookPosition = {};

    // ����U��ꂽ�ԍ��ɂ���Ĉʒu��ݒ肷��
    switch (num)
    {
    case 1: // �v���C���[�̉E
        // �v���C���[�̉E�̈ʒu���v�Z
        bookPosition = plPos + addPlPos;

        // ���������ɂȂ�悤��Y�ʒu����
        bookPosition.y += MODIFY_POS_Y;

        break;
    case 2: // �v���C���[�̍�
        // �v���C���[�̍��̈ʒu���v�Z
        bookPosition = plPos - addPlPos;

        // ���������ɂȂ�悤��Y�ʒu����
        bookPosition.y += MODIFY_POS_Y;

        break;
    case 3: // �v���C���[�̉E��
        // �v���C���[�̉E�̈ʒu���v�Z
        bookPosition = plPos + (addPlPos * MODIFY_POS_X);

        // ���������ɂȂ�悤��Y�ʒu����
        bookPosition.y += MODIFY_POS_Y + 0.5f;

        break;
    case 4: // �v���C���[�̍���
        // �v���C���[�̍��̈ʒu���v�Z
        bookPosition = plPos - (addPlPos * MODIFY_POS_X);

        // ���������ɂȂ�悤��Y�ʒu����
        bookPosition.y += MODIFY_POS_Y + 0.5f;

        break;
    }

    // �ӂ�ӂ킷�鏈��
#if 1
    // ��]�X�V
    circularMotionRotationZ_ += circularMotionAddRotate_ * elapsedTime;
    if (circularMotionRotationZ_ > 360.0f) { circularMotionRotationZ_ -= 360.0f; }

    // �㉺�Ɉړ�������
    DirectX::XMFLOAT3 circularMotionRotationPos = bookPosition;
    circularMotionRotationPos.y = bookPosition.y + ::cosf(circularMotionRotationZ_) * circularMotionRadius_;

    // �ʒu�ݒ�
    GetTransform()->SetPosition(circularMotionRotationPos);
#else
    // �ʒu�ݒ�
    GetTransform()->SetPosition(createPosition);
#endif

    // ��]�l
    DirectX::XMFLOAT4 bookRot = GetTransform()->GetRotation();
    if (bookRot.x > +::ToRadian(360.0f)) bookRot.x += -ToRadian(360.0f);
    if (bookRot.x < -::ToRadian(360.0f)) bookRot.x += +ToRadian(360.0f);
    if (bookRot.y > +::ToRadian(360.0f)) bookRot.y += -ToRadian(360.0f);
    if (bookRot.y < -::ToRadian(360.0f)) bookRot.y += +ToRadian(360.0f);
    if (bookRot.z > +::ToRadian(360.0f)) bookRot.z += -ToRadian(360.0f);
    if (bookRot.z < -::ToRadian(360.0f)) bookRot.z += +ToRadian(360.0f);
    GetTransform()->SetRotation(bookRot);
}