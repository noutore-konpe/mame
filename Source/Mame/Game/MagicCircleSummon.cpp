#include "MagicCircleSummon.h"

#include "../Graphics/Graphics.h"

#include "../Other/Easing.h"

// コンストラクタ
MagicCircleSummon::MagicCircleSummon()
{
    Graphics& graphics = Graphics::Instance();

    magicCircle[static_cast<UINT>(MAGIC_CIRCLE::Base)] = std::make_unique<MagicCircle>();
    magicCircle[static_cast<UINT>(MAGIC_CIRCLE::Move1)] =
        std::make_unique<MagicCircle>("./Resources/Model/Item/MagicCircle/MagicCircle1.fbx",
            L"./Resources/Model/Item/MagicCircle/planeEmissive1.png");
    magicCircle[static_cast<UINT>(MAGIC_CIRCLE::Move2)] =
        std::make_unique<MagicCircle>("./Resources/Model/Item/MagicCircle/MagicCircle1.fbx",
            L"./Resources/Model/Item/MagicCircle/planeEmissive1.png");
}

MagicCircleSummon::~MagicCircleSummon()
{
}

void MagicCircleSummon::Initialize()
{
    for (int i = 0; i < 3; ++i)
    {
        magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        magicCircle[i]->SetEmissiveColor(DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
    }

    state = static_cast<UINT>(STATE::AddScale);
    scaleTimer = 0.0f;    
}

void MagicCircleSummon::Finalize()
{
}

void MagicCircleSummon::Update(const float& elapsedTime)
{
    switch (state)
    {
    case static_cast<UINT>(STATE::AddScale):
    {
        float maxTime = 1.0f;
        float scale = 0.0f;
        if (scaleTimer <= maxTime)
        {
            scale = Easing::InSine(scaleTimer, 1.0f, 1.0f, 0.0f);

            for (int i = 0; i < 3; ++i)
            {
                magicCircle[i]->GetTransform()->AddRotationY(elapsedTime);
            }

            // タイマー加算
            scaleTimer += elapsedTime;
        }
        else
        {   // 次のステートへ
            scaleTimer = 0.0f;
            state = static_cast<UINT>(STATE::MoveUp);
            break;
        }

        // スケール設定
        for (int i = 0; i < 3; ++i)
        {
            magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
        }

        break;
    }
    case static_cast<UINT>(STATE::MoveUp):
    //{
    //    float scale = 1.0f;

    //    for (int i = 0; i < 3; ++i)
    //    {
    //        magicCircle[i]->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
    //    }
    //}

        for (int i = 0; i < 3; ++i)
        {
            magicCircle[i]->GetTransform()->AddRotationY(elapsedTime);
        }

        break;
    }
}

void MagicCircleSummon::Render()
{
    for (int i = 0; i < 3; ++i)
    {
        magicCircle[i]->Render(0.01f);
    }
}

void MagicCircleSummon::DrawDebug()
{
}
