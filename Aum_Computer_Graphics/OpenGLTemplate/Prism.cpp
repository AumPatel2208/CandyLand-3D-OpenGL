#include "Common.h"
#include "Prism.h"

Prism::Prism(): GameObject(), baseCenterPointIndex(0), topCenterPointIndex(0) {
    mPosition = glm::vec3(15, 10, 10);
    mScale = glm::vec3(1.f);
    // mRotationAmount = 90;
    // mRotationAxis = glm::vec3(1,0,0);

    mTexture.Load("resources\\textures\\colorful-diagonal-lines-abstract-background\\abstractLines.jpg");

    mTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    mTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    mTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Prism::Create(float radius, float height, float sideCount) {
    // tutorial followed code, has been adapted: http://www.songho.ca/opengl/gl_cylinder.html

    const float PI = 3.1315926f;
    float sideStep = 2 * PI / sideCount;
    float sectorAngle; // radians

    // add the vertices in for a face with 0 height
    for (int i = 0; i < sideCount; ++i) {
        sectorAngle = i * sideStep;
        unitCircleVertices.emplace_back(cos(sectorAngle), 0, sin(sectorAngle)); // x y and z
    }

    // BASE AND TOP

    baseCenterPointIndex = 0;
    // center point fpr the base, no height
    baseVertices.emplace_back(0.f, 0.f, 0.f);
    // normal is facing straight down
    baseNormals.emplace_back(0.f, -1.f, 0.f);
    // center of the texture
    baseTexCoords.emplace_back(0.5f, 0.5f);

    for (int i = 0; i < sideCount; ++i) {
        baseVertices.emplace_back(unitCircleVertices[i].x * radius, 0.f, unitCircleVertices[i].z * radius);
        baseNormals.emplace_back(0.f, -1.f, 0.f);
        // texture coordinate
        baseTexCoords.emplace_back(-unitCircleVertices[i].x * 0.5f + 0.5f, -unitCircleVertices[i].z * 0.5f + 0.5f);
    }

    // adding the first point back in 
    baseVertices.emplace_back(unitCircleVertices[0].x * radius, 0.f, unitCircleVertices[0].z * radius);
    baseNormals.emplace_back(0.f, -1.f, 0.f);
    baseTexCoords.emplace_back(-unitCircleVertices[0].x * 0.5f + 0.5f, -unitCircleVertices[0].z * 0.5f + 0.5f);


    // topCenterPointIndex = vertices.size();
    // // center point for the top 
    // vertices.emplace_back(0.f, height, 0.f);
    // // normal is facing straight up
    // normals.emplace_back(0.f, 1.f, 0.f);
    // // center of the texture
    // texCoords.emplace_back(0.5f, 0.5f);
    // for (int i = 0; i < sideCount; ++i) {
    //     vertices.emplace_back(unitCircleVertices[sideCount -1 - i].x * radius, height, unitCircleVertices[sideCount -1 - i].z * radius);
    //     normals.emplace_back(0.f, 1.f, 0.f);
    //     texCoords.emplace_back(-unitCircleVertices[sideCount -1 - i].x * 0.5f + 0.5f, -unitCircleVertices[sideCount -1 - i].z * 0.5f + 0.5f);
    // }
    // // adding the first point back in 
    // vertices.emplace_back(unitCircleVertices[sideCount - 1].x * radius, height, unitCircleVertices[sideCount - 1].z * radius);
    // normals.emplace_back(0.f, -1.f, 0.f);
    // texCoords.emplace_back(-unitCircleVertices[sideCount - 1].x * 0.5f + 0.5f, -unitCircleVertices[sideCount - 1].z * 0.5f + 0.5f);
    //


    // center point for the top 
    topVertices.emplace_back(0.f, height, 0.f);
    // normal is facing straight up
    topNormals.emplace_back(0.f, 1.f, 0.f);
    // center of the texture
    topTexCoords.emplace_back(0.5f, 0.5f);
    for (int i = 0; i < sideCount; ++i) {
        topVertices.emplace_back(unitCircleVertices[sideCount - 1 - i].x * radius, height, unitCircleVertices[sideCount - 1 - i].z * radius);
        topNormals.emplace_back(0.f, 1.f, 0.f);
        topTexCoords.emplace_back(-unitCircleVertices[sideCount - 1 - i].x * 0.5f + 0.5f, -unitCircleVertices[sideCount - 1 - i].z * 0.5f + 0.5f);
    }
    // adding the first point back in 
    topVertices.emplace_back(unitCircleVertices[sideCount - 1].x * radius, height, unitCircleVertices[sideCount - 1].z * radius);
    topNormals.emplace_back(0.f, -1.f, 0.f);
    topTexCoords.emplace_back(-unitCircleVertices[sideCount - 1].x * 0.5f + 0.5f, -unitCircleVertices[sideCount - 1].z * 0.5f + 0.5f);


    for (int i = 0; i < baseVertices.size(); ++i) {
        // skip the center points
        if (i == 0) {
            sideVertices.push_back(topVertices[topVertices.size() - 1 - i]);
            sideNormals.push_back(topNormals[topVertices.size() - 1 - i]);
            sideTexCoords.push_back(topTexCoords[topVertices.size() - 1 - i]);
            continue;
        }
        if(i == topVertices.size() - 1) {
            sideVertices.push_back(baseVertices[i]);
            sideNormals.push_back(baseNormals[i]);
            sideTexCoords.push_back(baseTexCoords[i]);
            continue;
        }
        

        sideVertices.push_back(baseVertices[i]);
        sideNormals.push_back(baseNormals[i]);
        sideTexCoords.push_back(baseTexCoords[i]);

        sideVertices.push_back(topVertices[topVertices.size() - 1 - i]);
        sideNormals.push_back(topNormals[topVertices.size() - 1 - i]);
        sideTexCoords.push_back(topTexCoords[topVertices.size() - 1 - i]);
    }
    
    // sideVertices.push_back(baseVertices[1]);
    // sideNormals.push_back(baseNormals[1]);
    // sideTexCoords.push_back(baseTexCoords[1]);
    //
    // sideVertices.push_back(topVertices[topVertices.size() - 1 ]);
    // sideNormals.push_back(topNormals[topVertices.size() - 1 ]);
    // sideTexCoords.push_back(topTexCoords[topVertices.size() - 1 ]);


    //// UNCNSOCJASOKFHJN
    glm::vec3 tempVec = sideVertices[0];
    glm::vec3 tempNorm = sideNormals[0];
    glm::vec2 tempTex = sideTexCoords[0];
    sideVertices.push_back(tempVec);
    sideNormals.push_back(tempNorm);
    sideTexCoords.push_back(tempTex);


    //
    // // SIDES
    // sidesIndex = vertices.size();
    // vector<glm::vec3> tempVertices;
    // for (int i = 0; i < sideCount; ++i) {
    //     // skip the center points
    //     if(i == baseCenterPointIndex || i == topCenterPointIndex)
    //         continue;
    //
    //     tempVertices.push_back(vertices[i]);
    //     normals.emplace_back(0.f, -1.f, 0.f);
    //     // texture coordinate
    //     texCoords.emplace_back(-unitCircleVertices[i].x * 0.5f + 0.5f, -unitCircleVertices[i].z * 0.5f + 0.5f);
    // 	
    //     tempVertices.push_back(vertices[sideCount - 1 - i]);
    //     normals.emplace_back(0.f, 1.f, 0.f);
    //     texCoords.emplace_back(-unitCircleVertices[sideCount - 1 - i].x * 0.5f + 0.5f, -unitCircleVertices[sideCount - 1 - i].z * 0.5f + 0.5f);
    //
    // }
    //
    // for (auto& vertex : tempVertices) {
    //     vertices.push_back(vertex);
    // }
    //

    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    GLsizei stride = sizeof(glm::vec3) * 2 + sizeof(glm::vec2);

    vbo.Create();
    vbo.Bind();

    // VBO ADD DATA
    for (int i = 0; i < baseVertices.size(); ++i) {
        vbo.AddData(&baseVertices[i], sizeof(glm::vec3));
        vbo.AddData(&baseTexCoords[i], sizeof(glm::vec2));
        vbo.AddData(&baseNormals[i], sizeof(glm::vec3));
    } // VBO ADD DATA

    for (int i = 0; i < topVertices.size(); ++i) {
        vbo.AddData(&topVertices[i], sizeof(glm::vec3));
        vbo.AddData(&topTexCoords[i], sizeof(glm::vec2));
        vbo.AddData(&topNormals[i], sizeof(glm::vec3));
    }

    for (int i = 0; i < sideVertices.size(); ++i) {
        vbo.AddData(&sideVertices[i], sizeof(glm::vec3));
        vbo.AddData(&sideTexCoords[i], sizeof(glm::vec2));
        vbo.AddData(&sideNormals[i], sizeof(glm::vec3));
    }

    // UPLOAD THAT DATA 
    vbo.UploadDataToGPU(GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

}

void Prism::Render() {
    mTexture.Bind();

    glBindVertexArray(mVao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, baseVertices.size());
    glDrawArrays(GL_TRIANGLE_FAN, baseVertices.size(), topVertices.size());
    glDrawArrays(GL_TRIANGLE_STRIP, baseVertices.size() + topVertices.size(), sideVertices.size()  );
    // glDrawArrays(GL_TRIANGLE_FAN, topCenterPointIndex, vertices.size());

    // glDrawArrays(GL_TRIANGLE_FAN, topCenterPointIndex, sidesIndex);
    // glDrawArrays(GL_QUAD_STRIP, sidesIndex, vertices.size());

}
