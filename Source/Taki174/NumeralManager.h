#pragma once

#include "Numeral.h"
#include <vector>
#include <set>

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
    // É_ÉÅÅ[ÉWêîéöê∂ê¨
    void CreateDamageNumeral(
        const int numeral,
        const DirectX::XMFLOAT3& worldPos,
        const DirectX::XMFLOAT2& size = { 1,1 },
        const DirectX::XMFLOAT4& color = { 1,1,1,1 },
        const float angle = 0.0f
    );

private:
    std::vector<Numeral*> numerals_ = {};
    std::set<Numeral*>    removes_ = {};

};
