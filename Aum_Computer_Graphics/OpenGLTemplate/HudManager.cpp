#include "HudManager.h"

#include "Shaders.h";
HudManager::HudManager() { }

HudManager::~HudManager() { }


void HudManager::Create() {
    float quadVertices[] = {
        // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f,

        -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f
    };


    // screen quad VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    //
    // // screen quad VAO
    // glGenVertexArrays(1, &secondQuadVAO);
    // glGenBuffers(1, &secondQuadVBO);
    // glBindVertexArray(secondQuadVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, secondQuadVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    
    santa.AddTexture("resources\\textures\\hud\\santa_png\\santa.png");

    for (int i = 1; i <= 6; ++i) {
        speedometer.AddTexture("resources\\textures\\hud\\speedometer_png\\speed_" + to_string(i) + ".png");
    }
    

}

void HudManager::Render(CShaderProgram* shader) {

    //
    // // Draw santa
    glBindVertexArray(quadVAO);
    santa.Bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // shader->UseProgram();
    // Draw Speedometer
    glBindVertexArray(quadVAO);
    speedometer.Bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);


}
