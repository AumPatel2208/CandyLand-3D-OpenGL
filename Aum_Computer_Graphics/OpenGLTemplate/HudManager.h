#pragma once
#include "Common.h"
#include "HudItem.h"
#include "Texture.h"

class CShaderProgram;

class HudManager {
    
public:

    HudManager();
    ~HudManager();
    
    void Create();
    void Render(CShaderProgram* shader);

private:
    unsigned int quadVAO, quadVBO;
    // HUD Items
    HudItem santa = {};
    HudItem speedometer = {};

    unsigned int secondQuadVAO, secondQuadVBO;

    

    

};
