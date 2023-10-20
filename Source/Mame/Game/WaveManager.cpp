#include "WaveManager.h"

#include "EnemyManager.h"
#include "EnemyAI_1.h"
#include "EnemyAI_2.h"
#include "EnemyAI_3.h"
#include "EnemyGolem.h"

#include "UserInterface.h"

// ウェーブ設定
// ※要素数の取得や変更などを行うときにクラス内で定義していると面倒なので外で定義

// ウェーブエネミーセット(※ウェーブに格納する)
#pragma region WaveEnemySet
WaveEnemySet waveEnemySet0[] = {
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(0), 1.0f, 5),
};
WaveEnemySet waveEnemySet1[] = {
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(0), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(1), 1.0f, 5),
};
WaveEnemySet waveEnemySet2[] = {
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(0), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(1), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(2), 1.0f, 5),
};
WaveEnemySet waveEnemySet3[] = {
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(0), 1.0f, 5),
};
WaveEnemySet waveEnemySet4[] = {
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(0), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(1), 1.0f, 5),
};
WaveEnemySet waveEnemySet5[] = {
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(0), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(1), 1.0f, 5),
};
WaveEnemySet waveEnemySet6[] = {
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(0), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(1), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(2), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(3), 1.0f, 5),
};
WaveEnemySet waveEnemySet7[] = {
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(0), 1.0f, 5),
};
WaveEnemySet waveEnemySet8[] = {
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(0), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(1), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(2), 1.0f, 5),
};
WaveEnemySet waveEnemySet9[] = {
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(0), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_2", ::GetGatewayPosition(1), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(2), 1.0f, 5),
    WaveEnemySet(1.0f, "EnemyAI_3", ::GetGatewayPosition(3), 1.0f, 5),
};
WaveEnemySet waveEnemySet10[] = {
    WaveEnemySet(1.0f, "EnemyGolem", SceneGame::stageCenter, 1.0f, 5),
};
#pragma endregion


// ウェーブ(※ウェーブ配列に格納する)
// 引数：ウェーブ名・ウェーブについての備考・WaveEnemySet配列の要素数・WaveEnemySet配列の先頭アドレス
#pragma region Wave
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
        // ウェーブ番号超過修正
        const int waveIndexEnd = GetWaveIndexEnd();
        if (currentWaveIndex_ > waveIndexEnd) currentWaveIndex_ = waveIndexEnd;
    }

    // 現在のウェーブが出現させる敵の総数を残りの敵カウンターに代入
    remainingEnemyCounter_ = waveParent_.children_[currentWaveIndex_].spawnEnemyCount_;

    // 休憩時間フラグリセット
    breakTimeFlag_ = false;

    // すべてのウェーブの敵の生成フラグをリセット
    ResetWaveEnemySpawnFlag();
}


// ウェーブ更新
void WaveManager::UpdateWave(const float elapsedTime)
{
    // if (ゲームクリアフラグが立っている) return;

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

    // ウェーブ中のすべての敵が倒されたら次のウェーブに移る
    if (remainingEnemyCounter_ <= 0)
    {
        ++currentWaveIndex_; // 現在のウェーブ番号を加算

        // 現在のウェーブが最後のウェーブならゲームクリアにする
        const int waveIndexEnd = GetWaveIndexEnd();
        if (currentWaveIndex_ > waveIndexEnd)
        {
            // ゲームクリア
            //assert("Game Clear");
        }
        else
        {
            // タイマーリセット
            waveTimer_ = 0.0f;

            // 敵の総数を残りの敵カウンターに代入
            remainingEnemyCounter_ = waveParent_.children_[currentWaveIndex_].spawnEnemyCount_;

            // 休憩時間に入る(休憩フラグON)
            breakTimeFlag_ = true;

            // waveの表記を出す
            UserInterface::Instance().SetWaveSlideSprite();
        }

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

    // 現在のウェーブ情報をデバッグ表示
    if (ImGui::BeginMenu("Wave"))
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
