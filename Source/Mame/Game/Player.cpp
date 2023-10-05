#include "Player.h"

#include "../Graphics/Graphics.h"
#include "../Other/Easing.h"
#include "../Other/MathHelper.h"

#include "AbilityManager.h"
#include "PlayerState.h"

#include <algorithm>

// コンストラクタ
Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // モデル生成
    {
        model = std::make_unique<Model>(graphics.GetDevice(),            
            //"./Resources/Model/Character/Player/sotai.fbx");
            //"./Resources/Model/Character/Player/P_Chara.fbx");
            "./Resources/Model/Character/Player/P_Motion.fbx");

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
    //Character::PlayAnimation(2, true);
    

    //カメラがプレイヤーを追いかけるよう設定
    Camera::Instance().SetTraget(GetTransform());

    stateMachine = std::make_unique<StateMachine<State<Player>>>();

    stateMachine->RegisterState(new PlayerState::NormalState(this));
    stateMachine->RegisterState(new PlayerState::JabAttackState(this));
    stateMachine->RegisterState(new PlayerState::AvoidState(this));
    stateMachine->RegisterState(new PlayerState::DieState(this));

    stateMachine->SetState(STATE::NORMAL);

    GetTransform()->SetScaleFactor(0.7f);

    //-------------------------スキルが関係するパラメータの初期化--------------------------------
    level = 1;//レベル
    curExp = 0;//経験値
    totalExp = 0;//合計経験値
    levelUpExp = 100;//レベルアップに必要な経験値
    isSelectingSkill = false;//スキルの選択演出中かどうかのフラグ

    maxSpeed = 4.2f;
    maxDodgeSpeed = 6.0f;
    baseAttackPower = 10.0f;
    attackSpeed = 1.0f;

    deceleration = 7.0f;
    acceleration = InitAcceleration;

    //初回時のみスキルの生成と配列挿入をする
    if (drainSkill == nullptr)
    {
        //ドレイン
        drainSkill = std::make_unique<PlayerSkill::Drain>(this);
        skillArray.emplace_back(drainSkill.get());
        //移動速度アップ
        moveSpeedUpSkill = std::make_unique<PlayerSkill::MoveSpeedUp>(this);
        skillArray.emplace_back(moveSpeedUpSkill.get());
        //攻撃力アップ
        attackPowerUpSkill = std::make_unique<PlayerSkill::AttackPowerUp>(this);
        skillArray.emplace_back(attackPowerUpSkill.get());
        //攻撃速度アップ
        attackSpeedUpSkill = std::make_unique<PlayerSkill::AttackSpeedUp>(this);
        skillArray.emplace_back(attackSpeedUpSkill.get());
        //本の数増加
        bookIncreaseSkill = std::make_unique<PlayerSkill::BookIncrease>(this);
        skillArray.emplace_back(bookIncreaseSkill.get());
    }
    for (auto& skill : skillArray)
    {
        skill->Initialize();
    }
    //------------------------------------------------------------------------------------------
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

    stateMachine->Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    //MoveUpdate(elapsedTime);

    CameraControllerUpdate(elapsedTime);

    for (auto& skill : skillArray)
    {
        skill->Update(elapsedTime);
    }
}

// Updateの後に呼ばれる
void Player::End()
{
}

void Player::MoveUpdate(float elapsedTime,float ax,float ay)
{
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();

    auto cTransform = Camera::Instance().GetTransform();

#if 0
    if (aLx || aLy)
    {
        auto forward = cTransform->CalcForward();
        auto right = cTransform->CalcRight();
        forward.x *= aLy;
        forward.z *= aLy;
        right.x *= aLx;
        right.z *= aLx;

        moveVec = {right.x + forward.x,0,right.z + forward.z};
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec.x /= length;
        moveVec.z /= length;

        float speed = acceleration * elapsedTime;
        pos.x += speed * moveVec.x;
        pos.z += speed * moveVec.z;
    }
#else
    if (ax || ay)
    {
        auto forward = cTransform->CalcForward();
        auto right = cTransform->CalcRight();
        forward.x *= ay;
        forward.z *= ay;
        right.x *= ax;
        right.z *= ax;

        moveVec = { right.x + forward.x,0,right.z + forward.z };
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec.x /= length;
        moveVec.z /= length;
    }

    UpdateVelocity(elapsedTime,ax,ay);
#endif // 0
    
    //GetTransform()->SetPosition(pos);
    DirectX::XMFLOAT3 move = {
        velocity.x * elapsedTime,
        velocity.y * elapsedTime,
        velocity.z * elapsedTime
    };
    GetTransform()->AddPosition(move);

    Turn(elapsedTime,moveVec.x, moveVec.z,360.0f);
}

void Player::MoveVecUpdate(float ax,float ay)
{
    if (ax || ay)
    {
        auto cTransform = Camera::Instance().GetTransform();

        auto forward = cTransform->CalcForward();
        auto right = cTransform->CalcRight();
        forward.x *= ay;
        forward.z *= ay;
        right.x *= ax;
        right.z *= ax;

        moveVec = { right.x + forward.x,0,right.z + forward.z };
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec.x /= length;
        moveVec.z /= length;
    }
}

void Player::UpdateVelocity(float elapsedTime,float ax,float ay)
{
    //GamePad& gamePad = Input::Instance().GetGamePad();

    /*float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();*/

    float length{ sqrtf(velocity.x * velocity.x + velocity.z * velocity.z) };

    //アニメーションの重みの変更
    model->weight = length / maxSpeed;

    if (length > 0.0f)
    {
        //入力がないなら減速
        if (ax == 0 && ay == 0)
        {
            //減速力計算
            float deceleration = this->deceleration * elapsedTime;

            if ((length -= deceleration) > 0.0f)
            {
                DirectX::XMVECTOR Velocity{velocity.x, velocity.z};
                DirectX::XMVECTOR vecNormal{DirectX::XMVector2Normalize(Velocity)};
                Velocity = DirectX::XMVectorScale(vecNormal, length);

                velocity.x = DirectX::XMVectorGetX(Velocity);
                velocity.z = DirectX::XMVectorGetY(Velocity);

                //減速処理をした場合加速処理は飛ばす
                return;
            }
            else
            {
                velocity.x = 0.0f;
                velocity.z = 0.0f;

                return;
            }
        }
    }

    //入力がないなら加速しない
    if (ax == 0 && ay == 0)return;
    //移動ベクトルがゼロベクトル出ないなら加速する
    float moveVecLength = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
    if (moveVecLength > 0.0f)
    {
        float acceleration;//加速力

        ////ダッシュ
        //if (InputDash())
        //{
        //    acceleration = this->acceleration * elapsedTime;
        //    maxSpeed = maxDashSpeed;
        //}
        //else
        //{
        //    acceleration = this->acceleration * elapsedTime;
        //    maxSpeed = this->maxSpeed;
        //}

        acceleration = this->acceleration * elapsedTime;

        //移動ベクトルによる加速処理
        velocity.x += moveVec.x * acceleration;
        velocity.z += moveVec.z * acceleration;

        //最大速度制限
        float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
        if (length > maxSpeed)
        {
            DirectX::XMVECTOR vec = { moveVec.x,moveVec.z };
            vec = DirectX::XMVector2Normalize(vec);
            DirectX::XMVECTOR velo = DirectX::XMVectorScale(vec, maxSpeed);
            velocity.x = DirectX::XMVectorGetX(velo);
            velocity.z = DirectX::XMVectorGetY(velo);

            length = maxSpeed;
        }
    }
}

void Player::AvoidUpdate(float elapsedTime)
{
    auto front = GetTransform()->CalcForward();

    float acceleration = this->acceleration * elapsedTime;

    //移動ベクトルによる加速処理
    velocity.x += front.x * acceleration;
    velocity.z += front.z * acceleration;

    //最大速度制限
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > maxDodgeSpeed)
    {
        DirectX::XMVECTOR vec = { moveVec.x,moveVec.z };
        vec = DirectX::XMVector2Normalize(vec);
        DirectX::XMVECTOR velo = DirectX::XMVectorScale(vec, maxDodgeSpeed);
        velocity.x = DirectX::XMVectorGetX(velo);
        velocity.z = DirectX::XMVectorGetY(velo);

        length = maxDodgeSpeed;
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    MoveVecUpdate(ax,ay);
    Turn(elapsedTime, moveVec.x, moveVec.z, 360.0f);

    DirectX::XMFLOAT3 velo = {
        velocity.x * elapsedTime,
        velocity.y * elapsedTime,
        velocity.z * elapsedTime
    };
    GetTransform()->AddPosition(velo);
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
    if (ay)
    {
        cTransform->AddRotationX(cameraRotSpeed * ay * elapsedTime);
        float rotX = cTransform->GetRotation().x;
        rotX = std::clamp(rotX,-0.35f,0.8f);
        cTransform->SetRotationX(rotX);
    }

    //cTransform->SetRotationX(DirectX::XMConvertToRadians(15.0f));

    Camera::Instance().ScreenVibrationUpdate(elapsedTime);
}

// 描画処理
void Player::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader);

    /*for (auto& skill : skillArray)
    {
        skill->Render();
    }*/
}

void Player::SkillImagesRender()
{
    int i = 0;
    for (auto& skill : skillArray)
    {
        if (skill->GetOverlapNum() == 0)continue;
        //skill->SetIconPos();
        skill->Render();
        i++;
    }
}

// ImGui用
void Player::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        stateMachine->DrawDebug();

        if (ImGui::TreeNode("Camera"))
        {
            ImGui::SliderFloat("RotSpeed", &cameraRotSpeed, 0.01f, 5.0f);

            ImGui::TreePop();
        }

        if(ImGui::TreeNode("Movement"))
        {
            ImGui::SliderFloat("MaxMoveSpeed", &maxSpeed, 0.1f, 10.0f);

            ImGui::TreePop();
        }

        ImGui::Begin("Skill");

        for (auto& skill : skillArray)
        {
            skill->DrawDebug();
        }

        ImGui::End();

        if (ImGui::Button("Initialize"))
        {
            Initialize();
        }

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

void Player::AttackSteppingUpdate(float elapsedTime)
{
    //タイマーのリセットはステートマシンの方で行う
    if (steppingTimer > steppingTime)return;

    //速度算出
    float maxSpeed = Easing::OutQuart(steppingTimer,steppingTime,steppingSpeed,0.0f);
    auto front = GetTransform()->CalcForward();
    velocity = front * maxSpeed;

    //移動処理
    DirectX::XMFLOAT3 move = velocity * elapsedTime;
    GetTransform()->AddPosition(move);

    //回転処理
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    MoveVecUpdate(ax,ay);
    Turn(elapsedTime, moveVec.x, moveVec.z, 360.0f);

    steppingTimer += elapsedTime;
}

void Player::LevelUpdate()
{
    if(curExp > levelUpExp)
    {
        level++;
        curExp -= levelUpExp;

        //レベルが上がると能力を取得出来る
        isSelectingSkill = true;
    }
}
