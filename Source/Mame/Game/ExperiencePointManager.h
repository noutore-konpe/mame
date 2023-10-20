#pragma once

#include <vector>
#include <set>
#include "ExperiencePoint.h"

class ExperiencePointManager
{
private:
    ExperiencePointManager() {}
    ~ExperiencePointManager() {}

public:
    static ExperiencePointManager& Instance()
    {
        static ExperiencePointManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
    void Begin();
    void Update(const float elapsedTime);
    void End();
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr);
    void DrawDebug();

public:
    void CollisionExpVsPlayer(const float elapsedTime); // プレイヤーとの衝突処理
    void CollisionExpVsExp(const float elapsedTime);    // 経験値同士の衝突処理
    void CollisionExpVsStage(const float elapsedTime);  // ステージとの衝突処理

public:
    void Register(ExperiencePoint* exp);    // 登録
    void Remove(ExperiencePoint* exp);      // 削除
    void Clear();                           // 全削除

    /// <param name="position：生成する位置"></param>
    /// <param name="count：生成する数"></param>
    /// <param name="minForce：速度に加える最小パワー(ランダム)"></param>
    /// <param name="maxForce：速度に加える最大パワー(ランダム)"></param>
    void CreateExp(
        const DirectX::XMFLOAT3& position,
        const int count,
        const DirectX::XMFLOAT3& minForce,
        const DirectX::XMFLOAT3& maxForce
    );
    /// <param name="position：生成する位置"></param>
    /// <param name="count：生成する数"></param>
    /// <param name="minForceXZ：XZ速度に加えるパワー(ランダム)"></param>
    /// <param name="maxForceY：Y速度に加えるパワー(ランダム)"></param>
    void CreateExp(
        const DirectX::XMFLOAT3& position,
        const int count,
        const float forceXZ = 2.5f,
        const float forceY  = 6.0f
    )
    {
        CreateExp(
            position, count,
            DirectX::XMFLOAT3(-forceXZ, +forceY * 0.25f, -forceXZ),
            DirectX::XMFLOAT3(+forceXZ, +forceY,         +forceXZ)
        );
    }

public:
    const size_t GetExpCount() const { return exps_.size(); }
    ExperiencePoint* GetExp(const size_t index) { return exps_.at(index); }

private:
    std::vector<ExperiencePoint*> exps_     = {};
    std::set<ExperiencePoint*>    removes_  = {};

};

