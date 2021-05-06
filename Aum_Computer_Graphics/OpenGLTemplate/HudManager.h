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

    void setSpeedometerIndex(int index) {
        speedometerIndex = index;
    }

private:
    unsigned int quadVAO, quadVBO;
    // HUD Items
    HudItem santa = {};
    HudItem speedometer = {};

    int speedometerIndex = 0;

    unsigned int secondQuadVAO, secondQuadVBO;


};
