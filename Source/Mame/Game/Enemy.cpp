#include "Enemy.h"

void Enemy::Initialize()
{
    Character::Initialize();
}

void Enemy::Update(const float& elapsedTime)
{
    Character::Update(elapsedTime);
}

void Enemy::Render(const float& elapsedTime, const float& scale)
{
    Character::Render(elapsedTime, scale);
}