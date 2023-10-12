#include "Player.h"

#include "../Graphics/Graphics.h"
#include "../Other/Easing.h"
#include "../Other/MathHelper.h"

#include "AbilityManager.h"
#include "PlayerState.h"
#include "EnemyManager.h"

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

        // pixelShader
        CreatePsFromCso(graphics.GetDevice(),
            "./Resources/Shader/playerPS.cso",
            playerPS.GetAddressOf());
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

    maxEyeSpeed = 4.2f;
    maxDodgeSpeed = 6.0f;
    baseAttackPower = 10.0f;
    attackSpeed = 1.0f;

    defense = 0.0f;

    deceleration = 7.0f;
    eyeAcceleration = InitAcceleration;

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
        //体力上限アップ
        maxHitPointUpSkill = std::make_unique<PlayerSkill::MaxHitPointUp>(this);
        skillArray.emplace_back(maxHitPointUpSkill.get());
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
void Player::Update(const float elapsedTime)
{
    //ロックオン解除、発動
    if (InputLockOn())
    {
        Camera::Instance().activeLockOn = !Camera::Instance().activeLockOn;
        if (Camera::Instance().activeLockOn)LockOnInitialize();
    }

    if (Camera::Instance().activeLockOn)
    {
        LockOnUpdate();
    }

    Character::Update(elapsedTime);

    stateMachine->Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    //MoveUpdate(elapsedTime);

    CameraControllerUpdate(elapsedTime);

    LevelUpdate();

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
    MoveVecUpdate(ax, ay);

    UpdateVelocity(elapsedTime,ax,ay);
#endif // 0

    //GetTransform()->SetPosition(pos);
    DirectX::XMFLOAT3 move = {
        eyeVelocity.x * elapsedTime,
        eyeVelocity.y * elapsedTime,
        eyeVelocity.z * elapsedTime
    };
    GetTransform()->AddPosition(move);

    Turn(elapsedTime,moveVec.x, moveVec.z,360.0f);
}

void Player::MoveVecUpdate(float ax,float ay)
{
    if (ax || ay)
    {
        //auto cTransform = Camera::Instance().GetTransform();

        auto forward = Camera::Instance().GetForward();
        auto right = Camera::Instance().GetRight();
        forward.x *= ay;
        forward.z *= ay;
        right.x *= ax;
        right.z *= ax;

        moveVec = { right.x + forward.x,0,right.z + forward.z };
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
        moveVec.x /= length;
        moveVec.z /= length;
    }
}

void Player::UpdateVelocity(float elapsedTime,float ax,float ay)
{
    //GamePad& gamePad = Input::Instance().GetGamePad();

    /*float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();*/

    float length{ sqrtf(eyeVelocity.x * eyeVelocity.x + eyeVelocity.z * eyeVelocity.z) };

    //アニメーションの重みの変更
    //model->weight = length / maxSpeed;
    model->weight = (std::min)(1.0f, length / maxEyeSpeed);


    if (length > 0.0f)
    {
        //入力がないなら減速
        if (ax == 0 && ay == 0)
        {
            //減速力計算
            float deceleration = this->deceleration * elapsedTime;

            if ((length -= deceleration) > 0.0f)
            {
                DirectX::XMVECTOR Velocity{eyeVelocity.x, eyeVelocity.z};
                DirectX::XMVECTOR vecNormal{DirectX::XMVector2Normalize(Velocity)};
                Velocity = DirectX::XMVectorScale(vecNormal, length);

                eyeVelocity.x = DirectX::XMVectorGetX(Velocity);
                eyeVelocity.z = DirectX::XMVectorGetY(Velocity);

                //減速処理をした場合加速処理は飛ばす
                return;
            }
            else
            {
                eyeVelocity.x = 0.0f;
                eyeVelocity.z = 0.0f;

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
        float eyeAcceleration;//加速力

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

        eyeAcceleration = this->eyeAcceleration * elapsedTime;

        //移動ベクトルによる加速処理
        eyeVelocity.x += moveVec.x * eyeAcceleration;
        eyeVelocity.z += moveVec.z * eyeAcceleration;

        //最大速度制限
        float length = sqrtf(eyeVelocity.x * eyeVelocity.x + eyeVelocity.z * eyeVelocity.z);
        if (length > maxEyeSpeed)
        {
            DirectX::XMVECTOR vec = { moveVec.x,moveVec.z };
            vec = DirectX::XMVector2Normalize(vec);
            DirectX::XMVECTOR velo = DirectX::XMVectorScale(vec, maxEyeSpeed);
            eyeVelocity.x = DirectX::XMVectorGetX(velo);
            eyeVelocity.z = DirectX::XMVectorGetY(velo);

            length = maxEyeSpeed;
        }
    }
}

void Player::AvoidUpdate(float elapsedTime)
{
    auto front = GetTransform()->CalcForward();

    float eyeAcceleration = this->dodgeAcceleration * elapsedTime;

    //移動ベクトルによる加速処理
    eyeVelocity.x += front.x * eyeAcceleration;
    eyeVelocity.z += front.z * eyeAcceleration;

    //最大速度制限
    float maxDodgeSpeed = this->maxDodgeSpeed + Easing::OutSine(
        static_cast<float>(model->GetCurrentKeyframeIndex()),
        static_cast<float>(model->GetCurrentKeyframeMaxIndex()/2.0f),
        2.0f, 0.0f);
    float length = sqrtf(eyeVelocity.x * eyeVelocity.x + eyeVelocity.z * eyeVelocity.z);
    if (length > maxDodgeSpeed)
    {
        DirectX::XMVECTOR vec = { moveVec.x,moveVec.z };
        vec = DirectX::XMVector2Normalize(vec);
        DirectX::XMVECTOR velo = DirectX::XMVectorScale(vec, maxDodgeSpeed);
        eyeVelocity.x = DirectX::XMVectorGetX(velo);
        eyeVelocity.z = DirectX::XMVectorGetY(velo);

        length = maxDodgeSpeed;
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    MoveVecUpdate(ax,ay);
    Turn(elapsedTime, moveVec.x, moveVec.z, 240.0f);

    DirectX::XMFLOAT3 velo = {
        eyeVelocity.x * elapsedTime,
        eyeVelocity.y * elapsedTime,
        eyeVelocity.z * elapsedTime
    };
    GetTransform()->AddPosition(velo);
}

void Player::ModelRotZUpdate(float elapsedTime)
{
    static float lastRotValue = 0;

    /*float rotSpeed = elapsedTime * 10.0f * actualRotValue - rotValue;
    if (actualRotValue - rotValue < 0.01f && actualRotValue - rotValue > -0.01f)
    {
        actualRotValue = 0;
    }
    if (actualRotValue < rotValue)
    {
        actualRotValue += rotSpeed;
    }
    else if (actualRotValue > rotValue)
    {
        actualRotValue -= rotSpeed;
    }*/

    if (rotTimer > 0.1f)
    {
        lastRotValue = GetTransform()->GetRotation().z;
        rotTimer = 0;
    }
    //float rotZ = Easing::InSine(rotTimer,0.1f,-rotValue * 5,lastRotValue);
    float rotZ = Easing::InSine(std::fabsf(rotValue), 0.1f, 0.5f, 0.0f);
    rotZ = rotValue < 0 ? rotZ : -rotZ;
    GetTransform()->SetRotationZ(rotZ);

    //GetTransform()->SetRotationZ(actualRotValue);

    rotTimer += elapsedTime;
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
        cTransform->AddRotationX(cameraRotSpeed * -ay * elapsedTime);
        float rotX = cTransform->GetRotation().x;
        rotX = std::clamp(rotX,-0.35f,0.8f);
        cTransform->SetRotationX(rotX);
    }

    //cTransform->SetRotationX(DirectX::XMConvertToRadians(15.0f));

    Camera::Instance().ScreenVibrationUpdate(elapsedTime);
}

// 描画処理
void Player::Render(const float scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, playerPS.Get());

    /*for (auto& skill : skillArray)
    {
        skill->Render();
    }*/
}

void Player::SkillImagesRender()
{
    int i = 0;
    float posX = 1220.0f;
    for (auto& skill : skillArray)
    {
        skill->Render();
        if (skill->GetOverlapNum() == 0)continue;
        skill->SetIconPos(DirectX::XMFLOAT2(posX,0));
        posX -= skill->icon->GetSpriteTransform()->GetSize().x;
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

            if (ImGui::Button("R Change LockOn Target"))
            {
                ChangeLockOnTarget(1.0f);
            }

            if (ImGui::Button("L Change LockOn Target"))
            {
                ChangeLockOnTarget(-1.0f);
            }

            ImGui::TreePop();
        }

        if(ImGui::TreeNode("Movement"))
        {
            ImGui::SliderFloat("MaxMoveSpeed", &maxEyeSpeed, 0.1f, 10.0f);

            ImGui::TreePop();
        }

        ImGui::Begin("Skill");
        ImGui::InputFloat("Exp", &curExp);
        if (ImGui::Button("Exp + 100"))
        {
            curExp += 100;
        }

        if (ImGui::Button("Skip"))
        {
            isSelectingSkill = false;
        }

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
    ImGui::Separator();

#endif // USE_IMGUI
}

void Player::AttackSteppingUpdate(float elapsedTime)
{
    //タイマーのリセットはステートマシンの方で行う
    if (steppingTimer > steppingTime)return;

    //速度算出
    float maxEyeSpeed = Easing::OutQuart(steppingTimer,steppingTime,steppingSpeed,0.0f);
    auto front = GetTransform()->CalcForward();
    eyeVelocity = front * maxEyeSpeed;

    //移動処理
    DirectX::XMFLOAT3 move = eyeVelocity * elapsedTime;
    GetTransform()->AddPosition(move);

    //回転処理
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    MoveVecUpdate(ax,ay);
    Turn(elapsedTime, moveVec.x, moveVec.z, 360.0f);

    steppingTimer += elapsedTime;
}

void Player::SelectSkillUpdate(float elapsedTime)
{
    static float _timer;
    switch (drawDirectionState)
    {
    case 0:
        DrawCards();
        selectCard = 0;

        for (int i = 0; i < 3; i++)
        {
            drawingSkillCards[i]->card->GetSpriteDissolve()->SetDissolveValue(-0.1f);
        }

        drawDirectionState++;
        break;

    case 1:
        for (int i = 0; i < 3; i++)
        {
            auto* transform = drawingSkillCards[i]->card->GetSpriteTransform();
            float timer = drawDirectionTimer - i * 0.1f;
            if (timer < 0)timer = 0;
            else if (timer > drawDirectionTime)timer = drawDirectionTime;
            float posY = Easing::OutSine(timer, drawDirectionTime, 165.0f, -transform->GetTexSize().y);
            transform->SetPos(DirectX::XMFLOAT2(65 + 400 * i, posY));

        }

        if (drawDirectionTimer >= 1.0f)
        {
            drawDirectionState++;
        }
        break;
    case 2:
    {
        //カード選択
        static bool buttonDown = false;
        auto ax = Input::Instance().GetGamePad().GetAxisLX();
        if (ax >= 0.5f && !buttonDown)
        {
            selectCard++;
            if (selectCard > 2)
            {
                selectCard = 2;
            }
            buttonDown = true;
        }
        else if (ax <= -0.5f && !buttonDown)
        {
            selectCard--;
            if (selectCard < 0)
            {
                selectCard = 0;
            }
            buttonDown = true;
        }
        else if (ax)buttonDown = true;
        else
        {
            buttonDown = false;
        }
    }

        //カードを選択
        if (Player::InputDecide())
        {
            _timer = 0;
            drawingSkillCards[selectCard]->Overlaping();
            drawDirectionState++;
        }
        break;

    case 3:
        if (drawingSkillCards[selectCard]->card->GetSpriteDissolve()->GetDissolveValue() < 2.0f)
        {
            drawingSkillCards[selectCard]->card->GetSpriteDissolve()->AddDissolveValue(elapsedTime);
        }

        for (int i = 0; i < 3; i++)
        {
            if (selectCard == i)continue;
            auto* transform = drawingSkillCards[i]->card->GetSpriteTransform();
            float timer = _timer;
            if (timer > 0.5)timer = 0.5;
            float posY = Easing::OutSine(timer, 0.5f, -transform->GetTexSize().y,165.0f);
            transform->SetPos(DirectX::XMFLOAT2(65 + 400 * i, posY));
        }

        if (_timer > 2.2f)isSelectingSkill = false;

        _timer += elapsedTime;
        break;
    }

    for (int i = 0; i < 3; i++)
    {
        drawingSkillCards[i]->card->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
    }
    drawingSkillCards[selectCard]->card->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(1,1,1,1));

    drawDirectionTimer += elapsedTime;
}

BaseSkill* Player::Lottery()
{
    std::vector<BaseSkill*> lotSkills;
    while (true)
    {
        //確率の分母を算出
        int denominator =
            BaseSkill::pCommon +
            BaseSkill::pUncommon +
            BaseSkill::pRare +
            BaseSkill::pSuperRare +
            BaseSkill::pUltraRare;
        int rarity = rand() % denominator;
        int checker = BaseSkill::pCommon;
        if (rarity < checker)rarity = BaseSkill::COMMON;
        else if (rarity < (checker += BaseSkill::pUncommon))rarity = BaseSkill::UNCOMMON;
        else if (rarity < (checker += BaseSkill::pRare))rarity = BaseSkill::RARE;
        else if (rarity < (checker += BaseSkill::pSuperRare))rarity = BaseSkill::SUPER_RARE;
        else if (rarity < (checker += BaseSkill::pUncommon))rarity = BaseSkill::ULTRA_RARE;

        //抽選対象のレア度のスキルを格納
        for (auto& skill : skillArray)
        {
            //すでに引かれたスキルはスキップ
            if (skill->isSelect)continue;

            if (skill->rarity == rarity)
            {
                lotSkills.emplace_back(skill);
            }
        }
        if (lotSkills.size() > 0)break;
        else lotSkills.clear();
    }

    auto* drawSkill = lotSkills.at(rand() % lotSkills.size());
    drawSkill->isSelect = true;
    return drawSkill;

}

void Player::DrawCards()
{
    for (auto& skill : skillArray)
    {
        skill->isSelect = false;
    }

    for (int i = 0; i < 3; i++)
    {
        drawingSkillCards[i] = Lottery();
    }
    drawDirectionTimer = 0;
}

bool Player::ChangeLockOnTarget(float ax)
{
    if (ax != 1 && ax != -1)return false;
    auto& camera = Camera::Instance();
    DirectX::XMFLOAT3 curLockOnTargetPos = camera.GetLockOnTarget()->GetPosition();

    //外積にプレイヤーから片側にいる敵のみ取得
    std::vector<Enemy*> sideEnemys;
    auto& enemyManager = EnemyManager::Instance();

    DirectX::XMFLOAT3 playerPos = GetTransform()->GetPosition();
    DirectX::XMFLOAT3 plVec = curLockOnTargetPos - playerPos;//プレイヤーからロックオン先のターゲットまでのベクトル
    //正規化
    const float length = sqrtf(plVec.x * plVec.x + plVec.z * plVec.z);
    plVec.x /= length;
    plVec.z /= length;

    for (size_t i = 0; i < enemyManager.GetEnemyCount(); i++)
    {
        auto* enemy = enemyManager.GetEnemy(i);
        DirectX::XMFLOAT3 enemyPos = enemy->GetTransform()->GetPosition();

        DirectX::XMFLOAT3 peVec = enemyPos - playerPos;//プレイヤーから敵までのベクトル
        //正規化
        const float length = sqrtf(peVec.x * peVec.x + peVec.z * peVec.z);
        peVec.x /= length;
        peVec.z /= length;

        //外積値からプレイヤーから見て敵がどちら側にいているか調べる(負の値なら右側に敵がいる)
        //引数のスティック入力方向に合わせてどちら側の敵を取得するか判断する
        float crossY = (plVec.x * peVec.z) - (plVec.z * peVec.x);
        if (ax == 1) //右側
        {
            if (crossY < 0)sideEnemys.emplace_back(enemy);
        }
        else if (ax == -1)//左側
        {
            if (crossY > 0)sideEnemys.emplace_back(enemy);
        }
    }

    //任意の方向に敵がいないならロックオンを切り替えない
    if (sideEnemys.size() == 0)return false;

    //一体のみならその敵をロックオン
    if (sideEnemys.size() == 1)
    {
        camera.SetLockOnTarget(sideEnemys.at(0));
        return true;
    }

    //複数の敵が任意の方向にいた場合、もっとも内積値の低い敵にロックオン
    Enemy* nextLockOnTarget = sideEnemys.at(0);
    float dot0 = -1;
    for (int i = 1; i < sideEnemys.size(); i++)
    {
        DirectX::XMFLOAT3 peVec = sideEnemys.at(i)->GetPosition() - playerPos;
        //正規化
        const float length = sqrtf(peVec.x * peVec.x + peVec.z * peVec.z);
        peVec.x /= length;
        peVec.z /= length;
        float dot1 = (plVec.x * peVec.x) + (plVec.z * peVec.z);
        
        if (dot0 < dot1)
        {
            dot0 = dot1;
            nextLockOnTarget = sideEnemys.at(i);
        }
    }

    camera.SetLockOnTarget(nextLockOnTarget);

    return true;
}

void Player::LockOnUpdate()
{
    //ロックオン先のエネミーが死んだとき左右の敵にロックオン切り替えし
    // 敵がいなかった場合、ロックオンを解除する
    if (Camera::Instance().GetLockOnTarget()->isDead)
    {
        if (ChangeLockOnTarget(1))return;
        else if (ChangeLockOnTarget(-1))return;

        Camera::Instance().activeLockOn = false;
        return;
    }

    float ax = Input::Instance().GetGamePad().GetAxisRX();

    static bool buttonDown = false;
    if (!buttonDown)
    {   
        ChangeLockOnTarget(ax);
        buttonDown = true;
    }
    if (ax <= 0.1f && ax >= -0.1f)
    {
        buttonDown = false;
    }
}

void Player::LockOnInitialize()
{
    if (EnemyManager::Instance().GetEnemyCount() == 0)return;
    float length0 = FLT_MAX;
    for (auto& enemy : EnemyManager::Instance().GetEnemies())
    {
        auto ePos = enemy->GetTransform()->GetPosition();
        float length1 = Length(ePos - GetTransform()->GetPosition());
        if (length0 > length1)
        {
            Camera::Instance().SetLockOnTarget(enemy);
            length0 = length1;
        }
    }
}

void Player::LevelUpdate()
{
    if(curExp > levelUpExp)
    {
        for (auto& skill : skillArray)
        {
            //カードが映りこまないように適当な座標に飛ばしとく
            skill->card->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(0, 1000));
        }

        level++;
        curExp -= levelUpExp;

        drawDirectionState = 0;

        //レベルが上がると能力を取得出来る
        isSelectingSkill = true;
    }
}
