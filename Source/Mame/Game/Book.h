#pragma once

#include "Item.h"

#include "ProjectileManager.h"

class Book : public Item
{
public:
    Book();
    ~Book() override;

    void Initialize()                                           override; // ������
    void Finalize()                                             override; // �I����
    void Begin()                                                override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override; // �X�V����
    void End()                                                  override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime, const float& scale)   override; // �`�揈��
    void DrawDebug()                                            override; // ImGui�p

public:
    void SetTransform(const float& elapsedTime);

private:
    // ----- ����ɕK�v�ȏ�� -----
    static int totalNum;    // ���� "Book" ������������Ă邩�m�邽�߂̂���
    int num;                // �����������ڂ�
    // ----- ����ɕK�v�ȏ�� -----

    ProjectileManager projectileManager;

    float launchTimer = 0.0f; // ���ˎ���

    DirectX::XMFLOAT3 createPosition{}; // �����ʒu
    DirectX::XMFLOAT3 prevPosition{};   // �P�t���[���O�̈ʒu
    bool isMoveToUp = true;             // ��Ɉړ����邩
};