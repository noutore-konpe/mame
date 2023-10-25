#include "Tutorial.h"

#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Other/Easing.h"
#include "TutorialManager.h"
#include "EnemyManager.h"
#include "EnemyAI_Tutorial.h"

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
    const XMFLOAT2 parentPos = { 735.0f, 215.0f };
    {
        texSize = { 520.0f, 300.0f };
        scale   = { 1.0f, 1.0f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 0.0f };
        angle   = 0.0f;
        textBackGround_->Initialize(parentPos, size, texSize, texPos, color, angle);
    }

    // テキスト初期化
    {
        // 親の位置を基準にして位置を設定する
        pos.x   = parentPos.x + (57.0f);
        pos.y   = parentPos.y + (51.0f);
        texSize = { 400.0f, 200.0f };
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
        texSize = { 512.0f, 350.0f };
        scale   = { 0.3f, 0.3f };
        size    = { texSize.x * scale.x, texSize.y * scale.y };
        texPos  = { 0.0f, 0.0f };
        color   = { 1.0f, 1.0f, 1.0f, 0.0f };
        angle   = 0.0f;
        checkMark_->Initialize(pos, size, texSize, texPos, color, angle);
    }

}

void BaseTutorial::Update(const float elapsedTime)
{
     TutorialManager& tutorialManager = TutorialManager::Instance();

    Sprite::SpriteTransform* textT      = text_->GetSpriteTransform();
    Sprite::SpriteTransform* checkMarkT = checkMark_->GetSpriteTransform();
    Sprite::SpriteTransform* textBG_T   = textBackGround_->GetSpriteTransform();

    text_->Update(elapsedTime);
    checkMark_->Update(elapsedTime);
    textBackGround_->Update(elapsedTime);

    float textColorAlpha        = 0.0f;
    float checkMarkColorAlpha   = 0.0f;
    float textBG_ColorAlpha     = 0.0f;

    switch (step_)
    {
    case 0:
        // イージングタイマー更新
        easingTimer_ += elapsedTime;

        textColorAlpha      = textT->GetColorA();
        checkMarkColorAlpha = checkMarkT->GetColorA();
        textBG_ColorAlpha   = textBG_T->GetColorA();

        {
            const bool isEndEaseIn[3] = {
                BaseTutorial::colorAlphaEaseIn(&textColorAlpha),
                BaseTutorial::colorAlphaEaseIn(&checkMarkColorAlpha),
                BaseTutorial::colorAlphaEaseIn(&textBG_ColorAlpha),
            };

            // すべてのスプライトが不透明になったら
            // 次のステップに移る
            if (false == isEndEaseIn[0] &&
                false == isEndEaseIn[1] &&
                false == isEndEaseIn[2])
            {
                ++step_;

                // イージングタイマーリセット
                easingTimer_ = 0.0f;
            }
        }

        // スプライト不透明度更新
        textT->SetColorA(textColorAlpha);
        checkMarkT->SetColorA(checkMarkColorAlpha);
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
        // イージンタイマー更新
        easingTimer_ += elapsedTime;

        textColorAlpha      = textT->GetColorA();
        checkMarkColorAlpha = checkMarkT->GetColorA();
        textBG_ColorAlpha   = textBG_T->GetColorA();

        {
            const bool isEndEaseOut[3] = {
                BaseTutorial::colorAlphaEaseOut(&textColorAlpha),
                BaseTutorial::colorAlphaEaseOut(&checkMarkColorAlpha),
                BaseTutorial::colorAlphaEaseOut(&textBG_ColorAlpha),
            };

            // すべてのスプライトが透明になったら
            // チュートリアル達成フラグを立てる
            if (false == isEndEaseOut[0] &&
                false == isEndEaseOut[1] &&
                false == isEndEaseOut[2])
            {
                tutorialManager.CompleteTutorial();

                // イージングタイマーリセット
                easingTimer_ = 0.0f;
            }
        }

        // スプライト不透明度更新
        textT->SetColorA(textColorAlpha);
        checkMarkT->SetColorA(checkMarkColorAlpha);
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
        XMFLOAT2 pos = textBackGroundT->GetPos();
        const XMFLOAT2 oldPos = pos;
        ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
        textBackGroundT->SetPos(pos);

        // 他のスプライトも一緒に移動させる
        const XMFLOAT2 addChlidPos = {
            (pos.x - oldPos.x), (pos.y - oldPos.y)
        };
        textT->AddPosX(addChlidPos.x);
        textT->AddPosY(addChlidPos.y);
        checkMarkT->AddPosX(addChlidPos.x);
        checkMarkT->AddPosY(addChlidPos.y);

        // scaleをsizeに反映させる
        const XMFLOAT2& texSize = textBackGroundT->GetTexSize();
        float scaleX    = textBackGroundT->GetSize().x / textBackGroundT->GetTexSize().x;
        float scaleY    = textBackGroundT->GetSize().y / textBackGroundT->GetTexSize().y;
        float oldScaleX = scaleX;
        ImGui::SliderFloat("scale", &scaleX, 0.0f, SCALE_LIMIT);
        // scaleXの変動量をscaleYにも反映させる
        scaleY += scaleX - oldScaleX;
        textBackGroundT->SetSizeX(texSize.x * scaleX);
        textBackGroundT->SetSizeY(texSize.y * scaleY);

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Text"))
    {
        XMFLOAT2 pos = textT->GetPos();
        ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
        textT->SetPos(pos);

        // 親の位置からどれくらいずれているかのメモ用
        const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
        XMFLOAT2 textVariation = {
            pos.x - textBackGroundPos.x,
            pos.y - textBackGroundPos.y,
        };
        ImGui::InputFloat2("textVariation", &textVariation.x);

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
        XMFLOAT2 pos = checkMarkT->GetPos();
        ImGui::SliderFloat2("pos", &pos.x, 0.0f, POS_LIMIT);
        checkMarkT->SetPos(pos);

        // 親の位置からどれくらいずれているかのメモ用
        const XMFLOAT2& textBackGroundPos = textBackGroundT->GetPos();
        XMFLOAT2 checkMarkVariation = {
            pos.x - textBackGroundPos.x,
            pos.y - textBackGroundPos.y,
        };
        ImGui::InputFloat2("checkMarkVariation", &checkMarkVariation.x);

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

        static float dissolveValue = 1.0f;
        ImGui::SliderFloat("dissolveValue", &dissolveValue, -1.0f, 1.0f);
        text_->GetSpriteDissolve()->SetDissolveValue(dissolveValue);


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

    // カメラ移動入力がされていたらtrueを返す
    if (gamePad.GetAxisRY() != 0.0f ||
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

void TutorialLockOn::Initialize()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // ベースチュートリアルの初期化関数呼び出し
    BaseTutorial::Initialize();

    // チュートリアルエネミー生成・初期化・登録
    {
        EnemyAI_Tutorial* tutorialEnemy = new EnemyAI_Tutorial();
        tutorialEnemy->Initialize();
        enemyManager.Register(tutorialEnemy);
    }

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

    //// ロックオンが有効になっていればtrueを返す
    //if (true == camera.GetActiveLockOn())
    //{
    //    return true;
    //}

    return false;
}

#pragma endregion


// 攻撃チュートリアル
#pragma region TutorialAttack

TutorialAttack::TutorialAttack()
{
    Graphics& graphics = Graphics::Instance();

    // テキストスプライト生成
    {
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/Image/Tutorial/Texts/Attack.png"
        );
    }
}

void TutorialAttack::DrawImGui()
{
#if USE_IMGUI

    if (ImGui::BeginMenu("TutorialAttack"))
    {
        BaseTutorial::DrawImGui();
        ImGui::EndMenu();
    }

#endif
}

const bool TutorialAttack::MoveNextStepJudgment()
{
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
            L"./Resources/Image/Tutorial/Texts/LevelUp.png"
        );
    }

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
    return false;
}

#pragma endregion
