#include "Boss.h"
#include "../Mame/Graphics/Graphics.h"
#include "../Mame/Graphics/Camera.h"
#include "../Mame/Input/Input.h"
#include "../Mame/AudioManager.h"

#include "BossStateDerived.h"

int Boss::nameNum = 0;

// コンストラクタ
Boss::Boss()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/bossall.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/temporary/assets_block.fbx", true);
    DirectX::XMFLOAT3 pos1 = model->GetTransform()->GetPosition();


    // ステートマシーンをセット
    stateMachine.reset(new StateMachine);

    GetStateMachine()->RegisterState(new BOSS::IdleState(this));
    GetStateMachine()->RegisterState(new BOSS::FindState(this));
    GetStateMachine()->RegisterState(new BOSS::TurnState(this));
    GetStateMachine()->RegisterState(new BOSS::AttackState(this));
    GetStateMachine()->RegisterState(new BOSS::RecoilState(this));
    GetStateMachine()->RegisterState(new BOSS::DamageState(this));
    GetStateMachine()->RegisterState(new BOSS::CryState(this));
    GetStateMachine()->RegisterState(new BOSS::WalkState(this));

    GetStateMachine()->SetState(static_cast<int>(BOSS::STATE::Walk));


    // imgui名前かぶり起きないように...
    name = "Boss" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}


Boss::Boss(const char* filename)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), filename, true);

    // imgui名前かぶり起きないように...
    name = "Boss" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}


// デストラクタ
Boss::~Boss()
{
}


// 初期化
void Boss::Initialize()
{
    GetTransform()->SetPosition(DirectX::XMFLOAT3(12.4f, 2.2f, 10.0f));

    // ※ここの初期回転値によって振り向きの方向が変わるので注意
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, ToRadian(270.0f), 0.0f, 0.0f));

    // TODO: ボスの当たり判定設定
    const DirectX::XMFLOAT3 min = { -0.6f, -0.0f, -0.6f };  // min設定
    const DirectX::XMFLOAT3 max = { +0.6f, +2.0f, +0.6f };  // max設定
    SetAABB(min, max);                                      // minとmaxの再設定（ジオメトリックプリミティブの再生成も行っている）
    UpdateAABB();                                           // minとmaxを現在の位置に更新する

    turnSpeed_ = ToRadian(180.0f);  // 旋回速度の設定

    characterType_ = CharacterType::Enemy;

    // 無敵状態にする
    isInvincible = true;
}


// 終了化
void Boss::Finalize()
{
}


// Updateの前に呼ばれる
void Boss::Begin()
{
}


// 更新処理
void Boss::Update(const float& elapsedTime)
{
    //GamePad& gamePad = Input::Instance().GetGamePad();

    //float ax = gamePad.GetAxisRX();

    //float speed = 0.001f;
    //speed *= ax;
    //DirectX::XMFLOAT3 pos = model->GetTransform()->GetPosition();
    //pos.x += speed;
    //model->GetTransform()->SetPosition(pos);

    if (stateMachine) GetStateMachine()->Update(elapsedTime); // ステート更新処理

    UpdateAABB();                       // AABBの更新処理

    UpdateVelocity(elapsedTime);        // 速力処理更新処理

    // ひるんでいるときなどにプレイヤーと衝突してプレイヤーがダメージを食らわないように
    // 衝突判定処理はステート毎に行う
    // CollisionEnemyVsPlayer();

    UpdateInvincibleTimer(elapsedTime); // 無敵時間の更新処理

    // 無敵時間中のキャラクターの点滅(ダメージを受けたかの確認用)
    {
        if (invincibleTimer > 0.0f)
        {
            modelColor.w = (static_cast<int>(invincibleTimer * 100.0f) & 0x04) ? 0.7f : 0.0f;
        }
        else
        {
            modelColor.w = 1.0f;
        }
    }

    UpdateAnimation(elapsedTime);       // アニメーション更新
}

// Updateの後に呼ばれる
void Boss::End()
{
}

// 描画処理
void Boss::Render(const float& elapsedTime)
{
    Enemy::Render(elapsedTime);
}

// debug用
void Boss::DrawDebug()
{
    ImGui::Begin(GetName());

    Character::DrawDebug();

    if (stateMachine) GetStateMachine()->DrawDebug();

    ImGui::End();
}


void Boss::OnDead()
{
    stateMachine->ChangeState(static_cast<int>(BOSS::STATE::Damage));
    stateMachine->GetCurrentState()->SetTimer(1.0f);

    // 自分を消去
    //Destroy();
}


void Boss::OnHitWall()
{
    Camera::Instance().PlayShake(ShakeType::HorizontalShake);
    SetMoveDirectionX(-GetMoveDirectionX());    // 移動方向を反転
    GetStateMachine()->ChangeState(static_cast<int>(BOSS::STATE::Recoil));  // 反動ステートへ遷移

    
    AudioManager& audioManager = AudioManager::Instance();
    audioManager.StopSE(SE::Boss_Run);              // 走行SE停止
    audioManager.PlaySE(SE::Boss_HitWall, false);   // 壁衝突SE再生
}

void Boss::OnDamaged()
{
    stateMachine->ChangeState(static_cast<int>(BOSS::STATE::Damage));
}
