#include "HudItem.h"

HudItem::HudItem() {
    
}
HudItem::~HudItem() {
    
}

void HudItem::Create() {
    
}

void HudItem::Bind(int index) {
    // glBindTexture(GL_TEXTURE_2D,mHudTextures[index]);
    mHudTextures[index].Bind();
}


void HudItem::AddTexture(string path) {
    CTexture hudTex;
    // HUD SETUP
    hudTex.Load(path);

    hudTex.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    hudTex.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    hudTex.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    hudTex.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

    mHudTextures.push_back(hudTex);
}

