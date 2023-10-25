#pragma once

#include "Player.h"

class PlayerManager
{
private:
    PlayerManager();
    ~PlayerManager() {}

public:
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    void Initialize();                                                  // ������
    void Finalize();                                                    // �I����
    void Begin();                                                       // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);                              // �X�V����
    void End();                                                         // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);          // �`�揈��
    void DrawDebug();                                                   // �f�o�b�O�`��

    void SkillImagesRender();
    void SkillImagesBloomRender();

    void CollisionPlayerVsEnemy();

    bool AttackCollisionPlayerToEnemy(std::vector<Enemy*>& hitEnemies, DirectX::XMFLOAT3& hitPos);

    //��炢����̗������U������Ƃ��Ĕ��肷��֐�
    bool HitCollisionPlayerToEnemy(std::vector<Enemy*>& hitEnemies, DirectX::XMFLOAT3& hitPos);

    const float GetLifeTime() const { return lifeTime; }
    void SetLifeTime(const float time) { lifeTime = time; }

    const int GetLevel() const { return level; }
    void SetLevel(const int lv) { level = lv; }

public: // �擾�E�ݒ� �֘A
    std::unique_ptr<Player>& GetPlayer() { return player; }

    std::vector<BaseSkill*>& GetSkillArray() { return skillArray; }

    PlayerSkill::Drain* GetDrainSkill() { return drainSkill.get(); }
    PlayerSkill::CanCounterAttack* GetCounterSkill() { return counterSkill.get(); }
    PlayerSkill::CanTripleAttack* GetTripleAttackSkill() { return tripleAttackSkill.get(); }
    PlayerSkill::ChangeHomingSkill* GetHomingSkill() { return homingSkill.get(); }
    PlayerSkill::PoisonSkill* GetPoisonSkill() { return poisonSkill.get(); }
    PlayerSkill::BookIncrease* GetBookIncreaseSkill() { return bookIncreaseSkill.get(); }
    PlayerSkill::BulletRateUp* GetBulletRateUpSkill() { return bulletRateUpSkill.get(); }
    PlayerSkill::BulletSizeUp* GetBulletSizeUpSkill() { return bulletSizeUpSkill.get(); }
    PlayerSkill::RevengeSkill* GetRevengeSkill() { return revengeSkill.get(); }
    
    //PlayerSkill::* GetDrainSkill() { return drainSkill.get(); }
    
    void SetTamaType(int type) { tamaType = type; }
    int GetTamaType() { return tamaType; }

    void SetFog(bool f) 
    {
        isFog = f;
    }
    bool GetFog() { return isFog; }
    void AddFogTimer(const float& elapsedTime) { fogTimer += elapsedTime; }
    void SetFogTimer(const float& t) { fogTimer = t; }
    float GetFogTimer() { return fogTimer; }

    bool isChange = false;

    bool isChangeBGM = false;

private:
    std::unique_ptr<Player> player = nullptr;

    std::vector<BaseSkill*> skillArray;
    std::unique_ptr<PlayerSkill::Drain> drainSkill;
    std::unique_ptr<PlayerSkill::MoveSpeedUp> moveSpeedUpSkill;
    std::unique_ptr<PlayerSkill::AttackPowerUp> attackPowerUpSkill;
    std::unique_ptr<PlayerSkill::AttackSpeedUp> attackSpeedUpSkill;
    std::unique_ptr<PlayerSkill::BookIncrease> bookIncreaseSkill;
    std::unique_ptr<PlayerSkill::MaxHitPointUp> maxHitPointUpSkill;
    std::unique_ptr<PlayerSkill::DefenseUp> defenseUpSkill;
    std::unique_ptr<PlayerSkill::BlackHoleSkill> blackHoleSkill;
    std::unique_ptr<PlayerSkill::CanCounterAttack> counterSkill;
    std::unique_ptr<PlayerSkill::CanTripleAttack> tripleAttackSkill;
    std::unique_ptr<PlayerSkill::ChangeHomingSkill> homingSkill;
    std::unique_ptr<PlayerSkill::PoisonSkill> poisonSkill;
    std::unique_ptr<PlayerSkill::BulletRateUp> bulletRateUpSkill;
    std::unique_ptr<PlayerSkill::BulletSizeUp> bulletSizeUpSkill;
    std::unique_ptr<PlayerSkill::RevengeSkill> revengeSkill;

    float lifeTime = 10;
    int level = 0;

    int tamaType = 0;

    bool isFog = false;
    float fogTimer = 0.0f;

public:
    enum class TYPE
    {
        Normal,
        Doku,
        Homing,
        All
    };
};

