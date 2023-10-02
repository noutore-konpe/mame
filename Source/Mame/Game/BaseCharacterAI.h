#pragma once

#include <memory>

#include "Character.h"

#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

class BaseCharacterAI : public Character
{
public:
    BaseCharacterAI() {}
    virtual ~BaseCharacterAI();

    virtual void Initialize() {}                      // ������
    virtual void Finalize() {}                        // �I����
    virtual void Begin() {}                           // ���t���[����ԍŏ��ɌĂ΂��
    virtual void Update(const float elapsedTime);     // �X�V����
    virtual void End() {}                             // ���t���[����ԍŌ�ɌĂ΂��
    virtual void Render(const float elapsedTime, const float scale); // �`�揈��
    virtual void DrawDebug();                         // �f�o�b�O�`��

protected:
    std::unique_ptr<BehaviorTree>   behaviorTree_;
    std::unique_ptr<BehaviorData>   behaviorData_; // ��ɃV�[�P���X�Ɏg��
    std::unique_ptr<NodeBase>       activeNode_;

};

