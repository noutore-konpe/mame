#include "WaveManager.h"

#include "EnemyManager.h"
#include "EnemyAI_1.h"
#include "EnemyAI_2.h"
#include "EnemyAI_3.h"
#include "EnemyGolem.h"

#include "UserInterface.h"

#include "../Resource/AudioManager.h"

// ウェーブ設定
// ※要素数の取得や変更などを行うときにクラス内で定義していると面倒なので外で定義

// ウェーブエネミーセット(※ウェーブに格納する)
#pragma region WaveEnemySet

namespace Set
{
    static constexpr float HP       = 70.0f;
    static constexpr float GolemHp  = 300.0f;
    static constexpr float ADD_HP   = 5.0f;
    static constexpr float ATK      = 20.0f;
    static constexpr float ADD_ATK  = 3.0f;
}

namespace EndlessSet
{
    static constexpr float HP       = Set::HP + (Set::ADD_HP * 10.0f);
    static constexpr float GOLEM_HP = Set::GolemHp + (Set::ADD_HP * 10.0f);
    static constexpr float ATK      = Set::ATK + (Set::ADD_ATK * 10.0f);
    static constexpr float EXP      = 5;
}

#if _DEBUG
WaveEnemySet waveEnemySetTest[] = {
     WaveEnemySet(0.0f, "EnemyAI_1",  ::GetGatewayPosition(0), Set::HP, Set::ATK, 5),
     WaveEnemySet(0.0f, "EnemyAI_1",  ::GetGatewayPosition(0), Set::HP, Set::ATK, 5),
     WaveEnemySet(0.0f, "EnemyAI_2",  ::GetGatewayPosition(0), Set::HP, Set::ATK, 5),
     WaveEnemySet(0.0f, "EnemyAI_2",  ::GetGatewayPosition(0), Set::HP, Set::ATK, 5),
     WaveEnemySet(0.0f, "EnemyAI_3",  ::GetGatewayPosition(0), Set::HP, Set::ATK, 5),
     WaveEnemySet(0.0f, "EnemyAI_3",  ::GetGatewayPosition(0), Set::HP, Set::ATK, 5),
     WaveEnemySet(0.0f, "EnemyGolem", ::GetGatewayPosition(0), Set::HP, Set::ATK, 5),
};
#endif
WaveEnemySet waveEnemySet0[] = {
    WaveEnemySet(0.5f, "EnemyGolem", SceneGame::stageCenter, Set::GolemHp + (Set::ADD_HP * 10.0f), Set::ATK + (Set::ADD_ATK * 10.0f), 40),
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 0.0f), Set::ATK + (Set::ADD_ATK * 0.0f), 5),
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 0.0f), Set::ATK + (Set::ADD_ATK * 0.0f), 5),
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(2), Set::HP + (Set::ADD_HP * 0.0f), Set::ATK + (Set::ADD_ATK * 0.0f), 5),
};
WaveEnemySet waveEnemySet1[] = {
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 1.0f), Set::ATK + (Set::ADD_ATK * 1.0f), 5),
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 1.0f), Set::ATK + (Set::ADD_ATK * 1.0f), 5),
    WaveEnemySet(0.5f, "EnemyAI_2", ::GetGatewayPosition(2), Set::HP + (Set::ADD_HP * 1.0f), Set::ATK + (Set::ADD_ATK * 1.0f), 8),
};
WaveEnemySet waveEnemySet2[] = {
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 2.0f), Set::ATK + (Set::ADD_ATK * 2.0f), 7),
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 2.0f), Set::ATK + (Set::ADD_ATK * 2.0f), 7),
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(2), Set::HP + (Set::ADD_HP * 2.0f), Set::ATK + (Set::ADD_ATK * 2.0f), 7),
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 2.0f), Set::ATK + (Set::ADD_ATK * 2.0f), 10),
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 2.0f), Set::ATK + (Set::ADD_ATK * 2.0f), 10),
};
WaveEnemySet waveEnemySet3[] = {
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 3.0f), Set::ATK + (Set::ADD_ATK * 3.0f), 10),
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 3.0f), Set::ATK + (Set::ADD_ATK * 3.0f), 10),
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(2), Set::HP + (Set::ADD_HP * 3.0f), Set::ATK + (Set::ADD_ATK * 3.0f), 10),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(3), Set::HP + (Set::ADD_HP * 3.0f), Set::ATK + (Set::ADD_ATK * 3.0f), 10),
};
WaveEnemySet waveEnemySet4[] = {
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 4.0f), Set::ATK + (Set::ADD_ATK * 4.0f), 10),
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 4.0f), Set::ATK + (Set::ADD_ATK * 4.0f), 10),
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(2), Set::HP + (Set::ADD_HP * 4.0f), Set::ATK + (Set::ADD_ATK * 4.0f), 10),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(3), Set::HP + (Set::ADD_HP * 4.0f), Set::ATK + (Set::ADD_ATK * 4.0f), 10),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 4.0f), Set::ATK + (Set::ADD_ATK * 4.0f), 10),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 4.0f), Set::ATK + (Set::ADD_ATK * 4.0f), 10),
};
WaveEnemySet waveEnemySet5[] = {
    WaveEnemySet(1.0f, "EnemyGolem", SceneGame::stageCenter, Set::GolemHp + (Set::ADD_HP * 5.0f), Set::ATK + (Set::ADD_ATK * 5.0f), 30),
};
WaveEnemySet waveEnemySet6[] = {
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 6.0f), Set::ATK + (Set::ADD_ATK * 6.0f), 10),
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 6.0f), Set::ATK + (Set::ADD_ATK * 6.0f), 10),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(2), Set::HP + (Set::ADD_HP * 6.0f), Set::ATK + (Set::ADD_ATK * 6.0f), 10),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(3), Set::HP + (Set::ADD_HP * 6.0f), Set::ATK + (Set::ADD_ATK * 6.0f), 10),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 6.0f), Set::ATK + (Set::ADD_ATK * 6.0f), 10),
};
WaveEnemySet waveEnemySet7[] = {
    WaveEnemySet(0.5f, "EnemyAI_3", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 7.0f), Set::ATK + (Set::ADD_ATK * 7.0f), 12),
    WaveEnemySet(0.5f, "EnemyAI_3", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 7.0f), Set::ATK + (Set::ADD_ATK * 7.0f), 12),
    WaveEnemySet(0.5f, "EnemyAI_3", ::GetGatewayPosition(2), Set::HP + (Set::ADD_HP * 7.0f), Set::ATK + (Set::ADD_ATK * 7.0f), 12),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(3), Set::HP + (Set::ADD_HP * 7.0f), Set::ATK + (Set::ADD_ATK * 7.0f), 12),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 7.0f), Set::ATK + (Set::ADD_ATK * 7.0f), 12),
};
WaveEnemySet waveEnemySet8[] = {
    WaveEnemySet(0.5f, "EnemyAI_2", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 8.0f), Set::ATK + (Set::ADD_ATK * 8.0f), 12),
    WaveEnemySet(0.5f, "EnemyAI_2", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 8.0f), Set::ATK + (Set::ADD_ATK * 8.0f), 12),
    WaveEnemySet(0.5f, "EnemyAI_2", ::GetGatewayPosition(2), Set::HP + (Set::ADD_HP * 8.0f), Set::ATK + (Set::ADD_ATK * 8.0f), 12),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(3), Set::HP + (Set::ADD_HP * 8.0f), Set::ATK + (Set::ADD_ATK * 8.0f), 12),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 8.0f), Set::ATK + (Set::ADD_ATK * 8.0f), 12),
};
WaveEnemySet waveEnemySet9[] = {
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 9.0f), Set::ATK + (Set::ADD_ATK * 9.0f), 13),
    WaveEnemySet(0.5f, "EnemyAI_1", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 9.0f), Set::ATK + (Set::ADD_ATK * 9.0f), 13),
    WaveEnemySet(0.5f, "EnemyAI_2", ::GetGatewayPosition(2), Set::HP + (Set::ADD_HP * 9.0f), Set::ATK + (Set::ADD_ATK * 9.0f), 13),
    WaveEnemySet(0.5f, "EnemyAI_2", ::GetGatewayPosition(3), Set::HP + (Set::ADD_HP * 9.0f), Set::ATK + (Set::ADD_ATK * 9.0f), 13),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(0), Set::HP + (Set::ADD_HP * 9.0f), Set::ATK + (Set::ADD_ATK * 9.0f), 13),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(1), Set::HP + (Set::ADD_HP * 9.0f), Set::ATK + (Set::ADD_ATK * 9.0f), 13),
};
WaveEnemySet waveEnemySet10[] = {
    WaveEnemySet(0.5f, "EnemyGolem", SceneGame::stageCenter, Set::GolemHp + (Set::ADD_HP * 10.0f), Set::ATK + (Set::ADD_ATK * 10.0f), 40),
    WaveEnemySet(2.5f, "EnemyGolem", SceneGame::stageCenter, Set::GolemHp + (Set::ADD_HP * 10.0f), Set::ATK + (Set::ADD_ATK * 10.0f), 40),
};
#pragma endregion


// ウェーブ(※ウェーブ配列に格納する)
// 引数：ウェーブ名・ウェーブについての備考・WaveEnemySet配列の要素数・WaveEnemySet配列の先頭アドレス
#pragma region Wave
#if _DEBUG
Wave waveTest_ = { "WaveTest", "Test", std::size(waveEnemySetTest),  waveEnemySetTest };
#endif
Wave wave0_  = { "Wave0",  "None",    std::size(waveEnemySet0),  waveEnemySet0  };
Wave wave1_  = { "Wave1",  "None",    std::size(waveEnemySet1),  waveEnemySet1  };
Wave wave2_  = { "Wave2",  "None",    std::size(waveEnemySet2),  waveEnemySet2  };
Wave wave3_  = { "Wave3",  "None",    std::size(waveEnemySet3),  waveEnemySet3  };
Wave wave4_  = { "Wave4",  "None",    std::size(waveEnemySet4),  waveEnemySet4  };
Wave wave5_  = { "Wave5",  "None",    std::size(waveEnemySet5),  waveEnemySet5  };
Wave wave6_  = { "Wave6",  "None",    std::size(waveEnemySet6),  waveEnemySet6  };
Wave wave7_  = { "Wave7",  "None",    std::size(waveEnemySet7),  waveEnemySet7  };
Wave wave8_  = { "Wave8",  "None",    std::size(waveEnemySet8),  waveEnemySet8  };
Wave wave9_  = { "Wave9",  "None",    std::size(waveEnemySet9),  waveEnemySet9  };
Wave wave10_ = { "Wave10", "VsGolem", std::size(waveEnemySet10), waveEnemySet10 };
#pragma endregion


// ウェーブ配列(※上から順にウェーブが実行される)
Wave waves_[] = {
#if _DEBUG
    //waveTest_,
#endif
    wave0_,
    wave1_,
    wave2_,
    wave3_,
    wave4_,
    wave5_,
    wave6_,
    wave7_,
    wave8_,
    wave9_,
    wave10_,
};


// ウェーブ初期化
void WaveManager::InitWave(const int waveIndex)
{
    // ウェーブの総数とウェーブを取得
    waveParent_ = { std::size(waves_), waves_ };

    // ウェーブタイマー初期化
    waveTimer_  = 0.0f;

    // 現在のウェーブ番号初期化
    {
#ifdef _DEBUG
        // 指定の番号のウェーブから開始できるようにする
        currentWaveIndex_ = (waveIndex > -1) ? waveIndex : 0;
#else
        // 現在のウェーブ番号をリセット
        currentWaveIndex_ = 0;
#endif
        // ウェーブ番号を超過している場合はエンドレスウェーブにする
        const int waveIndexEnd = GetWaveIndexEnd();
        if (currentWaveIndex_ > waveIndexEnd) endlessWaveFlag_ = true;
    }

    // 現在のウェーブが出現させる敵の総数を残りの敵カウンターに代入
    remainingEnemyCounter_ = waveParent_.children_[currentWaveIndex_].spawnEnemyCount_;

    // 休憩時間フラグリセット
    breakTimeFlag_ = false;

    // すべてのウェーブの敵の生成フラグをリセット
    ResetWaveEnemySpawnFlag();

    endlessWaveHp_  = EndlessSet::HP;
    endlessGolemHp_ = EndlessSet::GOLEM_HP;
    endlessWaveAtk_ = EndlessSet::ATK;
    endlessWaveExp_ = EndlessSet::EXP;
}


// ウェーブ更新
void WaveManager::UpdateWave(const float elapsedTime)
{
    // タイマー更新
    waveTimer_ = (std::min)(FLT_MAX, waveTimer_ + elapsedTime);

    // 休憩中のとき
    if (true == breakTimeFlag_)
    {
        // 休憩時間が終わっていなければreturn
        if (waveTimer_ < BREAK_TIME_) return;

        // 休憩時間が終わっていれば休憩終了(休憩時間フラグOFF)
        breakTimeFlag_ = false;

        // タイマーリセット
        waveTimer_ = 0.0f;

        // 一度returnする
        return;
    }

    // エンドレスフラグが立っていればエンドレスで敵を生成する
    if (true == endlessWaveFlag_)
    {
        if (waveTimer_ >= ENDLESS_WAVE_CREATE_TIME_ &&
            endlessWaveCreateCount_ > 0)
        {
            --endlessWaveCreateCount_;

            WaveEnemySet waveEnemy = {};

            // エンドレスウェーブカウンターが５ならゴーレム生成
            if (endlessWaveCounter_ == 5)
            {
                waveEnemy.name_ = "EnemyGolem";

                // ステージの中心に生成位置を設定
                waveEnemy.pos_ = SceneGame::stageCenter;

                waveEnemy.hp_ = endlessGolemHp_;
            }
            else
            {
                // 敵の種類をランダムで決める
                const int enemyNameRandom = ::RandInt(0, 2);
                switch (enemyNameRandom)
                {
                case 0: waveEnemy.name_ = "EnemyAI_1"; break;
                case 1: waveEnemy.name_ = "EnemyAI_2"; break;
                case 2: waveEnemy.name_ = "EnemyAI_3"; break;
                }

                // ランダムなゲート位置を生成位置に設定
                waveEnemy.pos_ = ::GetGatewayPosition(-1);

                waveEnemy.hp_ = endlessWaveHp_;
            }

            waveEnemy.atk_      = endlessWaveAtk_;
            waveEnemy.dropExp_  = endlessWaveExp_;

            // 敵生成
            SpawnEnemy(&waveEnemy);

            // ウェーブタイマーリセット
            waveTimer_ = 0.0f;
        }
    }
    else
    {
        // 敵生成
        {
            // 現在のウェーブを取得
            const Wave currentWave = waveParent_.children_[currentWaveIndex_];
            for (size_t i = 0; i < currentWave.spawnEnemyCount_; ++i)
            {
                // 出現させる敵のパラメータを取得
                WaveEnemySet* waveEnemy = &currentWave.waveEnemySets_[i];

                // 生成済みならcontinue
                if (true == waveEnemy->isSpawned_) continue;

                // 生成時間を過ぎたら生成する
                if (waveTimer_ >= waveEnemy->spawnTime_)
                {
                    SpawnEnemy(waveEnemy);  // 敵を生成
                    waveTimer_ = 0.0f;      // タイマーリセット
                }
            }
        }
    }

    // ウェーブ中のすべての敵が倒されたら次のウェーブに移る
    if (remainingEnemyCounter_ <= 0)
    {
        // 現在のウェーブ番号を加算
        ++currentWaveIndex_;

        // タイマーリセット
        waveTimer_ = 0.0f;

        // 休憩時間に入る(休憩フラグON)
        breakTimeFlag_ = true;

        // 現在のウェーブが最後のウェーブならエンドレスウェーブを回す
        const int waveIndexEnd = GetWaveIndexEnd();
        if (currentWaveIndex_ > waveIndexEnd)
        {
            // エンドレスウェーブフラグが立っていなければ立てる
            if (false == endlessWaveFlag_) { endlessWaveFlag_ = true; }

            // エンドレスウェーブカウンター加算
            ++endlessWaveCounter_;

            // エンドレスウェーブカウンターが５を超えたら１にリセットする
            if (endlessWaveCounter_ > 5) { endlessWaveCounter_ = 1; }

            // 体力・攻撃力増加
            endlessWaveHp_  += Set::ADD_HP;
            endlessGolemHp_ += Set::ADD_HP;
            endlessWaveAtk_ += Set::ADD_ATK;

            // エンドレスウェーブカウンターが５のときはゴーレムを生成するようにする
            if (endlessWaveCounter_ == 5)
            {
                // ゴーレム1~3体生成するようにする
                endlessWaveCreateCount_ = ::RandInt(1, 3);
            }
            else
            {
                // 7~9の敵を生成する
                endlessWaveCreateCount_ = ::RandInt(7, 9);
            }

            // エンドレスウェーブで生成する敵の数を残りの敵カウンターに代入
            remainingEnemyCounter_ = endlessWaveCreateCount_;
        }
        else
        {
            // ウェーブの敵の総数を残りの敵カウンターに代入
            remainingEnemyCounter_ = waveParent_.children_[currentWaveIndex_].spawnEnemyCount_;
        }

        // waveの表記を出す
        UserInterface::Instance().SetWaveSlideSprite();
        // waveの効果音
        AudioManager::Instance().PlaySE(SE::WaveBegin);

    }

}


// 敵生成
void WaveManager::SpawnEnemy(WaveEnemySet* waveEnemy)
{
    EnemyManager& enmManager = EnemyManager::Instance();

    Enemy* enemy = nullptr;

    // 名前から敵の種類を判別して生成する
    {
        const std::string& enemyName = waveEnemy->name_;
        if      ("EnemyAI_1"  == enemyName) { enemy = new EnemyAI_1();   }
        else if ("EnemyAI_2"  == enemyName) { enemy = new EnemyAI_2();   }
        else if ("EnemyAI_3"  == enemyName) { enemy = new EnemyAI_3();   }
        else if ("EnemyGolem" == enemyName) { enemy = new EnemyGolem();  }
        else                                { assert("Not Found Enemy"); }
    }

    // パラメータ設定
    {
        // エネミー初期化
        enemy->Initialize();

        enemy->SetPosition(waveEnemy->pos_);            // 位置設定
        enemy->SetHealth(waveEnemy->hp_);               // 体力設定
        enemy->SetMaxHealth(waveEnemy->hp_);            // 最大体力設定
        enemy->SetAttack(waveEnemy->atk_);              // 攻撃力設定
        enemy->SetDropExpCount(waveEnemy->dropExp_);    // ドロップ経験値数設定

        // ウェーブから生まれた敵である(ウェーブ敵フラグON)
        enemy->SetIsWaveEnemy(true);
    }

    enmManager.Register(enemy); // エネミーマネージャーに登録

    // 生成した(生成フラグON)
    waveEnemy->isSpawned_ = true;
}

// すべてのウェーブの敵の生成フラグをリセット
void WaveManager::ResetWaveEnemySpawnFlag()
{
    const size_t waveCount = waveParent_.waveCount_;
    for (size_t waveIndex = 0; waveIndex < waveCount; ++waveIndex)
    {
        // ウェーブ取得
        Wave* wave = &waveParent_.children_[waveIndex];

        const size_t enemyCount = wave->spawnEnemyCount_;
        for (size_t enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
        {
            // ウェーブエネミー取得
            WaveEnemySet* waveEnemy = &wave->waveEnemySets_[enemyIndex];

            // 生成フラグをリセット
            waveEnemy->isSpawned_ = false;
        }
    }
}

void WaveManager::DrawDebug()
{
#ifdef USE_IMGUI

    if (true == endlessWaveFlag_)
    {
        if (ImGui::BeginMenu("EndlessWave"))
        {
            // エンドレスウェーブカウンター
            int endlessWaveCounter = endlessWaveCounter_;
            ImGui::InputInt("endlessWaveCounter", &endlessWaveCounter);

            // エンドレスウェーブの生成する敵の数
            int endlessWaveCreateCount = endlessWaveCreateCount_;
            ImGui::InputInt("endlessWaveCreateCount", &endlessWaveCreateCount);

            ImGui::EndMenu();
        }
    }
    // 現在のウェーブ情報をデバッグ表示
    else if (ImGui::BeginMenu("Wave"))
    {
        const Wave& currentWave = waveParent_.children_[currentWaveIndex_];

        // 現在のウェーブの名前
        std::string currentWaveName = currentWave.name_.c_str();
        ImGui::InputText("CurrentWaveName", &currentWaveName.at(0), 1);

        // 現在のウェーブについての備考
        std::string currentWaveNote = currentWave.note_.c_str();
        ImGui::InputText("CurrentWaveNote", &currentWaveNote.at(0), 1);

        // 現在のウェーブ番号
        int currentWaveindex = currentWaveIndex_;
        ImGui::InputInt("CurrentWaveIndex", &currentWaveindex);

        // 残りの敵の数
        int remainingEnemyCount = remainingEnemyCounter_;
        ImGui::InputInt("RemainingEnemyCount", &remainingEnemyCount);

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif
}
