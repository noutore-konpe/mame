#pragma once

#include "State.h"
#include "Enemy.h"

class DemoState : public State<Enemy>
{
public:
    DemoState(Enemy* enemy) :State(enemy, "enemy") {}
    ~DemoState() {}

    void Initialize() override;
    void Update(const float& elapsedTime) override;
    void Finalize() override;
};