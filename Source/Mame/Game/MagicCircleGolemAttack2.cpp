#include "MagicCircleGolemAttack2.h"

#include "../Graphics/Graphics.h"



// コンストラクタ
MagicCircleGolemAttack2::MagicCircleGolemAttack2()
{
    Graphics& graphics = Graphics::Instance();

    for(int i=0;i<static_cast<UINT>(MAGIC_CIRCLE::Max);++i)
    {
        baseMagicCircle[i] = std::make_unique<MagicCircle>();
    }

    // ステートマシン
    {

    }
}

// デストラクタ
MagicCircleGolemAttack2::~MagicCircleGolemAttack2()
{
}

// 
void MagicCircleGolemAttack2::Initialize()
{
}

// 終了化
void MagicCircleGolemAttack2::Finalize()
{
}

void MagicCircleGolemAttack2::Update(const float& elapsedTime)
{
}

void MagicCircleGolemAttack2::Render(const DirectX::XMFLOAT4 color)
{
}

void MagicCircleGolemAttack2::DrawDebug()
{
}
