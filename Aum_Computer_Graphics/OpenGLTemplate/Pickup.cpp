#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// ABOVE CODE IS REQUIRED

#include "Pickup.h"

Pickup::Pickup():GameObject() {
    mPosition = mPosition = glm::vec3(0.0f, 10.0f, 0.0f);
    mScale = glm::vec3(0.01f);
    mCollisionHeight = 1.f;
    mMeshPath = "resources\\models\\among_us_obj\\among us.obj";
}

void Pickup::Update(double dt) {
    incrementSpin(dt);
    incrementSpinHeight(dt);
}


