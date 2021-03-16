#include  "Common.h"
#include "SpeedPowerUp.h"


SpeedPowerUp::SpeedPowerUp() : GameObject() {
    mPosition = glm::vec3(15, 10, 5);
    mScale = glm::vec3(10.f);
}

void SpeedPowerUp::Create(float length, float width, float height) {

    triangleVertices.emplace_back(0.f, height / 2, 2.f * length); // tip
    // right of base top
    triangleVertices.emplace_back(1 * width, 0.f, 1 * length);
    triangleVertices.emplace_back(1 * width, height, 1 * length);
    //left of base top
    triangleVertices.emplace_back(-1.f * width, 0.f, 1.f * length);
    triangleVertices.emplace_back(-1.f * width, height, 1.f * length);
    //right base
    triangleVertices.emplace_back(1.f * width, 0.f, -1.f * length);
    triangleVertices.emplace_back(1.f * width, height, -1.f * length);
    //left base
    triangleVertices.emplace_back(-1.f * width, 0.f, -1.f * length);
    triangleVertices.emplace_back(-1.f * width, height, -1.f * length);

    //Normals for the different sides
    triangleNormals.push_back(glm::cross(triangleVertices[0] - triangleVertices[2], triangleVertices[0] - triangleVertices[4]));
    triangleNormals.push_back(glm::cross(triangleVertices[0] - triangleVertices[1], triangleVertices[0] - triangleVertices[2]));
    triangleNormals.push_back(glm::cross(triangleVertices[0] - triangleVertices[1], triangleVertices[0] - triangleVertices[3]));
    triangleNormals.push_back(glm::cross(triangleVertices[0] - triangleVertices[3], triangleVertices[0] - triangleVertices[4]));
    triangleNormals.push_back(glm::cross(triangleVertices[2] - triangleVertices[4], triangleVertices[2] - triangleVertices[8]));
    triangleNormals.push_back(glm::cross(triangleVertices[2] - triangleVertices[6], triangleVertices[2] - triangleVertices[8]));
    triangleNormals.push_back(glm::cross(triangleVertices[2] - triangleVertices[5], triangleVertices[2] - triangleVertices[6]));
    triangleNormals.push_back(glm::cross(triangleVertices[2] - triangleVertices[5], triangleVertices[2] - triangleVertices[1]));
    triangleNormals.push_back(glm::cross(triangleVertices[1] - triangleVertices[3], triangleVertices[1] - triangleVertices[7]));
    triangleNormals.push_back(glm::cross(triangleVertices[1] - triangleVertices[5], triangleVertices[1] - triangleVertices[7]));
    triangleNormals.push_back(glm::cross(triangleVertices[4] - triangleVertices[7], triangleVertices[4] - triangleVertices[8]));
    triangleNormals.push_back(glm::cross(triangleVertices[4] - triangleVertices[3], triangleVertices[4] - triangleVertices[7]));
    triangleNormals.push_back(glm::cross(triangleVertices[7] - triangleVertices[8], triangleVertices[7] - triangleVertices[6]));
    triangleNormals.push_back(glm::cross(triangleVertices[7] - triangleVertices[5], triangleVertices[7] - triangleVertices[6]));

    triangleTextureCoordinates.emplace_back(0.f, 0.f);
    triangleTextureCoordinates.emplace_back(1.f, 0.f);
    triangleTextureCoordinates.emplace_back(0.5f, 1.f);
    triangleTextureCoordinates.emplace_back(0.f, 1.f);
    triangleTextureCoordinates.emplace_back(1.f, 1.f);


    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    GLsizei stride = sizeof(glm::vec3) * 2 + sizeof(glm::vec2);

    vbo.Create();
    vbo.Bind();

    // vbo.AddData()
    {
        // top front
        vbo.AddData(&triangleVertices[0], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[2], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[4], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[2], sizeof(glm::vec2));


        // top right
        vbo.AddData(&triangleVertices[0], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[1], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[2], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[2], sizeof(glm::vec2));


        // top back
        vbo.AddData(&triangleVertices[0], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[3], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[2], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[1], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));


        // top left
        vbo.AddData(&triangleVertices[0], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[4], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[2], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[3], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));


        // front 1
        vbo.AddData(&triangleVertices[2], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[8], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[4], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));


        // front 2
        vbo.AddData(&triangleVertices[2], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[6], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[8], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));


        // right 1
        vbo.AddData(&triangleVertices[2], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));
        vbo.AddData(&triangleVertices[5], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));
        vbo.AddData(&triangleVertices[6], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));


        // right 2
        vbo.AddData(&triangleVertices[2], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[1], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[5], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));


        // back 1
        vbo.AddData(&triangleVertices[1], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[3], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[7], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));


        // back 2
        vbo.AddData(&triangleVertices[1], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[7], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[5], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));


        // left 1
        vbo.AddData(&triangleVertices[4], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[8], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[7], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));


        // left 2
        vbo.AddData(&triangleVertices[4], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[7], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[3], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));


        // bottom 1
        vbo.AddData(&triangleVertices[7], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[8], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[6], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));


        // bottom 2
        vbo.AddData(&triangleVertices[7], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[0], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[6], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[4], sizeof(glm::vec2));

        vbo.AddData(&triangleVertices[5], sizeof(glm::vec3));
        vbo.AddData(&triangleNormals[0], sizeof(glm::vec3));
        vbo.AddData(&triangleTextureCoordinates[1], sizeof(glm::vec2));
    }

    vbo.UploadDataToGPU(GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));


}

void SpeedPowerUp::Render() {
    glBindVertexArray(mVao);
    // 14 triangles                3 points per triangle
    glDrawArrays(GL_TRIANGLES, 0, 14 * 3);
}
