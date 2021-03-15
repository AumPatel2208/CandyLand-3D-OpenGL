#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "GameObject.h"
#include <math.h>

#include "camera.h"
#include "CatmullRom.h"
#include "OpenAssetImportMesh.h"


GameObject::GameObject(): mVao(0), mNumTriangles(0) {
    mMesh = NULL;
    mPosition = glm::vec3(0.0f, 10.0f, 0.0f);
    mScale = glm::vec3(1.f);
    mRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    mRotationAmount = 0.0f;
    mMeshPath = "resources\\models\\Barrel\\Barrel02.obj";

    mForward = glm::vec3(0, 0, 1);
    mUp = glm::vec3(0, 1, 0);
}

GameObject::~GameObject() {
    delete mMesh;
}

void GameObject::Create(string path) {
    mMesh = new COpenAssetImportMesh;
    mMesh->Load(path);
    mMeshPath = path;
}

void GameObject::Create() {
    mMesh = new COpenAssetImportMesh;
    mMesh->Load(mMeshPath);
}

void GameObject::Render() {
    mMesh->Render();
}

void GameObject::Release() {
    mTexture.Release();
    glDeleteVertexArrays(1, &mVao);
    vbo.Release();
}

bool GameObject::CheckCollision(glm::vec3 bPosition, float bCollisionRadius) {

    float distance = glm::distance(mPosition, bPosition);

    if (distance < mCollisionRadius || distance < bCollisionRadius) {
        return true;
    }

    return false;
}
