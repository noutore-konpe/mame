#pragma once

#include <vector>
#include <set>

#include "Enemy.h"
#include "ProjectileManager.h"

class EnemyManager
{
private:
    EnemyManager()  {}
    ~EnemyManager() {}

public:
    // �B��̃C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
    void Begin();
    void Update(const float elapsedTime);
    void End();
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr);
    void DrawDebug();

    void RenderShadow(const float scale, ID3D11PixelShader* psShader = nullptr);

    // �e�ۂƃv���C���[�̏Փˏ���
    void CollisionProjectileVsPlayer();

    // �G���m�̏Փˏ���
    void CollisionEnemyVsEnemy(const float elapsedTime);

    // �G�ƃX�e�[�W�Ƃ̏Փˏ���
    void CollisionEnemyVsStage(const float elapsedTime);

    //�U��������擾����֐�
    static bool AttackCollisionPlayerToEnemy(Enemy* my, DirectX::XMFLOAT3& hitPos);

    //AC��Active Check �R���C�_�[��isActive��true�ɂȂ��Ă��镨�̂ݔ��肷��֐�
    static bool ACAttackCollisionPlayerToEnemy(Enemy* my);

    // �L���������Z�b�g����
    void ResetKillNum()
    {
        enemy1KillNum = 0;
        enemy2KillNum = 0;
        enemy3KillNum = 0;
        enemyGolemKillNum = 0;
    }

public:
    // �G�l�~�[�o�^
    void Register(Enemy* enemy);

    // �G�l�~�[�폜
    void Remove(Enemy* enemy);

    // �G�l�~�[�S�폜
    void Clear();

public:
    // �G�l�~�[���擾
    const size_t GetEnemyCount() const { return enemies_.size(); }
    void GetSpecifyEnemy(const Enemy::TYPE& type, std::vector<Enemy*>& e) const
    {
        for (Enemy* enemy : enemies_)
        {
            if (enemy->GetType() == type)
            {
                e.emplace_back(enemy);
            }
        }
    }

    std::vector<Enemy*>& GetEnemies() { return enemies_; }

    // �G�l�~�[�擾
    Enemy* GetEnemy(const size_t index) { return enemies_.at(index); }
    ProjectileManager* GetProjectileManager() { return &projectileManager_; }

    const bool GetIsRunningCRAAction() const { return isRunningCRAAction_; }
    void SetIsRunningCRAAction(const bool isRunningCRAAction) { isRunningCRAAction_ = isRunningCRAAction; }

    const float GetCRAActionCoolTimer() const { return craActionCoolTimer_; }
    void SetCRAActionCoolTimer(const float craActionCoolTime) { craActionCoolTimer_ = craActionCoolTime; }

    int GetEnemy1KillNum() { return enemy1KillNum; }
    int GetEnemy2KillNum() { return enemy2KillNum; }
    int GetEnemy3KillNum() { return enemy3KillNum; }
    int GetEnemyGolemKillNum() { return enemyGolemKillNum; }
public:
    static constexpr float CRA_ACTION_COOL_TIME_ = 0.5f; // �ߐڍU���N�[���^�C��

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
    std::set<Enemy*>    generates_ = {};

    ProjectileManager projectileManager_ = {};

    float   craActionCoolTimer_ = 0.0f;     // �ߐڍU���s���N�[���^�C�}�[
    bool    isRunningCRAAction_ = false;    // �N�����ߐڍU���s�������s����

    int enemy1KillNum = 0;
    int enemy2KillNum = 0;
    int enemy3KillNum = 0;
    int enemyGolemKillNum = 0;
};

