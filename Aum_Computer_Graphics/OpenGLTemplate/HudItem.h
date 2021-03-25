#pragma once
#include "Common.h"
#include "Texture.h"

class HudItem {
public:
    HudItem();
    ~HudItem();
    void Create();

    void AddTexture(string path);

    void Bind(int index);

private:
    vector<CTexture> mHudTextures;
};
