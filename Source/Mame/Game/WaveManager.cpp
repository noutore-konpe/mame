#include "WaveManager.h"
#include "EnemyManager.h"
#include "EnemyAI_1.h"
#include "EnemyAI_2.h"
#include "EnemyAI_3.h"
#include "EnemyGolem.h"

// ウェーブ設定
//（※要素数の取得や変更などを行うときにクラス内で定義していると面倒なので外で定義）
#pragma region Wave Setting
// ウェーブエネミーセット(※ウェーブに格納する)
WaveEnemySet waveEnemySet0[] = {
    WaveEnemySet(1.0f, "EnemyAI_1", ::GetGatewayPosition(0), 0.0f, 0),
};

// ウェーブ(※ウェーブ配列に格納する)
// 引数：ウェーブ名・ウェーブについての備考・WaveEnemySet配列の要素数・WaveEnemySet配列の先頭アドレス
Wave wave0_ = { "Wave0", u8"特になし", static_cast<int>(std::size(waveEnemySet0)), waveEnemySet0};

// ウェーブ配列(※上から順にウェーブが実行される)
Wave waves_[] = {
    wave0_,
};
#pragma endregion


void WaveManager::InitWave(const int waveIndex)
{
    // ウェーブの総数とウェーブを取得
    waveParent_ = { static_cast<int>(std::size(waves_)), waves_ };

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

void WaveManager::UpdateWave(const float elapsedTime)
{
    // if (ゲームクリアフラグが立っている) return;

    // タイマー更新
    waveTimer_ += elapsedTime;

    // 休憩中のとき
    if (true == breakTimeFlag_)
    {
        // 休憩時間が終わっていなければreturn
        if (waveTimer_ < BREAK_TIME) return;

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
        for (int i = 0; i < currentWave.spawnEnemyCount_; ++i)
        {
            // 出現させる敵のパラメータを取得
            WaveEnemySet* waveEnemy = &currentWave.waveEnemy_[i];

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
            assert("Game Clear");
        }
        else
        {
            // タイマーリセット
            waveTimer_ = 0.0f;

            // 敵の総数を残りの敵カウンターに代入
            remainingEnemyCounter_ = waveParent_.children_[currentWaveIndex_].spawnEnemyCount_;

            // 休憩時間に入る(休憩フラグON)
            breakTimeFlag_ = true;
        }

    }

}

// 敵生成
void WaveManager::SpawnEnemy(WaveEnemySet* waveEnemy)
{
    EnemyManager& enmManager = EnemyManager::Instance();

    // 名前から敵の種類を判別して生成する
    Enemy* enemy = nullptr;
    const std::string& enemyName = waveEnemy->name_;
    if      ("EnemyAI_1"  == enemyName) { enemy = new EnemyAI_1();   }
    else if ("EnemyAI_2"  == enemyName) { enemy = new EnemyAI_2();   }
    else if ("EnemyAI_3"  == enemyName) { enemy = new EnemyAI_3();   }
    else if ("EnemyGolem" == enemyName) { enemy = new EnemyGolem();  }
    else                                { assert("Not Found Enemy"); }

    // パラメータを設定
    {
        enemy->SetPosition(waveEnemy->pos_);            // 位置設定
        enemy->SetHealth(waveEnemy->hp_);               // 体力設定
        enemy->SetMaxHealth(waveEnemy->hp_);            // 最大体力設定
        enemy->SetDropExpCount(waveEnemy->dropExp_);    // ドロップ経験値数設定
    }

    enmManager.Register(enemy); // エネミーマネージャーに登録

    // 生成した(生成フラグON)
    waveEnemy->isSpawned_ = true;
}

// すべてのウェーブの敵の生成フラグをリセット
void WaveManager::ResetWaveEnemySpawnFlag()
{
    const int waveCount = waveParent_.waveCount_;
    for (int waveIndex = 0; waveIndex < waveCount; ++waveIndex)
    {
        Wave* wave = &waveParent_.children_[waveIndex];
        const int enemyCount = wave->spawnEnemyCount_;
        for (int enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
        {
            WaveEnemySet* waveEnemy = &wave->waveEnemy_[enemyIndex];
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
