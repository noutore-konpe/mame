#pragma once

#include "State.h"
#include "Book.h"

namespace BookState
{
    class IdleState : public State<Book>
    {
    public:
        IdleState(Book* book) : State(book, "book") {}
        ~IdleState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        // �����Ă���G��T��
        [[nodiscard]] const bool SearchAliveEnemy();

    };

    class OpenState : public State<Book>
    {
    private:
        enum RotateState
        {
            No_Rot,                 // ��]�Ȃ�
            VerticalRot,            // �c��]
            HorizontalRot,          // ����]
            DiagonalRot,            // �΂߉�]
            ContenuOpenState,       // ��]����
        };

    public:
        OpenState(Book* book) : State(book, "book") {}
        ~OpenState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        DirectX::XMFLOAT4 addBookRot = {};
        int rotateToOpen = 0;

        bool isOpen = false;
    };

    class AttackState : public State<Book>
    {
    public:
        AttackState(Book* book) : State(book, "book") {}
        ~AttackState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        // ��ԋ߂��G��T��
        [[nodiscard]] const bool SearchNearEnemy(Enemy** enemy);

    private:
        int launchNum = 0;
        bool isLaunch = false;
    };

    class CloseState : public State<Book>
    {
    public:
        CloseState(Book* book) : State(book, "book") {}
        ~CloseState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;

    private:
        bool isClose = false;
    };

    class CounterAttackState : public State<Book>
    {
    public:
        CounterAttackState(Book* book) : State(book, "book") {}
        ~CounterAttackState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override;
    };
}