#pragma once

#include "Numeral.h"
#include <vector>
#include <set>

class Character;

class NumeralManager
{
private:
    NumeralManager()  = default;
    ~NumeralManager() = default;

public:
    static NumeralManager& Instance()
    {
        static NumeralManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
    void Update(const float elapsedTime);
    void Render();
    void DrawDebug();

public:
    void Register(Numeral* numeral);
    void Remove(Numeral* numeral);
    void ClearAll();

public:
    // ダメージ数字生成
    void CreateDamageNumeral(
        Character* parent,
        const int numeral,
        const DirectX::XMFLOAT3& worldPos,
        const DirectX::XMFLOAT2& size = { 22, 30 },
        const DirectX::XMFLOAT4& color = { 1,1,1,1 },
        const float angle = 0.0f,
        const float rowOffset = 40.0f // 行間調整
    );

    const size_t GetNumeralCount() const { return numerals_.size(); }

    Numeral* GetNumeral(const size_t index) { return numerals_.at(index); }

private:
    std::vector<Numeral*> numerals_ = {};
    std::set<Numeral*>    removes_ = {};

};
