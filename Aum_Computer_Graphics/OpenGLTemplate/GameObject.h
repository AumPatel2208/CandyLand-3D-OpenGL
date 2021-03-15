#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"

class COpenAssetImportMesh;

// Class for generating a unit sphere
class GameObject {
public:
    GameObject();
    virtual ~GameObject();
    void Create(string path);
    void Create();
    void Render();
    void Update(double dt);
    void Release();

    // Basic Collision Detection between two objects
    bool CheckCollision(glm::vec3 bPosition, float bCollisionRadius);
    
    // getters
    glm::vec3 position() { return mPosition; }
    glm::vec3 scale() { return mScale; }
    glm::vec3 rotationAxis() { return mRotationAxis; }
    float rotationAmount() { return mRotationAmount; }
    float collisionRadius(){ return mCollisionRadius;}



    // setters
    void setPosition(glm::vec3 pos) {
        mPosition = pos;
    }

protected:
    UINT mVao;
    CVertexBufferObject vbo;
    CTexture mTexture;
    COpenAssetImportMesh* mMesh;
    string mDirectory;
    string mMeshPath;
    string mFilename;
    int mNumTriangles;
    vector<glm::vec3> triangles;

    // float mSpeed;
    glm::vec3 mPosition;
    float mRotationAmount;
    glm::vec3 mRotationAxis;
    glm::vec3 mScale;
    glm::vec3 mForward;
    glm::vec3 mUp;
    float mCollisionRadius = 0.f;

};
