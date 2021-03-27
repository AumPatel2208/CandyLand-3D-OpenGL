#pragma once
#pragma once
#include "GameObject.h"
#include "SpeedPowerUp.h"
#include "Texture.h"

class CCatmullRom;

class Car : public GameObject {

public:
    Car();
    void Update(double dt, CCatmullRom*& catmullRom);
    void Advance(double p);
    void Strafe(double p);

    void addSpeed(float speed) {
        mSpeed += speed;
    }
    void setSpeed (float speed) {
        mSpeed = speed;
    }
    float getSpeed() {
        return mSpeed;
    }
    
    void setPosition(glm::vec3 pos) {
        mPosition = pos;
    }

    float getXOffsetSpeed() {
        return mPositionOffsetSpeed;
    }
    float getXOffset() {
        return mPositionOffset;
    }
    
    void setXOffset(float offset) {
        mPositionOffset = offset;
    }
    glm::mat4 getRotationOnPath(){return rotationOnPath;}
    

private:
    float m_currentDistance;
    float mOffsetFromCamera = 10;
    float mPositionOffset = 0;
    float mPositionOffsetSpeed;
    float mSpeed;

    glm::mat4 rotationOnPath;
};
