#include "PlayerManager.h"

// ������
void PlayerManager::Initialize()
{
    player->Initialize();
}

// �I����
void PlayerManager::Finalize()
{
    player->Finalize();
}

// Update�̑O�ɌĂ΂��
void PlayerManager::Begin()
{
    player->Begin();
}

// �X�V����
void PlayerManager::Update(const float& elapsedTime)
{
    player->Update(elapsedTime);
}

// Update�̌�ɌĂ΂��
void PlayerManager::End()
{
    player->End();
}

// �`�揈��
void PlayerManager::Render(const float& elapsedTime, const float& scale)
{
    player->Render(elapsedTime, scale);
}

// ImGui�p
void PlayerManager::DrawDebug()
{
#ifdef USE_IMGUI
    player->DrawDebug();
#endif // USE_IMGUI
}
