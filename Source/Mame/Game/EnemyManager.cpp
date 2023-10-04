#include "EnemyManager.h"

#include "../../Taki174/Common.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../Game/Collision.h"

void EnemyManager::Initialize()
{
    // CRA : 0.Initialize : ������
    isRunningCRAAction_ = false;
    craActionCoolTimer_ = 0.0f;

    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Initialize();
    }
}

void EnemyManager::Finalize()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Finalize();
    }
    Clear();
}

void EnemyManager::Begin()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Begin();
    }
}

// �X�V����
void EnemyManager::Update(const float elapsedTime)
{
    // CRA : 1.Update : �ߐڍU���s���N�[���^�C�}�[����
    craActionCoolTimer_ = (std::max)(0.0f, craActionCoolTimer_ - elapsedTime);

    // �X�V
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Update(elapsedTime);
    }

    // �j������
    {
        for (BaseEnemyAI* enemy : removes_)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            std::vector<BaseEnemyAI*>::iterator it = std::find(enemies_.begin(), enemies_.end(), enemy);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != enemies_.end())
            {
                enemies_.erase(it);
            }

            // �G�̔j������
            SafeDeletePtr(enemy);
        }
        // �j�����X�g���N���A
        removes_.clear();
    }

    // �G�l�~�[���m�̏Փ˔��菈��
    CollisionEnemyVsEnemy(elapsedTime);

}

void EnemyManager::End()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->End();
    }
}


// �`�揈��
void EnemyManager::Render(const float scale, ID3D11PixelShader* psShader)
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Render(scale, psShader);
    }
}


// �G�l�~�[�o�^
void EnemyManager::Register(BaseEnemyAI* enemy)
{
    enemies_.emplace_back(enemy);
}


// �G�l�~�[�폜
void EnemyManager::Remove(BaseEnemyAI* enemy)
{
    // �j�����X�g�ɒǉ�
    removes_.insert(enemy);
}


// �G�l�~�[�S�폜
void EnemyManager::Clear()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        SafeDeletePtr(enemy);
    }
    enemies_.clear();
    enemies_.shrink_to_fit();    // vector�̗]���ȃ����������
}


// �f�o�b�O
void EnemyManager::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("Enemies"))
    {
        for (BaseEnemyAI*& enemy : enemies_)
        {
            enemy->DrawDebug();
        }

        ImGui::EndMenu();

        for (BaseEnemyAI*& enemy : enemies_)
        {
            std::string str = (enemy->GetActiveNode() != nullptr)
                ? enemy->GetActiveNode()->GetName()
                : u8"�Ȃ�";
            ImGui::Text(u8"Behavior�F%s", str.c_str());
        }
    }
    ImGui::Separator();

#endif
}


void EnemyManager::CollisionEnemyVsEnemy(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    EnemyManager& enemyManager = EnemyManager::Instance();

    const size_t enemyCount = enemyManager.GetEnemyCount();
    for (size_t a = 0; a < enemyCount; ++a)
    {
        BaseEnemyAI* enemyA = enemyManager.GetEnemy(a);

        // a�ȍ~�̓G�Ɣ�����s���ia�ȑO�͂��łɔ���ς݂̂��߁j
        for (size_t b = a + 1; b < enemyCount; ++b)
        {
            BaseEnemyAI* enemyB = enemyManager.GetEnemy(b);

            const XMFLOAT3  positionA   = enemyA->GetPosition();
            const XMFLOAT3  positionB   = enemyB->GetPosition();
            const float     radiusA     = enemyA->GetRadius();
            const float     radiusB     = enemyB->GetRadius();

            const XMFLOAT3  vecAtoB     = positionB - positionA;
            const float     lengthSq    = ::XMFloat3LengthSq(vecAtoB);
            const float     range       = radiusA + radiusB;


            if (lengthSq > (range * range)) continue;


            const XMFLOAT3 vecAtoB_N = ::XMFloat3Normalize(vecAtoB);

            // CRA : 7.Collisition : �ߐڍU��������Ȃ����̓G�������o��
            if (enemyB->GetActiveNode() != nullptr &&
                "CloseRangeAttack" == enemyB->GetActiveNode()->GetName())
            {
                const XMFLOAT3 vecBtoA_N = vecAtoB_N * (-1.0f);
                enemyA->SetPosition(positionB + vecBtoA_N * range);
            }
            else
            {
                enemyB->SetPosition(positionA + vecAtoB_N * range);
            }

        }

    }

}
