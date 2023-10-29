#include "Enemy.h"

#include "../../Taki174/FunctionXMFloat3.h"
#include "EnemyManager.h"
#include "WaveManager.h"
#include "PlayerManager.h"

// �����}�[�J�[���g�p���邩
#define USE_DIRECTION_MARKER (1)

Enemy::Enemy()
{
    Graphics& graphics = Graphics::Instance();

    // �����}�[�J�[����
#if USE_DIRECTION_MARKER
    directionMarker_ = std::make_unique<Sprite>(
        graphics.GetDevice(),
        L"./Resources/Image/UI/DirectionMarker.png"
    );
#endif

}

Enemy::~Enemy()
{
    // �E�F�[�u�ŊǗ�����Ă����G�Ȃ�c��̃E�F�[�u�G�l�~�[�J�E���g���P���炷
    if (true == isWaveEnemy_)
    {
        WaveManager::Instance().ReduceRemainingEnemyCounter();
    }
}

void Enemy::Initialize()
{
    Character::Initialize();

    // �����}�[�J�[�����ݒ�
    InitializeDirectionMarker();
}

void Enemy::Update(const float& elapsedTime)
{
    Character::Update(elapsedTime);

    // �����}�[�J�[�X�V
    UpdateDirectionMarker(elapsedTime);
}

void Enemy::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader);
}

void Enemy::RenderDirectionMarker()
{
    if (nullptr == directionMarker_) { return; }

    Camera& camera = Camera::Instance();

    // ���b�N�I�����͕`�悵�Ȃ��i���j
    if (true == camera.GetActiveLockOn()) { return; }

    directionMarker_->Render();
}

void Enemy::InitializeDirectionMarker()
{
    if (nullptr == directionMarker_) { return; }

    using DirectX::XMFLOAT4;
    Sprite::SpriteTransform* directionMarkerT = directionMarker_->GetSpriteTransform();

    // �ԐF�ɐݒ�
    const XMFLOAT4 color = { 1.0f, 0.25f, 0.0f, 1.0f };
    directionMarkerT->SetColor(color);

    // �T�C�Y����
    directionMarkerT->SetSizeX(directionMarkerT->GetSizeX() * 0.1f);
    directionMarkerT->SetSizeY(directionMarkerT->GetSizeY() * 0.1f);

}

void Enemy::UpdateDirectionMarker(const float elapsedTime)
{
    if (nullptr == directionMarker_) { return; }

    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT3;

    Player* player = PlayerManager::Instance().GetPlayer().get();
    Camera& camera = Camera::Instance();
    Sprite::SpriteTransform* directionMarkerT = directionMarker_->GetSpriteTransform();

    const XMFLOAT3& plPos = player->GetTransform()->GetPosition();
    const XMFLOAT3& enemyPos = this->GetTransform()->GetPosition();
    const XMFLOAT2  screenPlayerPos = Sprite::ConvertToScreenPos(plPos);
    const float     vecX = enemyPos.x - plPos.x;
    const float     vecZ = enemyPos.z - plPos.z;
    const float     lengthXZ_Sq = (vecX * vecX + vecZ * vecZ);

    const float cameraRotationY = camera.GetTransform()->GetRotationY();
    //float angleRadian = {
    //    ::atan2f(vecX, vecZ) +
    //    ((true == camera.GetActiveLockOn())
    //     ? /*�v���P*/(-cameraRotationY)
    //     : (-cameraRotationY))
    //};
    float angleRadian = ::atan2f(vecX, vecZ) + (-cameraRotationY);
    if      (angleRadian < 0.0f)   angleRadian += ::ToRadian(360.0f);
    else if (angleRadian > 360.0f) angleRadian += ::ToRadian(-360.0f);

    // �ʒu�X�V
    {
        static constexpr float RADIUS = 120.0f;
        //const XMFLOAT2 centerPos =
        //    (true == camera.GetActiveLockOn())
        //    ? Sprite::ConvertToScreenPos(camera.GetLockOnTarget()->GetPosition())
        //    : screenPlayerPos;
        const XMFLOAT2 centerPos = screenPlayerPos;
        directionMarkerT->SetPosX(screenPlayerPos.x + ::sinf(angleRadian) * RADIUS);
        directionMarkerT->SetPosY(screenPlayerPos.y + (-::cosf(angleRadian)) * RADIUS);

        // ���S�Ɉʒu����
        directionMarkerT->SetSpriteCenterPos(directionMarkerT->GetPos());
    }

    // �p�x�X�V
    {
        const float angleDegree = DirectX::XMConvertToDegrees(angleRadian);
        directionMarkerT->SetAngle(angleDegree);
    }

    //�@�s�����x�X�V
    {
        // �}�[�J�[��\�����鋗��
        static constexpr float DISP_LENGTH = 15.0f;
        static constexpr float DISP_LENGTH_SQ = DISP_LENGTH * DISP_LENGTH;

        // �v���C���[�ƓG�Ƃ̋����ŕs�����x�𒲐߂���
        const float colorA = {
            (lengthXZ_Sq <= DISP_LENGTH_SQ)
            ? 1.0f - (lengthXZ_Sq / DISP_LENGTH_SQ)
            : 0.0f
        };

        directionMarkerT->SetColorA(colorA);
    }

}

// ������΂����̕ۑ�
void Enemy::SaveBlowOffInfo(
    const DirectX::XMFLOAT3& blowOffVec,
    const BLOW_OFF_FORCE_LEVEL& blowOffForceLevel)
{
    blowOffVec_        = blowOffVec;
    blowOffForceLevel_ = blowOffForceLevel;
}

void Enemy::Destroy()
{
    // ����������
    EnemyManager::Instance().Remove(this);
}

// ��������q�m�[�h�����݂��邩��������
const bool Enemy::IsExistChildNode(const std::string& findNodeName)
{
    std::vector<NodeBase*>* Nodes = behaviorTree_->GetRoot()->GetChildren();

    for (size_t i = 0; i < Nodes->size(); ++i)
    {
        // �T���Ă���m�[�h�Ɩ��O����v������true��Ԃ�
        if (findNodeName == Nodes->at(i)->GetName())
        {
            return true;
        }
    }

    return false;
}

void Enemy::AttackCollisionOnPlayer(const float damage)
{
    DirectX::XMFLOAT3 pos;
    if (EnemyManager::AttackCollisionPlayerToEnemy(this,pos))
    {
        PlayerManager::Instance().GetPlayer()->ApplyDamage(damage,pos,Player::HitReaction::SOFT,this,0.2f);
    }
}

#undef USE_DIRECTION_MARKER