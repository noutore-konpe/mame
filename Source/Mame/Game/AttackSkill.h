#pragma once
#include "BaseSkill.h"

namespace PlayerSkill
{
    class Drain : public BaseSkill
    {
    public:
        Drain(Player* player) : BaseSkill(player) {}
        ~Drain() {}

        void Initialize()override;
        void Update(float elapsedTime)override;
        void Render()override;
        void DrawDebug()override;

        void Overlaping()override;

        //HP�z������ ������v���C���[�̍U�������ɑg�ݍ���
        void Assimilate(float power/*�G�ɗ^�����U���̗�*/);

    private:
        float drainCoefficient;//�U���͂���z��HP�̎Z�o�Ɏg���W��(�d�����邽�тɂ��ꂪ�����Ă���)
    };
}

