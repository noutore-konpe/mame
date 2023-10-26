#include "WaveManager.h"

#include "EnemyManager.h"
#include "EnemyAI_1.h"
#include "EnemyAI_2.h"
#include "EnemyAI_3.h"
#include "EnemyGolem.h"

#include "UserInterface.h"

#include "../Resource/AudioManager.h"

// �E�F�[�u�ݒ�
// ���v�f���̎擾��ύX�Ȃǂ��s���Ƃ��ɃN���X���Œ�`���Ă���Ɩʓ|�Ȃ̂ŊO�Œ�`

// �E�F�[�u�G�l�~�[�Z�b�g(���E�F�[�u�Ɋi�[����)
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


// �E�F�[�u(���E�F�[�u�z��Ɋi�[����)
// �����F�E�F�[�u���E�E�F�[�u�ɂ��Ă̔��l�EWaveEnemySet�z��̗v�f���EWaveEnemySet�z��̐擪�A�h���X
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


// �E�F�[�u�z��(���ォ�珇�ɃE�F�[�u�����s�����)
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


// �E�F�[�u������
void WaveManager::InitWave(const int waveIndex)
{
    // �E�F�[�u�̑����ƃE�F�[�u���擾
    waveParent_ = { std::size(waves_), waves_ };

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
        // �E�F�[�u�ԍ��𒴉߂��Ă���ꍇ�̓G���h���X�E�F�[�u�ɂ���
        const int waveIndexEnd = GetWaveIndexEnd();
        if (currentWaveIndex_ > waveIndexEnd) endlessWaveFlag_ = true;
    }

    // ���݂̃E�F�[�u���o��������G�̑������c��̓G�J�E���^�[�ɑ��
    remainingEnemyCounter_ = waveParent_.children_[currentWaveIndex_].spawnEnemyCount_;

    // �x�e���ԃt���O���Z�b�g
    breakTimeFlag_ = false;

    // ���ׂẴE�F�[�u�̓G�̐����t���O�����Z�b�g
    ResetWaveEnemySpawnFlag();

    endlessWaveHp_  = EndlessSet::HP;
    endlessGolemHp_ = EndlessSet::GOLEM_HP;
    endlessWaveAtk_ = EndlessSet::ATK;
    endlessWaveExp_ = EndlessSet::EXP;
}


// �E�F�[�u�X�V
void WaveManager::UpdateWave(const float elapsedTime)
{
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

    // �G���h���X�t���O�������Ă���΃G���h���X�œG�𐶐�����
    if (true == endlessWaveFlag_)
    {
        if (waveTimer_ >= ENDLESS_WAVE_CREATE_TIME_ &&
            endlessWaveCreateCount_ > 0)
        {
            --endlessWaveCreateCount_;

            WaveEnemySet waveEnemy = {};

            // �G���h���X�E�F�[�u�J�E���^�[���T�Ȃ�S�[��������
            if (endlessWaveCounter_ == 5)
            {
                waveEnemy.name_ = "EnemyGolem";

                // �X�e�[�W�̒��S�ɐ����ʒu��ݒ�
                waveEnemy.pos_ = SceneGame::stageCenter;

                waveEnemy.hp_ = endlessGolemHp_;
            }
            else
            {
                // �G�̎�ނ������_���Ō��߂�
                const int enemyNameRandom = ::RandInt(0, 2);
                switch (enemyNameRandom)
                {
                case 0: waveEnemy.name_ = "EnemyAI_1"; break;
                case 1: waveEnemy.name_ = "EnemyAI_2"; break;
                case 2: waveEnemy.name_ = "EnemyAI_3"; break;
                }

                // �����_���ȃQ�[�g�ʒu�𐶐��ʒu�ɐݒ�
                waveEnemy.pos_ = ::GetGatewayPosition(-1);

                waveEnemy.hp_ = endlessWaveHp_;
            }

            waveEnemy.atk_      = endlessWaveAtk_;
            waveEnemy.dropExp_  = endlessWaveExp_;

            // �G����
            SpawnEnemy(&waveEnemy);

            // �E�F�[�u�^�C�}�[���Z�b�g
            waveTimer_ = 0.0f;
        }
    }
    else
    {
        // �G����
        {
            // ���݂̃E�F�[�u���擾
            const Wave currentWave = waveParent_.children_[currentWaveIndex_];
            for (size_t i = 0; i < currentWave.spawnEnemyCount_; ++i)
            {
                // �o��������G�̃p�����[�^���擾
                WaveEnemySet* waveEnemy = &currentWave.waveEnemySets_[i];

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
    }

    // �E�F�[�u���̂��ׂĂ̓G���|���ꂽ�玟�̃E�F�[�u�Ɉڂ�
    if (remainingEnemyCounter_ <= 0)
    {
        // ���݂̃E�F�[�u�ԍ������Z
        ++currentWaveIndex_;

        // �^�C�}�[���Z�b�g
        waveTimer_ = 0.0f;

        // �x�e���Ԃɓ���(�x�e�t���OON)
        breakTimeFlag_ = true;

        // ���݂̃E�F�[�u���Ō�̃E�F�[�u�Ȃ�G���h���X�E�F�[�u����
        const int waveIndexEnd = GetWaveIndexEnd();
        if (currentWaveIndex_ > waveIndexEnd)
        {
            // �G���h���X�E�F�[�u�t���O�������Ă��Ȃ���Η��Ă�
            if (false == endlessWaveFlag_) { endlessWaveFlag_ = true; }

            // �G���h���X�E�F�[�u�J�E���^�[���Z
            ++endlessWaveCounter_;

            // �G���h���X�E�F�[�u�J�E���^�[���T�𒴂�����P�Ƀ��Z�b�g����
            if (endlessWaveCounter_ > 5) { endlessWaveCounter_ = 1; }

            // �̗́E�U���͑���
            endlessWaveHp_  += Set::ADD_HP;
            endlessGolemHp_ += Set::ADD_HP;
            endlessWaveAtk_ += Set::ADD_ATK;

            // �G���h���X�E�F�[�u�J�E���^�[���T�̂Ƃ��̓S�[�����𐶐�����悤�ɂ���
            if (endlessWaveCounter_ == 5)
            {
                // �S�[����1~3�̐�������悤�ɂ���
                endlessWaveCreateCount_ = ::RandInt(1, 3);
            }
            else
            {
                // 7~9�̓G�𐶐�����
                endlessWaveCreateCount_ = ::RandInt(7, 9);
            }

            // �G���h���X�E�F�[�u�Ő�������G�̐����c��̓G�J�E���^�[�ɑ��
            remainingEnemyCounter_ = endlessWaveCreateCount_;
        }
        else
        {
            // �E�F�[�u�̓G�̑������c��̓G�J�E���^�[�ɑ��
            remainingEnemyCounter_ = waveParent_.children_[currentWaveIndex_].spawnEnemyCount_;
        }

        // wave�̕\�L���o��
        UserInterface::Instance().SetWaveSlideSprite();
        // wave�̌��ʉ�
        AudioManager::Instance().PlaySE(SE::WaveBegin);

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
        enemy->SetAttack(waveEnemy->atk_);              // �U���͐ݒ�
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
    const size_t waveCount = waveParent_.waveCount_;
    for (size_t waveIndex = 0; waveIndex < waveCount; ++waveIndex)
    {
        // �E�F�[�u�擾
        Wave* wave = &waveParent_.children_[waveIndex];

        const size_t enemyCount = wave->spawnEnemyCount_;
        for (size_t enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
        {
            // �E�F�[�u�G�l�~�[�擾
            WaveEnemySet* waveEnemy = &wave->waveEnemySets_[enemyIndex];

            // �����t���O�����Z�b�g
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
            // �G���h���X�E�F�[�u�J�E���^�[
            int endlessWaveCounter = endlessWaveCounter_;
            ImGui::InputInt("endlessWaveCounter", &endlessWaveCounter);

            // �G���h���X�E�F�[�u�̐�������G�̐�
            int endlessWaveCreateCount = endlessWaveCreateCount_;
            ImGui::InputInt("endlessWaveCreateCount", &endlessWaveCreateCount);

            ImGui::EndMenu();
        }
    }
    // ���݂̃E�F�[�u�����f�o�b�O�\��
    else if (ImGui::BeginMenu("Wave"))
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
