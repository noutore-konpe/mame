#pragma once

#include "../sprite.h"

#include "spriteDissolve.h"

#include <string>
#include <memory>

class UI
{
public:
    UI(const wchar_t* filename);
    ~UI();

    void Render();
    void DrawDebug();

    void SetPosition(DirectX::XMFLOAT3 p) { spr.pos = p; }
    DirectX::XMFLOAT3 GetPosition() { return spr.pos; }
    void SetSize(DirectX::XMFLOAT2 s) { spr.size = s; }
    DirectX::XMFLOAT2 GetSize() { return spr.size; }

    void SetIsRender(bool r) { isRender = r; }
    bool GetIsRender() { return isRender; }

    // Imgui–¼‘O”í‚è–hŽ~‚Ì‚½‚ß‚Ì‚â‚Â //
    static int nameNum;
    const char* GetName() const { return name.c_str(); }
    void SetName(const char* n) { name = n; }
    // Imgui–¼‘O”í‚è–hŽ~‚Ì‚½‚ß‚Ì‚â‚Â //   

private:
    struct SpriteUi
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT2 size;
    }spr;

private:
    std::unique_ptr<SpriteDissolve>uiSprite;
    //std::unique_ptr<Sprite>uiSprite;
    std::string name;

    bool isRender = false;
};

