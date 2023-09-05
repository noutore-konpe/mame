#include "Player.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "AbilityManager.h"

// コンストラクタ
Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // モデル生成
    {
        model = std::make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/sotai.fbx");
            //"./Resources/Model/sanaModel/mameoall.fbx");
            //"./Resources/Model/testModel/nico.fbx");
    }
}

// デストラクタ
Player::~Player()
{
}

// 初期化
void Player::Initialize()
{
    Character::Initialize();

    // 待機アニメーションに設定してる
    Character::PlayAnimation(0, true);
}

// 終了化
void Player::Finalize()
{
}

// Updateの前に呼ばれる
void Player::Begin()
{
}

// 更新処理
void Player::Update(const float& elapsedTime)
{
    Character::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    GamePad& gamePad = Input::Instance().GetGamePad();

    float aLx = gamePad.GetAxisLX();
    float aLy = gamePad.GetAxisLY();

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();

    if (aLx > 0)pos.x += elapsedTime;
    if (aLx < 0)pos.x -= elapsedTime;
    if (aLy > 0)pos.z += elapsedTime;
    if (aLy < 0)pos.z -= elapsedTime;

    GetTransform()->SetPosition(pos);
}

// Updateの後に呼ばれる
void Player::End()
{
}

// 描画処理
void Player::Render(const float& elapsedTime, const float& scale)
{
    Character::Render(elapsedTime, scale);
}

// ImGui用
void Player::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
