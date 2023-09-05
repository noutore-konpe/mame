#pragma once

#include <vector>
#include "State.h"

#include "Enemy.h"

template <class T>
class StateMachine
{
public:
    StateMachine() {};
    ~StateMachine();

    void Update(float elapsedTime); // �X�V����

    void DrawDebug();               // ImGui�p

    void SetState(int newState);        // �X�e�[�g�Z�b�g
    void ChangeState(int newState);     // �X�e�[�g�ύX
    void RegisterState(T* state);       // �X�e�[�g�o�^

public:// �擾�E�ݒ�
    int GetStateIndex();                // ���݂̃X�e�[�g�ԍ��擾
    T* GetCurrentState() { return currentState; } // ���݂̃X�e�[�g�擾

private:
    T* currentState;            // ���݂̃X�e�[�g
    std::vector<T*> statePool;  // �e�X�e�[�g��ێ�����z��
};

template<class T>
inline StateMachine<T>::~StateMachine()
{
    // �o�^�����X�e�[�g���폜����
    for (T* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

template<class T>
inline void StateMachine<T>::Update(float elapsedTime)
{
    currentState->Update(elapsedTime);
}

template<class T>
inline void StateMachine<T>::DrawDebug()
{
    if (ImGui::TreeNode("stateMachine"))
    {
        ImGui::Text(currentState->GetName());

        float temp = currentState->GetTimer();
        ImGui::DragFloat("time", &temp);

        ImGui::TreePop();
    }
}

template<class T>
inline void StateMachine<T>::SetState(int newState)
{
    currentState = statePool.at(newState);
    currentState->Initialize();
}

template<class T>
inline void StateMachine<T>::ChangeState(int newState)
{
    currentState->Finalize();
    SetState(newState);
}

template<class T>
inline void StateMachine<T>::RegisterState(T* state)
{
    statePool.emplace_back(state);
}

template<class T>
inline int StateMachine<T>::GetStateIndex()
{
    int i = 0;
    for (T* state : statePool)
    {
        if (state == currentState)
        {
            // i�ԍ��ڂ̃X�e�[�g�����^�[��
            return i;
        }
        ++i
    }

    // �X�e�[�g��������Ȃ������Ƃ�
    return -1;
}
