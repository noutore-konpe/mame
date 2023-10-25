#include "Tutorial.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Other/Easing.h"

#include "TutorialManager.h"
#include "EnemyManager.h"
#include "EnemyAI_Tutorial.h"
#include "PlayerManager.h"
#include "Player.h"
#include "WaveManager.h"

// ベースチュートリアル
#pragma region BaseTutorial

BaseTutorial::BaseTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // スプライト生成
    {
        // ※テキストスプライトは各継承クラスで生成する

        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/CheckMark.png",
            "./Resources/Shader/sprite_dissolve_ps.cso" // ディゾルブさせる
        );

        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/textBackGround.png"
        );
    }

}

void BaseTutorial::Initialize()
{
    using DirectX::XMFLOAT2;
    using DirectX::XMFLOAT4;

    XMFLOAT2 pos     = {};
    XMFLOAT2 texSize = {};
    XMFLOAT2 scale   = {};
    XMFLOAT2 size    = {};
    XMFLOAT2 texPos  = {};
    XMFLOAT4 color   = {};
    float    angle   = {};

    // テキスト背景初期化
    // 親の位置（※テキスト背景の位置を親の位置にする）
    const XMFLOAT2 parentPos = { 840.0f, 215.0f };
    {
        texSize = textBackGround_->GetSpriteTransform()->GetTexSize();
        scale   = { 1.0f, 1.0f };
        //size    = { texSize.x * scale.x, texSize.y * scale.y };
        size    = { 400.0f * scale.x, 200.0f * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 0.0f };
        angle   = 0.0f;
        textBackGround_->Initialize(parentPos, size, texSize, texPos, color, angle);
    }


    // テキスト初期化
    {
        // 親の位置を基準にして位置を設定する
        pos.x   = parentPos.x + (-4.0f);
        pos.y   = parentPos.y + (+10.0f);
        texSize = text_->GetSpriteTransform()->GetTexSize();
        scale   = { 1.0f, 1.0f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 0.0f };
        angle   = 0.0f;
        text_->Initialize(pos, size, texSize, texPos, color, angle);
    }

    // チェックマーク初期化
    {
        // 親の位置を基準にして位置を設定する
        pos.x   = parentPos.x + (-20.0f);
        pos.y   = parentPos.y + (-30.0f);
        texSize = checkMark_->GetSpriteTransform()->GetTexSize();
        scale   = { 0.3f, 0.3f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 1.0f };
        angle   = 0.0f;
        checkMark_->Initialize(pos, size, texSize, texPos, color, angle);

        // ディゾルブ初期設定
        checkMark_->GetSpriteDissolve()->SetDissolveValue(1.0f);

    }

    createTimer_ = 0.0f;
    easingTimer_ = 0.0f;
    step_        = 0;

}

void BaseTutorial::Update(const float elapsedTime)
{
     TutorialManager& tutorialManager = TutorialManager::Instance();

    Sprite::SpriteTransform* textT      = text_->GetSpriteTransform();
    Sprite::SpriteTransform* checkMarkT = checkMark_->GetSpriteTransform();
    Sprite::SpriteTransform* textBG_T   = textBackGround_->GetSpriteTransform();

    // ディゾルブ更新の呼び出しはせずこちらで更新を行う
    //text_->Update(elapsedTime);
    //checkMark_->Update(elapsedTime);
    //textBackGround_->Update(elapsedTime);

    float textColorAlpha        = 0.0f;
    float textBG_ColorAlpha     = 0.0f;

    switch (step_)
    {
    case 0:
        // イージングタイマー更新
        easingTimer_ += elapsedTime;

        textColorAlpha      = textT->GetColorA();
        textBG_ColorAlpha   = textBG_T->GetColorA();

        {
            const bool isEndEaseIn[2] = {
                BaseTutorial::colorAlphaEaseIn(&textColorAlpha),
                BaseTutorial::colorAlphaEaseIn(&textBG_ColorAlpha),
            };

            // すべてのスプライトが不透明になったら
            // 次のステップに移る
            if (false == isEndEaseIn[0] &&
                false == isEndEaseIn[1]   )
            {
                ++step_;

                // イージングタイマーリセット
                easingTimer_ = 0.0f;
            }
        }

        // スプライト不透明度更新
        textT->SetColorA(textColorAlpha);
        textBG_T->SetColorA(textBG_ColorAlpha);

        break;
    case 1:
        // 次のステップに移れるなら次のステップに移動
        if (true == MoveNextStepJudgment())
        {
            ++step_;
            break;
        }

        break;
    case 2:
        // チェックマークのディゾルブインが終わったら次のステップに移動
        {
            Sprite::SpriteDissolve* checkMarkDissolve = checkMark_->GetSpriteDissolve();
            checkMarkDissolve->AddDissolveValue(-elapsedTime);
            if (checkMarkDissolve->GetDissolveValue() <= (-0.1f))
            {
                checkMarkDissolve->SetDissolveValue(-0.1f);
                ++step_;
                break;
            }
        }

        break;
    case 3: // 待機
        // タイマー更新(イージングタイマーを待機タイマー代わりに使用)
        easingTimer_ += elapsedTime;

        // しばらく間を開けたら次のステップに移動
        if (easingTimer_ >= 2.0)
        {
            // イージングタイマーリセット
            easingTimer_ = 0.0f;
            ++step_;
            break;
        }

        break;
    case 4:
        // イージングタイマー更新
        easingTimer_ += elapsedTime;

        // チェックマークのディゾルブアウト更新
        {
            Sprite::SpriteDissolve* checkMarkDissolve = checkMark_->GetSpriteDissolve();
            if (checkMarkDissolve->GetDissolveValue() < 1.0f)
            {
                checkMarkDissolve->AddDissolveValue(elapsedTime);
            }
        }

        textColorAlpha      = textT->GetColorA();
        textBG_ColorAlpha   = textBG_T->GetColorA();

        {
            const bool isEndEaseOut[2] = {
                BaseTutorial::colorAlphaEaseOut(&textColorAlpha),
                BaseTutorial::colorAlphaEaseOut(&textBG_ColorAlpha),
            };

            // すべてのスプライトが透明になったら
            // チュートリアル達成フラグを立てる
            if (false == isEndEaseOut[0] &&
                false == isEndEaseOut[1]   )
            {
                tutorialManager.CompleteTutorial();

                // イージングタイマーリセット
                easingTimer_ = 0.0f;
            }
        }

        // スプライト不透明度更新
        textT->SetColorA(textColorAlpha);
        textBG_T->SetColorA(textBG_ColorAlpha);

        break;
    }

}

void BaseTutorial::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

void BaseTutorial::DrawImGui()
{
#if USE_IMGUI

    using DirectX::XMFLOAT2;

    Sprite::SpriteTransform* textBackGroundT = textBackGround_->GetSpriteTransform();
    Sprite::SpriteTransform* textT           = text_->GetSpriteTransform();
    Sprite::SpriteTransform* checkMarkT      = checkMark_->GetSpriteTransform();

    static constexpr float POS_LIMIT   = 1280.0f;
    static constexpr float SCALE_LIMIT = 20.0f;

    if (ImGui::BeginMenu("TextBackGround"))
    {
        textBackGround_->DrawDebug();

        // 位置
        {
            // 他のスプライト位置も動かす
            XMFLOAT2 variationPos = textBackGroundT->GetPos();
            const XMFLOAT2 oldPos = variationPos;
            ImGui::SliderFloat2("variationPos", &variationPos.x, 0.0f, POS_LIMIT);
            textBackGroundT->SetPos(variationPos);

            // 他のスプライトも一緒に移動させる
            const XMFLOAT2 addChlidPos = {
                (variationPos.x - oldPos.x), (variationPos.y - oldPos.y)
            };
            textT->AddPosX(addChlidPos.x);
            textT->AddPosY(addChlidPos.y);
            checkMarkT->AddPosX(addChlidPos.x);
            checkMarkT->AddPosY(addChlidPos.y);
        }

        // スケール
        {
            // scaleをsizeに反映させる
            const XMFLOAT2& texSize = textBackGroundT->GetTexSize();
            float scaleX    = textBackGroundT->GetSize().x / textBackGroundT->GetTexSize().x;
            float scaleY    = textBackGroundT->GetSize().y / textBackGroundT->GetTexSize().y;
            float oldScaleX = scaleX;
            ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
            // scaleXの変動量をscaleYにも反映させる
            const float variationScaleX = scaleX - oldScaleX;
            scaleY += variationScaleX;
            textBackGroundT->SetSizeX(texSize.x * scaleX);
            textBackGroundT->SetSizeY(texSize.y * scaleY);

            // 基準点が違ったので失敗
            //// 他のスプライトも一緒にスケーリングさせる
            //{
            //    const float textScaleX = textT->GetSize().x / textT->GetTexSize().x;
            //    const float textScaleY = textT->GetSize().y / textT->GetTexSize().y;
            //    textT->SetSizeX(textT->GetTexSize().x * (textScaleX + variationScaleX));
            //    textT->SetSizeY(textT->GetTexSize().y * (textScaleY + variationScaleX));
            //
            //    const float checkMarkScaleX = checkMarkT->GetSize().x / checkMarkT->GetTexSize().x;
            //    const float checkMarkScaleY = checkMarkT->GetSize().y / checkMarkT->GetTexSize().y;
            //    checkMarkT->SetSizeX(checkMarkT->GetTexSize().x * (checkMarkScaleX + variationScaleX));
            //    checkMarkT->SetSizeY(checkMarkT->GetTexSize().y * (checkMarkScaleY + variationScaleX));
            //}
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Text"))
    {
        textBackGround_->DrawDebug();

        // 親の位置からどれくらいずれているかのメモ用
        const XMFLOAT2& pos = textT->GetPos();
        const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
        XMFLOAT2 variationPos = {
            pos.x - textBackGroundPos.x,
            pos.y - textBackGroundPos.y,
        };
        ImGui::InputFloat2("variationPos", &variationPos.x);

        // scaleをsizeに反映させる
        const XMFLOAT2& texSize = textT->GetTexSize();
        float scaleX    = textT->GetSize().x / textT->GetTexSize().x;
        float scaleY    = textT->GetSize().y / textT->GetTexSize().y;
        float oldScaleX = scaleX;
        ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
        // scaleXの変動量をscaleYにも反映させる
        scaleY += scaleX - oldScaleX;
        textT->SetSizeX(texSize.x * scaleX);
        textT->SetSizeY(texSize.y * scaleY);

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("CheckMark"))
    {
        // 親の位置からどれくらいずれているかのメモ用
        const XMFLOAT2& pos = checkMarkT->GetPos();
        const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
        XMFLOAT2 variationPos = {
            pos.x - textBackGroundPos.x,
            pos.y - textBackGroundPos.y,
        };
        ImGui::InputFloat2("variationPos", &variationPos.x);

        // scaleをsizeに反映させる
        const XMFLOAT2& texSize = checkMarkT->GetTexSize();
        float scaleX    = checkMarkT->GetSize().x / checkMarkT->GetTexSize().x;
        float scaleY    = checkMarkT->GetSize().y / checkMarkT->GetTexSize().y;
        float oldScaleX = scaleX;
        ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
        // scaleXの変動量をscaleYにも反映させる
        scaleY += scaleX - oldScaleX;
        checkMarkT->SetSizeX(texSize.x * scaleX);
        checkMarkT->SetSizeY(texSize.y * scaleY);

        float dissolveValue = checkMark_->GetSpriteDissolve()->GetDissolveValue();
        ImGui::SliderFloat("dissolveValue", &dissolveValue, -0.1f, 1.0f);
        checkMark_->GetSpriteDissolve()->SetDissolveValue(dissolveValue);

        ImGui::EndMenu();
    }

#endif
}

const bool BaseTutorial::colorAlphaEaseIn(float* colorAlpha, const float endTime)
{
    if (easingTimer_ > endTime) { easingTimer_ = endTime; }

    // 不透明度増加
    (*colorAlpha) = Easing::InSine(easingTimer_, endTime, 1.0f, 0.0f);

    // 不透明度が1.0f以上になったらfalseを返す
    if ((*colorAlpha) >= 1.0f) { return false; }

    // EaseIn実行中
    return true;
}

const bool BaseTutorial::colorAlphaEaseOut(float* colorAlpha, const float endTime)
{
    if (easingTimer_ > endTime) { easingTimer_ = endTime; }

    // 不透明度減少
    (*colorAlpha) = Easing::OutSine(easingTimer_, endTime, 0.0f, 1.0f);

    // 不透明度がゼロ以下になったらfalseを返す
    if ((*colorAlpha) <= 0.0f) { return false; }

    // EaseOut実行中
    return true;
}

void BaseTutorial::UpdateCreateEnemy(
    const float elapsedTime,
    const int createCount = 1,
    const int dropExpCount = 0)
{
    using DirectX::XMFLOAT3;

    EnemyManager& enemyManager = EnemyManager::Instance();

    // 指定した数だけ生成するようにする
    if (enemyManager.GetEnemyCount() >= createCount)
    {
        // 生成タイマーリセット
        createTimer_ = 0.0f;

        return;
    }

    // 生成タイマー更新
    createTimer_ = (std::min)(CREATE_TIME_, createTimer_ + elapsedTime);

    // 生成時間に達していなければreturn
    if (createTimer_ < CREATE_TIME_) return;

    // 敵生成
    {
        EnemyAI_Tutorial* tutorialEnemy = new EnemyAI_Tutorial();
        tutorialEnemy->Initialize();

        // ランダムなゲート位置から生成
        const XMFLOAT3 createPos = ::GetGatewayPosition(-1);
        tutorialEnemy->SetPosition(createPos);

        // 無敵解除
        tutorialEnemy->SetIsInvincible(false);

        // ドロップ経験値設定
        tutorialEnemy->SetDropExpCount(dropExpCount);

        enemyManager.Register(tutorialEnemy);
    }

    // 生成タイマーリセット
    createTimer_ = 0.0f;

}

#pragma endregion


// 移動チュートリアル
#pragma region TutorialMove

TutorialMove::TutorialMove()
{
    Graphics& graphics = Graphics::Instance();

    // テキストスプライト生成
    text_ = std::make_unique<Sprite>(
        graphics.GetDevice(),
        L"./Resources/Image/Tutorial/Texts/Move.png"
    );

}

void TutorialMove::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialMove"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialMove::MoveNextStepJudgment()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    // 移動入力がされていたらtrueを返す
    if (gamePad.GetAxisLX() != 0.0f ||
        gamePad.GetAxisLY() != 0.0f)
    {
        return true;
    }

    return false;
}

#pragma endregion


// カメラ移動チュートリアル
#pragma region TutorialMoveCamera

TutorialMoveCamera::TutorialMoveCamera()
{
    Graphics& graphics = Graphics::Instance();

    // テキストスプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/MoveCamera.png"
        );
    }

}

void TutorialMoveCamera::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialMoveCamera"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialMoveCamera::MoveNextStepJudgment()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();
    const Mouse&   mouse   = Input::Instance().GetMouse();

    // マウスのカメラ移動入力がされていたらtrueを返す
    if (mouse.GetPositionX() != mouse.GetOldPositionX() ||
        mouse.GetPositionY() != mouse.GetOldPositionY())
    {
        return true;
    }

    // キーボードのカメラ移動入力がされていたらtrueを返す
    if (gamePad.GetAxisRX() != 0.0f ||
        gamePad.GetAxisRY() != 0.0f)
    {
        return true;
    }

    return false;
}

#pragma endregion


// ロックオンチュートリアル
#pragma region TutorialLockOn

TutorialLockOn::TutorialLockOn()
{
    Graphics& graphics = Graphics::Instance();

    // テキストスプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/LockOn.png"
        );
    }

}

void TutorialLockOn::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // 敵生成更新
    UpdateCreateEnemy(elapsedTime);

    BaseTutorial::Update(elapsedTime);
}

void TutorialLockOn::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialLockOn"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialLockOn::MoveNextStepJudgment()
{
    const Camera& camera = Camera::Instance();

    // ロックオンが有効になっていればtrueを返す
    if (true == camera.GetActiveLockOn())
    {
        return true;
    }

    return false;
}

#pragma endregion


// 弱攻撃チュートリアル
#pragma region TutorialLowAttack

TutorialLowAttack::TutorialLowAttack()
{
    Graphics& graphics = Graphics::Instance();

    // テキストスプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/LowAttack.png"
        );
    }
}

void TutorialLowAttack::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // 敵生成更新
    UpdateCreateEnemy(elapsedTime);

    BaseTutorial::Update(elapsedTime);
}

void TutorialLowAttack::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialLowAttack"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialLowAttack::MoveNextStepJudgment()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    if (true == Player::InputJabAttack()) { return true; }

    return false;
}

#pragma endregion

// 強攻撃チュートリアル
#pragma region TutorialHighAttack

TutorialHighAttack::TutorialHighAttack()
{
    Graphics& graphics = Graphics::Instance();

    // テキストスプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/HighAttack.png"
        );
    }
}

void TutorialHighAttack::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // 敵生成更新
    UpdateCreateEnemy(elapsedTime);

    BaseTutorial::Update(elapsedTime);
}

void TutorialHighAttack::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialHighAttack"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialHighAttack::MoveNextStepJudgment()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    if (true == Player::InputHardAttack()) { return true; }

    return false;
}

#pragma endregion


// 回避チュートリアル
#pragma region TutorialAvoid

TutorialAvoid::TutorialAvoid()
{
    Graphics& graphics = Graphics::Instance();

    // テキストスプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/Avoid.png"
        );
    }

}

void TutorialAvoid::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialAvoid"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialAvoid::MoveNextStepJudgment()
{
    const GamePad& gamePad = Input::Instance().GetGamePad();

    if (true == Player::InputAvoid()) { return true; }

    return false;
}

#pragma endregion


// レベルアップチュートリアル
#pragma region TutorialLevelUp

TutorialLevelUp::TutorialLevelUp()
{
    Graphics& graphics = Graphics::Instance();

    // テキストスプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            //L"./Resources/Image/Tutorial/Texts/LevelUp.png"
            L"./Resources/Image/Tutorial/Texts/LockOn.png"
        );
    }

}

void TutorialLevelUp::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // 敵生成更新
    const int createCount = 3;
    const int dropExpCount = 8;
    UpdateCreateEnemy(elapsedTime, createCount, dropExpCount);

    BaseTutorial::Update(elapsedTime);

}

void TutorialLevelUp::Render()
{
    PlayerManager& playerManager = PlayerManager::Instance();

    //// スキルカード選択中は描画しないようにする
    //Player* player = playerManager.GetPlayer().get();
    //if (true == player->isSelectingSkill) { return; }

    BaseTutorial::Render();
}

void TutorialLevelUp::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialLevelUp"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialLevelUp::MoveNextStepJudgment()
{
    PlayerManager& playerManager = PlayerManager::Instance();

    // レベルが上がったらtrueを返す
    Player* player = playerManager.GetPlayer().get();
    if (player->GetLevel() > 1) { return true; }

    return false;
}

#pragma endregion
