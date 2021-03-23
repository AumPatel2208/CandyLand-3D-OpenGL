#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// ABOVE CODE IS REQUIRED

#include "CatmullRom.h"
#include "Car.h"

Car::Car():GameObject() {
    mPosition = mPosition = glm::vec3(0.0f, 10.0f, 150.0f);
    mScale = glm::vec3(0.5f);
    mCollisionRadius = 1.5f;
    // mCollisionScale = glm::vec3(0.5f,0.5f,1.f);
    mCollisionHeight = 1.f;
    // mMeshPath = "resources\\models\\chrysler\\Crysler_new_yorker.obj";
    mMeshPath = "resources\\models\\spaceship01\\StarSparrow_FBX\\use\\ship.obj";
    mSpeed = 0.1f;
    mPositionOffsetSpeed = .005f;
    m_currentDistance = 0.0f;
}
void Car::Advance(double direction) {
    // float speed = (float)(mSpeed * direction);
    // mPosition += speed;
    
}

void Car::Strafe(double direction) {
    //    glm::vec3 vector = glm::cross(mPosition, m_upVector);
    //    m_strafeVector = glm::normalize(vector);
    // float speed = (float)(mSpeed * direction);
    //    mPosition.x = mPosition.x + m_strafeVector.x * speed;
    //    mPosition.z = mPosition.z + m_strafeVector.z * speed;


    // ROTATE TEMP HERE
    glm::vec3 vView = mForward - mPosition;

    glm::mat4 R = glm::rotate(glm::mat4(1), mRotationAmount * 180.0f / (float)M_PI, mUp);
    glm::vec4 newView = R * glm::vec4(vView, 1);

    mForward = mPosition + glm::vec3(newView);
}
void Car::Update(double dt, CCatmullRom*& catmullRom) {
  
    m_currentDistance += dt * mSpeed; // increment by 0.1
    glm::vec3 p;
    glm::vec3 pNext;
    catmullRom->Sample(m_currentDistance + mOffsetFromCamera, p);
    catmullRom->Sample(m_currentDistance + mOffsetFromCamera + dt * 0.1f, pNext);
    
    glm::vec3 tangent = pNext - p;
    tangent = glm::normalize(tangent);
    glm::vec3 normal = glm::normalize(glm::cross(tangent, glm::vec3(0, 1, 0)));
    glm::vec3 binormal = glm::normalize(glm::cross(normal, tangent));

    p.y += .5f;

    // rotationOnPath = glm::mat4(glm::mat3(tangent,normal,binormal));
    rotationOnPath = glm::mat4(glm::mat3(tangent,binormal,normal));
    
    glm::vec3 up = glm::rotate(glm::vec3(0, 1, 0), rotationAmount(), tangent);

    // car->setPosition(pNext);
    // move left and right
    p = p+normal*mPositionOffset;
    mPosition = p;
}


