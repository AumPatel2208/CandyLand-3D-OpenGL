#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// ABOVE CODE IS REQUIRED

#include "Pickup.h"

Pickup::Pickup():GameObject() {
    mPosition = mPosition = glm::vec3(0.0f, 10.0f, 0.0f);
    mScale = glm::vec3(10.f);
    mCollisionHeight = 1.f;
    // mMeshPath = "resources\\models\\among_us_obj\\among us.obj";
    mMeshPath = "resources\\models\\ChocoSantaClaus06_obj\\ChocoSantaClaus06.obj";
}

void Pickup::Update(double dt) {
    incrementSpin(dt);
    incrementSpinHeight(dt);
}


