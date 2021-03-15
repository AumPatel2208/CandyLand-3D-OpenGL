#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"

class COpenAssetImportMesh;
class CCamera;
class CCatmullRom;

// Class for generating a unit sphere
class Pickup {
public:
    Pickup();
    ~Pickup();
    void Create(string directory, string front, int slicesIn, int stacksIn);
    void Render();
    void Update(double dt, CCatmullRom*& catmullRom);
    void Release();

    glm::vec3 position() { return mPosition; }
    glm::vec3 scale() { return mScale; }
    glm::vec3 rotationAxis() { return mRotationAxis; }
    float rotationAmount() { return mRotationAmount; }
    // void addSpeed (float speed) {
    //     mSpeed += speed;
    // }

    void setPosition(glm::vec3 pos) {
        mPosition = pos;
    }

    void addXOffset(float offset) {
        positionOffset = offset;
    }

private:
    UINT m_vao;
    CVertexBufferObject vbo;
    CTexture m_texture;
    COpenAssetImportMesh* mMesh;
    string m_directory;
    string m_filename;
    int m_numTriangles;
    vector<glm::vec3> triangles;

    // float mSpeed;
    glm::vec3 mPosition;
    float mRotationAmount;
    glm::vec3 mRotationAxis;
    glm::vec3 mScale;
    glm::vec3 mForward;
    glm::vec3 mUp;
    float mOffsetFromCamera = 2;
    float positionOffset = 0;
    float m_currentDistance;


};
