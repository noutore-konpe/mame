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
    void CollisionExpVsPlayer(const float elapsedTime); // �v���C���[�Ƃ̏Փˏ���
    void CollisionExpVsExp(const float elapsedTime);    // �o���l���m�̏Փˏ���
    void CollisionExpVsStage(const float elapsedTime);  // �X�e�[�W�Ƃ̏Փˏ���

public:
    void Register(ExperiencePoint* exp);    // �o�^
    void Remove(ExperiencePoint* exp);      // �폜
    void Clear();                           // �S�폜

    /// <param name="position�F��������ʒu"></param>
    /// <param name="count�F�������鐔"></param>
    /// <param name="minForce�F���x�ɉ�����ŏ��p���[(�����_��)"></param>
    /// <param name="maxForce�F���x�ɉ�����ő�p���[(�����_��)"></param>
    void CreateExp(
        const DirectX::XMFLOAT3& position,
        const int count,
        const DirectX::XMFLOAT3& minForce,
        const DirectX::XMFLOAT3& maxForce
    );
    /// <param name="position�F��������ʒu"></param>
    /// <param name="count�F�������鐔"></param>
    /// <param name="minForceXZ�FXZ���x�ɉ�����p���[(�����_��)"></param>
    /// <param name="maxForceY�FY���x�ɉ�����p���[(�����_��)"></param>
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

