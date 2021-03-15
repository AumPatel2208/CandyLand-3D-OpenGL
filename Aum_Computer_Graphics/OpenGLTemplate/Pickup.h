#pragma once
#include "GameObject.h"
#include "Texture.h"


class Pickup : public GameObject {

public:
    Pickup();
    void Update(double dt);

    // void Update(double dt, CCatmullRom*& catmullRom);
    float spinAmount = 0.f;
    float spinHeight = 0.f;
    float spinHeightMax = 0.5f;
    float heightIncrements = 0.001f;

    void incrementSpinHeight(double dt) {
        if (spinHeight >= 0.5f || spinHeight <= 0.f)
            heightIncrements *= -1;
        spinHeight+= heightIncrements*dt;
    }

    void incrementSpin(double dt) {
        if(spinAmount == 360)
            spinAmount = 0;

        spinAmount += 0.001f * dt;
    }

private:


};
