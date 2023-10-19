#include "WaveManager.h"

#include "EnemyManager.h"
#include "EnemyAI_1.h"
#include "EnemyAI_2.h"
#include "EnemyAI_3.h"
#include "EnemyGolem.h"

// �E�F�[�u�ݒ�
// ���v�f���̎擾��ύX�Ȃǂ��s���Ƃ��ɃN���X���Œ�`���Ă���Ɩʓ|�Ȃ̂ŊO�Œ�`

// �E�F�[�u�G�l�~�[�Z�b�g(���E�F�[�u�Ɋi�[����)
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


// �E�F�[�u(���E�F�[�u�z��Ɋi�[����)
// �����F�E�F�[�u���E�E�F�[�u�ɂ��Ă̔��l�EWaveEnemySet�z��̗v�f���EWaveEnemySet�z��̐擪�A�h���X
#pragma region Wave
Wave wave0_  = { "Wave0",  "None",    static_cast<int>(std::size(waveEnemySet0)),  waveEnemySet0  };
Wave wave1_  = { "Wave1",  "None",    static_cast<int>(std::size(waveEnemySet1)),  waveEnemySet1  };
Wave wave2_  = { "Wave2",  "None",    static_cast<int>(std::size(waveEnemySet2)),  waveEnemySet2  };
Wave wave3_  = { "Wave3",  "None",    static_cast<int>(std::size(waveEnemySet3)),  waveEnemySet3  };
Wave wave4_  = { "Wave4",  "None",    static_cast<int>(std::size(waveEnemySet4)),  waveEnemySet4  };
Wave wave5_  = { "Wave5",  "None",    static_cast<int>(std::size(waveEnemySet5)),  waveEnemySet5  };
Wave wave6_  = { "Wave6",  "None",    static_cast<int>(std::size(waveEnemySet6)),  waveEnemySet6  };
Wave wave7_  = { "Wave7",  "None",    static_cast<int>(std::size(waveEnemySet7)),  waveEnemySet7  };
Wave wave8_  = { "Wave8",  "None",    static_cast<int>(std::size(waveEnemySet8)),  waveEnemySet8  };
Wave wave9_  = { "Wave9",  "None",    static_cast<int>(std::size(waveEnemySet9)),  waveEnemySet9  };
Wave wave10_ = { "Wave10", "VsGolem", static_cast<int>(std::size(waveEnemySet10)), waveEnemySet10 };
#pragma endregion


// �E�F�[�u�z��(���ォ�珇�ɃE�F�[�u�����s�����)
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


// �E�F�[�u������
void WaveManager::InitWave(const int waveIndex)
{
    // �E�F�[�u�̑����ƃE�F�[�u���擾
    waveParent_ = { static_cast<int>(std::size(waves_)), waves_ };

    // �E�F�[�u�^�C�}�[������
    waveTimer_  = 0.0f;

    // ���݂̃E�F�[�u�ԍ�������
    {
#ifdef _DEBUG
        // �w��̔ԍ��̃E�F�[�u����J�n�ł���悤�ɂ���
        currentWaveIndex_ = (waveIndex > -1) ? waveIndex : 0;
#else
        // ���݂̃E�F�[�u�ԍ������Z�b�g
        currentWaveIndex_ = 0;
#endif
        // �E�F�[�u�ԍ����ߏC��
        const int waveIndexEnd = GetWaveIndexEnd();
        if (currentWaveIndex_ > waveIndexEnd) currentWaveIndex_ = waveIndexEnd;
    }

    // ���݂̃E�F�[�u���o��������G�̑������c��̓G�J�E���^�[�ɑ��
    remainingEnemyCounter_ = waveParent_.children_[currentWaveIndex_].spawnEnemyCount_;

    // �x�e���ԃt���O���Z�b�g
    breakTimeFlag_ = false;

    // ���ׂẴE�F�[�u�̓G�̐����t���O�����Z�b�g
    ResetWaveEnemySpawnFlag();
}


// �E�F�[�u�X�V
void WaveManager::UpdateWave(const float elapsedTime)
{
    // if (�Q�[���N���A�t���O�������Ă���) return;

    // �^�C�}�[�X�V
    waveTimer_ = (std::min)(FLT_MAX, waveTimer_ + elapsedTime);

    // �x�e���̂Ƃ�
    if (true == breakTimeFlag_)
    {
        // �x�e���Ԃ��I����Ă��Ȃ����return
        if (waveTimer_ < BREAK_TIME_) return;

        // �x�e���Ԃ��I����Ă���΋x�e�I��(�x�e���ԃt���OOFF)
        breakTimeFlag_ = false;

        // �^�C�}�[���Z�b�g
        waveTimer_ = 0.0f;

        // ��xreturn����
        return;
    }

    // �G����
    {
        // ���݂̃E�F�[�u���擾
        const Wave currentWave = waveParent_.children_[currentWaveIndex_];
        for (int i = 0; i < currentWave.spawnEnemyCount_; ++i)
        {
            // �o��������G�̃p�����[�^���擾
            WaveEnemySet* waveEnemy = &currentWave.waveEnemy_[i];

            // �����ς݂Ȃ�continue
            if (true == waveEnemy->isSpawned_) continue;

            // �������Ԃ��߂����琶������
            if (waveTimer_ >= waveEnemy->spawnTime_)
            {
                SpawnEnemy(waveEnemy);  // �G�𐶐�
                waveTimer_ = 0.0f;      // �^�C�}�[���Z�b�g
            }
        }
    }

    // �E�F�[�u���̂��ׂĂ̓G���|���ꂽ�玟�̃E�F�[�u�Ɉڂ�
    if (remainingEnemyCounter_ <= 0)
    {
        ++currentWaveIndex_; // ���݂̃E�F�[�u�ԍ������Z

        // ���݂̃E�F�[�u���Ō�̃E�F�[�u�Ȃ�Q�[���N���A�ɂ���
        const int waveIndexEnd = GetWaveIndexEnd();
        if (currentWaveIndex_ > waveIndexEnd)
        {
            // �Q�[���N���A
            assert("Game Clear");
        }
        else
        {
            // �^�C�}�[���Z�b�g
            waveTimer_ = 0.0f;

            // �G�̑������c��̓G�J�E���^�[�ɑ��
            remainingEnemyCounter_ = waveParent_.children_[currentWaveIndex_].spawnEnemyCount_;

            // �x�e���Ԃɓ���(�x�e�t���OON)
            breakTimeFlag_ = true;
        }

    }

}


// �G����
void WaveManager::SpawnEnemy(WaveEnemySet* waveEnemy)
{
    EnemyManager& enmManager = EnemyManager::Instance();

    Enemy* enemy = nullptr;

    // ���O����G�̎�ނ𔻕ʂ��Đ�������
    {
        const std::string& enemyName = waveEnemy->name_;
        if      ("EnemyAI_1"  == enemyName) { enemy = new EnemyAI_1();   }
        else if ("EnemyAI_2"  == enemyName) { enemy = new EnemyAI_2();   }
        else if ("EnemyAI_3"  == enemyName) { enemy = new EnemyAI_3();   }
        else if ("EnemyGolem" == enemyName) { enemy = new EnemyGolem();  }
        else                                { assert("Not Found Enemy"); }
    }

    // �p�����[�^�ݒ�
    {
        // �G�l�~�[������
        enemy->Initialize();

        enemy->SetPosition(waveEnemy->pos_);            // �ʒu�ݒ�
        enemy->SetHealth(waveEnemy->hp_);               // �̗͐ݒ�
        enemy->SetMaxHealth(waveEnemy->hp_);            // �ő�̗͐ݒ�
        enemy->SetDropExpCount(waveEnemy->dropExp_);    // �h���b�v�o���l���ݒ�

        // �E�F�[�u���琶�܂ꂽ�G�ł���(�E�F�[�u�G�t���OON)
        enemy->SetIsWaveEnemy(true);
    }

    enmManager.Register(enemy); // �G�l�~�[�}�l�[�W���[�ɓo�^

    // ��������(�����t���OON)
    waveEnemy->isSpawned_ = true;
}

// ���ׂẴE�F�[�u�̓G�̐����t���O�����Z�b�g
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

    // ���݂̃E�F�[�u�����f�o�b�O�\��
    if (ImGui::BeginMenu("Wave"))
    {
        const Wave& currentWave = waveParent_.children_[currentWaveIndex_];

        // ���݂̃E�F�[�u�̖��O
        std::string currentWaveName = currentWave.name_.c_str();
        ImGui::InputText("CurrentWaveName", &currentWaveName.at(0), 1);

        // ���݂̃E�F�[�u�ɂ��Ă̔��l
        std::string currentWaveNote = currentWave.note_.c_str();
        ImGui::InputText("CurrentWaveNote", &currentWaveNote.at(0), 1);

        // ���݂̃E�F�[�u�ԍ�
        int currentWaveindex = currentWaveIndex_;
        ImGui::InputInt("CurrentWaveIndex", &currentWaveindex);

        // �c��̓G�̐�
        int remainingEnemyCount = remainingEnemyCounter_;
        ImGui::InputInt("RemainingEnemyCount", &remainingEnemyCount);

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif
}