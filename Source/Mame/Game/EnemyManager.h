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

    void CollisionEnemyVsEnemy(const float elapsedTime);

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

    std::vector<Enemy*>& GetEnemies() { return enemies_; }

    // �G�l�~�[�擾
    Enemy* GetEnemy(const size_t index) { return enemies_.at(index); }
    ProjectileManager* GetProjectileManager() { return &projectileManager_; }

    const bool GetIsRunningCRAAction() const { return isRunningCRAAction_; }
    void SetIsRunningCRAAction(const bool isRunningCRAAction) { isRunningCRAAction_ = isRunningCRAAction; }

    const float GetCRAActionCoolTimer() const { return craActionCoolTimer_; }
    void SetCRAActionCoolTimer(const float craActionCoolTime) { craActionCoolTimer_ = craActionCoolTime; }

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};

    ProjectileManager projectileManager_ = {};

    float   craActionCoolTimer_ = 0.0f;     // �ߐڍU���s���N�[���^�C�}�[
    bool    isRunningCRAAction_ = false;    // �N�����ߐڍU���s�������s����

};

