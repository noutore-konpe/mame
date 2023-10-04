#pragma once

#include "BaseEnemyAI.h"
#include <vector>
#include <set>

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
    void Register(BaseEnemyAI* enemy);

    // �G�l�~�[�폜
    void Remove(BaseEnemyAI* enemy);

    // �G�l�~�[�S�폜
    void Clear();

public:
    // �G�l�~�[���擾
    const size_t GetEnemyCount() const { return enemies_.size(); }

    // �G�l�~�[�擾
    BaseEnemyAI* GetEnemy(const size_t index) { return enemies_.at(index); }

    const bool GetIsRunningCRAAction() const { return isRunningCRAAction_; }
    void SetIsRunningCRAAction(const bool isRunningCRAAction) { isRunningCRAAction_ = isRunningCRAAction; }

    const float GetCRAActionCoolTimer() const { return craActionCoolTimer_; }
    void SetCRAActionCoolTimer(const float craActionCoolTime) { craActionCoolTimer_ = craActionCoolTime; }

private:
    std::vector<BaseEnemyAI*> enemies_ = {};
    std::set<BaseEnemyAI*>    removes_ = {};

protected:
    bool    isRunningCRAAction_ = false;    // �N�����ߐڍU���s�������s����
    float   craActionCoolTimer_ = 0.0f;     // �ߐڍU���s���N�[���^�C�}�[

};
