#include "MagicCircleGolemAttack2State.h"

// DummyState
namespace MagicCircleGolemAttack2State
{
    void DummyState::Initialize()
    {
        for (int i = 0; i < static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Max); ++i)
        {
            owner->baseMagicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(0, 0, 0));
            owner->baseMagicCircle[i]->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 0));
        }
    }
}

// AppearState
namespace MagicCircleGolemAttack2State
{
    void AppearState::Initialize()
    {

    }

    void AppearState::Update(const float& elapsedTime)
    {

    }

    void AppearState::Finalize()
    {

    }
}

namespace MagicCircleGolemAttack2State
{

}