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
    void Register(ExperiencePoint* exp);    // �o�^
    void Remove(ExperiencePoint* exp);      // �폜
    void Clear();                           // �S�폜

    /// <param name="position�F��������ʒu"></param>
    /// <param name="count�F�������鐔"></param>
    /// <param name="minForceXZ�FXZ���x�ɉ�����ŏ��p���[(�����_��)"></param>
    /// <param name="maxForceXZ�FXZ���x�ɉ�����ő�p���[(�����_��)"></param>
    /// <param name="minForceY�FY���x�ɉ�����ŏ��p���[(�����_��)"></param>
    /// <param name="maxForceY�FY���x�ɉ�����ő�p���[(�����_��)"></param>
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

