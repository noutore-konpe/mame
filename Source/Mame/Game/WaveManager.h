#pragma once

#include <DirectXMath.h>
#include <string>

#include "../../Taki174/Common.h"
#include "../Scene/SceneGame.h"


// 番号に対応するゲートの位置を返す
inline DirectX::XMFLOAT3 GetGatewayPosition(/*NoConst*/ int gatewayIndex)
{
    static constexpr int gatewayCount = 10; // ゲートの数

    // ゲート番号超過修正
    if (gatewayIndex > gatewayCount) gatewayIndex = gatewayCount;

    // 最初の一回だけ位置テーブルを作成するようにする
    // ※すべてのゲート位置をひとつひとつ手動で設定するのが
    // 　面倒なのでfor文でゲート方向を向くY回転値のテーブルを
    // 　作成してゲート位置を設定する
    static float rotationY_table[gatewayCount] = {};    // Y回転値テーブル(static)
    static bool  createRotationY_TableFlag     = false; // Y回転値テーブル作成フラグ(static)
    if (false == createRotationY_TableFlag)
    {
        // 360度をゲート数分に等分したときの角度
        static constexpr float angle = 360.0f / static_cast<float>(gatewayCount);

        // Y回転値テーブルを作成
        for (int i = 0; i < gatewayCount; ++i)
        {
            rotationY_table[i] = ::ToRadian(angle * static_cast<float>(i));
        }

        // Y回転値テーブルを作成した(フラグON)
        createRotationY_TableFlag = true;
    }

    // Y回転値を取得
    const int   rotationY_index = (gatewayIndex > -1) ? gatewayIndex : ::RandInt(0, gatewayCount);
    const float rotationY       = rotationY_table[rotationY_index];

    // ステージの中心からゲートの奥ぐらいまでのだいたいの半径(距離)
    static constexpr float RADIUS = 35.0f;

    // ゲートのXZ位置設定
    DirectX::XMFLOAT3 gatewayPos = {};
    gatewayPos.x = SceneGame::stageCenter.x + ::sinf(rotationY) * RADIUS;
    gatewayPos.z = SceneGame::stageCenter.z + ::cosf(rotationY) * RADIUS;

    // ゲートのXZ位置を返す
    return gatewayPos;
}


// ウェーブ関係の構造体
#pragma region Wave Struct
// ウェーブエネミー構造体
struct WaveEnemySet
{
    // 配列を作るときにどの値が何の値を指しているのか
    // わかりやすいようにコンストラクタを作成しておく
    WaveEnemySet(
        const float              spawnTime,
        const std::string&       name,
        const DirectX::XMFLOAT3& pos,
        const float              hp,
        const int                dropExp)
        : spawnTime_(spawnTime)
        , name_(name)
        , pos_(pos)
        , hp_(hp)
        , dropExp_(dropExp)
        , isSpawned_(false) // 生成フラグはデフォルトでfalse
    {}

    float             spawnTime_ = 0.0f;    // 出現時間
    std::string       name_      = "";      // 名前(敵生成時の敵クラス検索用)
    DirectX::XMFLOAT3 pos_       = {};      // 出現位置
    float             hp_        = 0.0f;    // 体力
    int               dropExp_   = 0;       // 落とす経験値の数
    bool              isSpawned_ = false;   // 生成したか(１回だけ生成するためのフラグ)

};

// ウェーブ構造体
struct Wave
{
    std::string name_            = "";       // ウェーブの名前
    std::string note_            = "";       // ウェーブについての備考(ウェーブについて詳しい説明などを殴り書く用)
    int         spawnEnemyCount_ = 0;        // 出現させる敵の総数(ウェーブエネミー配列の要素数)
    WaveEnemySet*  waveEnemy_       = nullptr;  // 出現させる敵の構造体ポインタ(パラメータ)

};

// ウェーブペアレント構造体
struct WaveParent
{
    int   waveCount_    = 0;        // ウェーブの総数(ウェーブ配列の要素数)
    Wave* children_     = nullptr;  // ウェーブ配列のポインタ
};
#pragma endregion


// ウェーブ設定
// ※要素数の取得や変更などを行うときにクラス内で定義していると面倒なので外で定義
#pragma region Wave Setting
// WaveEnemySet
extern WaveEnemySet waveEnemy1[];
// Wave
extern Wave wave1_;
// Wave Array
extern Wave waves_[];
#pragma endregion


class WaveManager
{
private:
    WaveManager()  = default;
    ~WaveManager() = default;

public:
    static WaveManager& Instance()
    {
        static WaveManager instance;
        return instance;
    }

    void InitWave(const int waveindex = -1);    // ウェーブ初期化(引数の番号で指定のウェーブから始める(※デバッグ時のみ機能))
    void UpdateWave(const float elapsedTime);   // ウェーブ更新
    void DrawDebug();

    void ResetWaveEnemySpawnFlag();             // すべてのウェーブの敵生成フラグをリセット

    void SpawnEnemy(WaveEnemySet* waveEnemy);      // 敵生成

    // 残りの敵カウンターを１減らす
    void ReduceRemainingEnemyCounter() { --remainingEnemyCounter_; }

public: // Get・Set Function

    // 現在のウェーブ番号を取得
    const int GetCurrentWaveIndex() const { return currentWaveIndex_; }

    // ウェーブ番号の末尾を取得
    const int GetWaveIndexEnd() const { return (waveParent_.waveCount_ - 1); }

    // 現在のウェーブの名前を取得
    const std::string& GetCurrentWaveName() const { return waveParent_.children_->name_; }
    // 現在のウェーブが出現させる敵の総数を取得
    const int GetCurrentWaveEnemyCount() const { return waveParent_.children_->spawnEnemyCount_; }

private:
    static constexpr float BREAK_TIME = 5.0f; // 現在のウェーブから次のウェーブに移るまでの休憩時間

private:
    WaveParent  waveParent_             = {};       // ウェーブの親(ウェーブを管理)
    float       waveTimer_              = 0.0f;     // ウェーブタイマー
    int         currentWaveIndex_       = 0;        // 現在のウェーブ番号
    int         remainingEnemyCounter_  = 0;        // 残りの敵の数
    bool        breakTimeFlag_          = false;    // 休憩フラグ
};