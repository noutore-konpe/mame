#include "Player.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"

#include "AbilityManager.h"
#include "PlayerState.h"

// コンストラクタ
Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // モデル生成
    {
        model = std::make_unique<Model>(graphics.GetDevice(),
            //"./Resources/Model/testModel/character.fbx");
            //"./Resources/Model/sotai.fbx");
            "./Resources/Model/P_Chara.fbx");
            //"./Resources/Model/player.fbx");
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

    Camera::Instance().SetTraget(GetTransform());

    //stateMachine.RegisterState(new PlayerState::NormalState());
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

    MoveUpdate(elapsedTime);

    CameraControllerUpdate(elapsedTime);
}

// Updateの後に呼ばれる
void Player::End()
{
}

void Player::MoveUpdate(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float aLx = gamePad.GetAxisLX();
    float aLy = gamePad.GetAxisLY();

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();

    auto cTransform = Camera::Instance().GetTransform();

    
    if (aLx || aLy) 
    {
        auto forward = cTransform->CalcForward();
        auto right = cTransform->CalcRight();
        forward.x *= aLy;
        forward.z *= aLy;
        right.x *= aLx;
        right.z *= aLx;

        DirectX::XMFLOAT2 moveVec{
            right.x + forward.x,
            right.z + forward.z
        };
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec.x /= length;
        moveVec.y /= length;

        float speed = moveSpeed * elapsedTime;
        pos.x += speed * moveVec.x;
        pos.z += speed * moveVec.y;
    }

    GetTransform()->SetPosition(pos);

    
}

void Player::CameraControllerUpdate(float elapsedTime)
{
    //カメラ挙動（今は回転のみ）
    auto* cTransform = Camera::Instance().GetTransform();
    
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    if (ax)
    {
        cTransform->AddRotationY(cameraRotSpeed * ax * elapsedTime);
    }

    //cTransform->SetRotationX(DirectX::XMConvertToRadians(15.0f));
}

// 描画処理
void Player::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader);
}

// ImGui用
void Player::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        if (ImGui::TreeNode("Camera"))
        {
            ImGui::SliderFloat("RotSpeed", &cameraRotSpeed, 0.01f, 5.0f);

            ImGui::TreePop();
        }

        ImGui::DragFloat("Move Speed", &moveSpeed, 0.05f, 0.1f);

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
