#pragma once
#include "MatrixStack.h"
#include "shaders.h"

class CCamera;
class COpenAssetImportMesh;
class CShaderProgram;

class Player {
public:
    Player();
    ~Player();
    void Initialise();
    void Update();
    void Render(glutil::MatrixStack& modelViewMatrixStack, CShaderProgram*& pMainProgram, CCamera*& camera);
    
    

private:
    COpenAssetImportMesh *m_pHorseMesh;
	// CCamera*  m_pCamera;

};
