#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"

class COpenAssetImportMesh;
class CSphere;

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
    // bool CheckCollision(glm::vec3 bPosition, float bCollisionRadius);
    
    // getters
    glm::vec3 position() { return mPosition; }
    glm::vec3 scale() { return mScale; }
    glm::vec3 rotationAxis() { return mRotationAxis; }
    float rotationAmount() { return mRotationAmount; }
    float collisionRadius(){ return mCollisionRadius;}
    float collisionHeight(){ return mCollisionHeight;}
    glm::vec3 collisionScale(){ return mCollisionScale;}

    CSphere* &getCollisionSphere(){return mCollisionSphere;}
    bool showCollisionSphere = true;


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

    CSphere* mCollisionSphere;
    

    // float mSpeed;
    glm::vec3 mPosition;
    float mRotationAmount;
    glm::vec3 mRotationAxis;
    glm::vec3 mScale;
    glm::vec3 mForward;
    glm::vec3 mUp;
    float mCollisionRadius = 1.f;
    float mCollisionHeight = 0.1f;
    glm::vec3 mCollisionScale = glm::vec3(1.f);

};
