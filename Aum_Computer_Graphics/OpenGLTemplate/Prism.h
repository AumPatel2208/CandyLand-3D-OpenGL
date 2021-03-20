#pragma once
#include "GameObject.h"

class Prism : public GameObject {
public:
    Prism();
    void Create(float radius, float height, float sideCount);
    void Render() override;

private:
    vector<glm::vec3> topFace;
    vector<glm::vec3> bottomFace;
    vector<glm::vec3> unitCircleVertices;
    
    vector<glm::vec3> baseVertices;
    vector<glm::vec3> baseNormals;
    vector<glm::vec2> baseTexCoords;
    
    vector<glm::vec3> topVertices;
    vector<glm::vec3> topNormals;
    vector<glm::vec2> topTexCoords;

    vector<glm::vec3> sideVertices;
    vector<glm::vec3> sideNormals;
    vector<glm::vec2> sideTexCoords;
    int baseCenterPointIndex;
    int topCenterPointIndex;
    int sidesIndex;


};
