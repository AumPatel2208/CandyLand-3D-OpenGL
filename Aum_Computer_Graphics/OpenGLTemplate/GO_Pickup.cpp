#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// ABOVE CODE IS REQUIRED

#include "GO_Pickup.h"

GO_Pickup::GO_Pickup():GameObject() {
    mPosition = mPosition = glm::vec3(0.0f, 10.0f, 0.0f);
    mScale = glm::vec3(0.01f);   
    mMeshPath = "resources\\models\\among_us_obj\\among us.obj";

}


