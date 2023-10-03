#pragma once
#include "MagicCircle.h"

class MagicCircleSummon
{
public:
    enum class MAGIC_CIRCLE
    {
        Base,
        Move1,
        Move2,
    };

    enum class STATE
    {
        AddScale,
        MoveUp,
    };

public:
    MagicCircleSummon();
    ~MagicCircleSummon();

    void Initialize();
    void Finalize();
    void Update(const float& elapsedTime);
    void Render();
    void DrawDebug();

private:
    std::unique_ptr<MagicCircle> magicCircle[3];

    int state = 0;
    

    float scaleTimer = 0.0f; // easing—p

};

