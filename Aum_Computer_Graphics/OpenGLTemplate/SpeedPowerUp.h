#pragma once
#include "Pickup.h"

class SpeedPowerUp : public Pickup {
    
public:
    SpeedPowerUp();
    void Create(float length, float width, float height) ;
    void Render() override;

private:
    vector<glm::vec3> triangleNormals;
    vector<glm::vec2> triangleTextureCoordinates;
};
