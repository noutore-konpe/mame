#include "TutorialManager.h"
#include "../Graphics/Graphics.h"

// テキスト画像パスの配列
static constexpr wchar_t* const TUTORIAL_TEXTS_[static_cast<int>(TUTORIAL::COUNT)] = {
    L"./Resources/UI/Text0.png",
    L"./Resources/UI/Text0.png",
    L"./Resources/UI/Text0.png",
};

// テキスト位置の配列
static constexpr DirectX::XMFLOAT2 TUTORIAL_TEXT_POSITIONS_[static_cast<int>(TUTORIAL::COUNT)] = {
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
};


void TutorialManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    // 現在のチュートリアル番号初期化
    currentTutorialIndex_ = 0;

    // スプライト生成
    {
        // テキスト生成
        text_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            TUTORIAL_TEXTS_[currentTutorialIndex_]
        );

        // チェックマーク生成
        checkMark_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/UI/CheckMark.png"
        );

        // テキスト背景生成
        textBackGround_ = std::make_unique<Sprite>(
            graphics.GetDevice(),
            L"./Resources/UI/TextBackGround.png"
        );
    }

    // パラメータ初期化
    {
        text_->Initialize(
            TUTORIAL_TEXT_POSITIONS_[currentTutorialIndex_],
            textParam_.size_,
            textParam_.texSize_,
            textParam_.texPos_,
            textParam_.color_,
            textParam_.angle_
        );

        checkMark_->Initialize(
            text_->GetSpriteTransform()->GetPos(),
            checkMarkParam_.size_,
            checkMarkParam_.texSize_,
            checkMarkParam_.texPos_,
            checkMarkParam_.color_,
            checkMarkParam_.angle_
        );

        textBackGround_->Initialize(
            text_->GetSpriteTransform()->GetPos(),
            textBackGroundParam_.size_,
            textBackGroundParam_.texSize_,
            textBackGroundParam_.texPos_,
            textBackGroundParam_.color_,
            textBackGroundParam_.angle_
        );

        // チュートリアル達成フラグリセット
        for (int i = 0; i < TUTORIAL_COUNT_; ++i)
        {
            tutorialCompleteFlag_[i] = false;
        }

    }

}

const bool TutorialManager::Update(const float elapsedTime)
{
    // 現在のチュートリアルの達成フラグが立っていたら
    // 次のチュートリアルに移る
    if (true == tutorialCompleteFlag_[currentTutorialIndex_])
    {
        // チュートリアル終了フラグを取得
        const bool tutorialEndFlag = MoveNextTutorial(); // 次のチュートリアルに移る

        // チュートリアルが最後まで行っていたら終了する
        if (true == tutorialEndFlag)
        {
            // チュートリアル終了
            return false;
        }

        // チュートリアル実行中
        return true;
    }


    // チュートリアル実行中
    return true;
}

void TutorialManager::Render()
{
    textBackGround_->Render();
    text_->Render();
    checkMark_->Render();
}

const bool TutorialManager::MoveNextTutorial()
{
    Graphics& graphics = Graphics::Instance();

    // チュートリアル番号加算
    ++currentTutorialIndex_;

    // チュートリアルが最後まで行ったらチュートリアル終了
    if (currentTutorialIndex_ >= TUTORIAL_COUNT_)
    {
        return false;
    }

    // テキスト再生成
    text_ = std::make_unique<Sprite>(
        graphics.GetDevice(),
        TUTORIAL_TEXTS_[currentTutorialIndex_]
    );

    // テキスト初期化
    text_->Initialize(
        TUTORIAL_TEXT_POSITIONS_[currentTutorialIndex_],
        textParam_.size_,
        textParam_.texSize_,
        textParam_.texPos_,
        textParam_.color_,
        textParam_.angle_
    );

    return true;
}
