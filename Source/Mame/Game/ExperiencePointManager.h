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
    void CollisionExpVsPlayer(const float elapsedTime);
    void CollisionExpVsExp(const float elapsedTime);

public:
    void Register(ExperiencePoint* exp);    // 登録
    void Remove(ExperiencePoint* exp);      // 削除
    void Clear();                           // 全削除

    /// <param name="position：生成する位置"></param>
    /// <param name="count：生成する数"></param>
    /// <param name="minForceXZ：XZ速度に加える最小パワー(ランダム)"></param>
    /// <param name="maxForceXZ：XZ速度に加える最大パワー(ランダム)"></param>
    /// <param name="minForceY：Y速度に加える最小パワー(ランダム)"></param>
    /// <param name="maxForceY：Y速度に加える最大パワー(ランダム)"></param>
    void CreateExp(
        const DirectX::XMFLOAT3& position,
        const int count,
        const float minForceXZ = -2.0f,
        const float maxForceXZ = +2.0f,
        const float minForceY  = 0.0f,
        const float maxForceY  = +2.0f
    );

public:
    const size_t GetExpCount() const { return exps_.size(); }
    ExperiencePoint* GetExp(const size_t index) { return exps_.at(index); }

private:
    std::vector<ExperiencePoint*> exps_     = {};
    std::set<ExperiencePoint*>    removes_  = {};

};

