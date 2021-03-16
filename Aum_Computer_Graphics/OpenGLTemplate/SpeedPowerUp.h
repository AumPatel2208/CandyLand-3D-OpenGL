#pragma once
#include "GameObject.h"

class SpeedPowerUp : public GameObject {
    
public:
    SpeedPowerUp();
    void Create(float length, float width, float height) ;
    void Render() override;

private:
    vector<glm::vec3> triangleNormals;
    vector<glm::vec2> triangleTextureCoordinates;
};
