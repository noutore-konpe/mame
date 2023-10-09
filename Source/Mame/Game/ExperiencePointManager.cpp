#include "ExperiencePointManager.h"
#include "PlayerManager.h"
#include "Collision.h"

void ExperiencePointManager::Initialize()
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->Initialize();
    }
}


void ExperiencePointManager::Finalize()
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->Finalize();
    }
    Clear();
}


void ExperiencePointManager::Begin()
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->Begin();
    }
}


void ExperiencePointManager::Update(const float elapsedTime)
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->Update(elapsedTime);
    }

    // ”jŠüˆ—
    for (ExperiencePoint* exp : exps_)
    {
        auto it = std::find(exps_.begin(), exps_.end(), exp);
        if (it != exps_.end()) { exps_.erase(it); }

        ::SafeDeletePtr(exp);
    }
    removes_.clear();

    CollisionExpVsPlayer(elapsedTime);
    CollisionExpVsExp(elapsedTime);
}


void ExperiencePointManager::End()
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->End();
    }
}


void ExperiencePointManager::Render(
    const float scale, ID3D11PixelShader* psShader)
{
    for (ExperiencePoint*& exp : exps_)
    {
        exp->Render(scale, psShader);
    }
}


void ExperiencePointManager::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu("Exps"))
    {
        for (ExperiencePoint*& exp : exps_)
        {
            exp->DrawDebug();
        }
        ImGui::Separator();

        ImGui::EndMenu();
    }

#endif
}


void ExperiencePointManager::CollisionExpVsPlayer(const float /*elapsedTime*/)
{
    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();

    const size_t expCount = exps_.size();
    for (size_t i = 0; i < expCount; ++i)
    {
        ExperiencePoint* exp = GetExp(i);

        const XMFLOAT3& expPos    = exp->GetTransform()->GetPosition();
        const float     expRadius = exp->GetRadius();
        const XMFLOAT3& plPos     = plManager.GetPlayer()->GetTransform()->GetPosition();
        //const float     plRadius  = plManager.GetPlayer()->GetRadius();
        const float     plRadius  = 0.5f;

        if (Collision::IntersectSphereVsSphere(
            expPos, expRadius, plPos, plRadius))
        {
            Remove(exp);
        }

    }
}


void ExperiencePointManager::CollisionExpVsExp(const float /*elapsedTime*/)
{
}


void ExperiencePointManager::Register(ExperiencePoint* exp)
{
    exps_.emplace_back(exp);
}

void ExperiencePointManager::Remove(ExperiencePoint* exp)
{
    removes_.insert(exp);
}

void ExperiencePointManager::Clear()
{
    for (ExperiencePoint*& exp : exps_)
    {
        ::SafeDeletePtr(exp);
    }
    exps_.clear();
    exps_.shrink_to_fit();
}


void ExperiencePointManager::CreateExp(
    const DirectX::XMFLOAT3& position, const int expCount,
    const float minForceXZ, const float maxForceXZ,
    const float minForceY, const float maxForceY)
{
    using DirectX::XMFLOAT3;

    for (int i = 0; i < expCount; ++i)
    {
        ExperiencePoint* exp = new ExperiencePoint();

        exp->Initialize();  // ‰Šú‰»‚µ‚Ä‚¨‚­
        exp->GetTransform()->SetPosition(position); // ¶¬ˆÊ’uÝ’è

        // ŽU‚ç‚Î‚é‘¬“x‚ðÝ’è
        const XMFLOAT3 force = {
            ::RandFloat(minForceXZ, maxForceXZ),
            ::RandFloat(minForceY, maxForceY),
            ::RandFloat(minForceXZ, maxForceXZ),
        };
        exp->SetVelocity(force);

        this->Register(exp);
    }
}
