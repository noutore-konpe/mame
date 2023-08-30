#pragma once

#include "Player.h"

class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {}

public:
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    void Initialize();                                                  // ������
    void Finalize();                                                    // �I����
    void Begin();                                                       // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);                              // �X�V����
    void End();                                                         // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime, const float& scale);          // �`�揈��
    void DrawDebug();                                                   // �f�o�b�O�`��

public: // �擾�E�ݒ� �֘A
    std::unique_ptr<Player>& GetPlayer() { return player; }
    
private:
    std::unique_ptr<Player> player = nullptr;
};

