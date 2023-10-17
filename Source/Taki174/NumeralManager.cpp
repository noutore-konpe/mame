#include "NumeralManager.h"

#include "../../External/imgui/ImGuiCtrl.h"
#include "../Taki174/Common.h"
#include "DamageNumeral.h"

void NumeralManager::Initialize()
{
    for (Numeral*& numeral : numerals_)
    {
        numeral->Initialize();
    }
}

void NumeralManager::Finalize()
{
    ClearAll();
}

void NumeralManager::Update(const float elapsedTime)
{
    for (Numeral*& numeral : numerals_)
    {
        numeral->Update(elapsedTime);
    }

    // ”jŠüˆ—
    {
        for (Numeral* numeral : removes_)
        {
            auto it = std::find(numerals_.begin(), numerals_.end(), numeral);
            if (it != numerals_.end())
            {
                numerals_.erase(it);
            }
            ::SafeDeletePtr(numeral);
        }
        removes_.clear();
    }
}

void NumeralManager::Render()
{
    for (Numeral*& numeral : numerals_)
    {
        //if (true == numeral->GetUseBloom()) { numeral->BloomRender(); }
        //else { numeral->Render(); }
        numeral->Render();
    }
}

void NumeralManager::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu("Numerals"))
    {
        for (Numeral*& numeral : numerals_)
        {
            numeral->DrawDebug();
        }

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif
}

void NumeralManager::Register(Numeral* numeral)
{
    numerals_.emplace_back(numeral);
}

void NumeralManager::Remove(Numeral* numeral)
{
    removes_.insert(numeral);
}

void NumeralManager::ClearAll()
{
    for (Numeral*& numeral : numerals_)
    {
        ::SafeDeletePtr(numeral);
    }
    numerals_.clear();
    numerals_.shrink_to_fit();
}

void NumeralManager::CreateDamageNumeral(
    Character* parent,
    const int damage,
    const DirectX::XMFLOAT3& worldPos,
    const DirectX::XMFLOAT2& size,
    const DirectX::XMFLOAT4& color,
    const float angle,
    const float rowOffset)
{
    DamageNumeral* dmgNumeral = new DamageNumeral(damage, worldPos, size, color, angle, rowOffset);
    dmgNumeral->SetParent(parent); // ’Ç]‚·‚ée‚ğİ’è
    this->Register(dmgNumeral);
}
