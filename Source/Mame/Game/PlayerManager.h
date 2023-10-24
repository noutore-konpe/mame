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

    void Initialize();                                                  // 初期化
    void Finalize();                                                    // 終了化
    void Begin();                                                       // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);                              // 更新処理
    void End();                                                         // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);          // 描画処理
    void DrawDebug();                                                   // デバッグ描画

    void SkillImagesRender();
    void SkillImagesBloomRender();

    void CollisionPlayerVsEnemy();

    bool AttackCollisionPlayerToEnemy(std::vector<Enemy*>& hitEnemies, DirectX::XMFLOAT3& hitPos);

    const float GetLifeTime() const { return lifeTime; }
    void SetLifeTime(const float time) { lifeTime = time; }

    const int GetLevel() const { return level; }
    void SetLevel(const int lv) { level = lv; }

public: // 取得・設定 関連
    std::unique_ptr<Player>& GetPlayer() { return player; }

    std::vector<BaseSkill*>& GetSkillArray() { return skillArray; }

    PlayerSkill::Drain* GetDrainSkill() { return drainSkill.get(); }
    
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

    float lifeTime = 10;
    int level = 0;
};

