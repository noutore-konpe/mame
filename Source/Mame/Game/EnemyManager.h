#pragma once

#include "BaseEnemyAI.h"
#include <vector>
#include <set>

class EnemyManager
{
private:
    EnemyManager()  {}
    ~EnemyManager() { Clear(); }

public:
    // �B��̃C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Initialize();                      // ������
    void Finalize();
    void Update(const float elapsedTime);  // �X�V����
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr);  // �`�揈��
    void DrawDebug();                       // �f�o�b�O�`��

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

private:
    std::vector<BaseEnemyAI*> enemies_ = {};
    std::set<BaseEnemyAI*>    removes_ = {};

};

