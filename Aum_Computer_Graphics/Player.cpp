#include "Player.h"


#include "camera.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "shaders.h"

Player::Player() {
    m_pHorseMesh = NULL;
    // m_pCamera = NULL;
}

Player::~Player() {
    delete m_pHorseMesh;
}

void Player::Initialise() {
    // m_pCamera = camera;
    
    m_pHorseMesh = new COpenAssetImportMesh;
    m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj"); // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
}

void Player::Render(glutil::MatrixStack& modelViewMatrixStack,CShaderProgram* &pMainProgram, CCamera* &camera) {
    // Render the horse 
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
        modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
        modelViewMatrixStack.Scale(2.5f);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", camera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pHorseMesh->Render();
    }
    modelViewMatrixStack.Pop();
}



