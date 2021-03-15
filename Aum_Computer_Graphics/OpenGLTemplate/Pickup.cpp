#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "Pickup.h"
#include <math.h>

#include "camera.h"
#include "CatmullRom.h"
#include "OpenAssetImportMesh.h"


Pickup::Pickup(): m_vao(0), m_numTriangles(0) {
    mMesh = NULL;
    mPosition = glm::vec3(0.0f, 10.0f, 150.0f);
    mScale = glm::vec3(.01f);
    mRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    mRotationAmount = 0.0f;
    
    mForward = glm::vec3(0,0,1);
    mUp = glm::vec3(0,1,0);

    m_currentDistance = 0.0f;

    // mForward = glm::rotate(mForward, mRotationAxis, mRotationAmount, mUp);
}

Pickup::~Pickup() {
    delete mMesh;
}

// Create a unit sphere 
void Pickup::Create(string a_sDirectory, string a_sFilename, int slicesIn, int stacksIn) {
    mMesh = new COpenAssetImportMesh;
    mMesh->Load("resources\\models\\among_us_obj\\among us.obj"); // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
}

// Render the sphere as a set of triangles
void Pickup::Render() {
    // glBindVertexArray(m_vao);
    // // m_texture.Bind();
    // // glDrawElements(GL_TRIANGLES, m_numTriangles * 3, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, triangles.size());

    mMesh->Render();

}


void Pickup::Update(double dt, CCatmullRom*& catmullRom) {
    
    m_currentDistance += dt * 0.1; // increment by 0.1
    glm::vec3 p;
    glm::vec3 pNext;
    catmullRom->Sample(m_currentDistance + mOffsetFromCamera, p);
    catmullRom->Sample(m_currentDistance + mOffsetFromCamera + dt * 0.1f, pNext);
    
    glm::vec3 tangent = pNext - p;
    tangent = glm::normalize(tangent);
    glm::vec3 normal = glm::normalize(glm::cross(tangent, glm::vec3(0, 1, 0)));
    glm::vec3 binormal = glm::normalize(glm::cross(normal, tangent));

    p.y += 3.0f;

    glm::vec3 up = glm::rotate(glm::vec3(0, 1, 0), rotationAmount(), tangent);

    // move left and right
    p = p+normal*positionOffset;
    mPosition = p;
    
   
  
}

// Release memory on the GPU 
void Pickup::Release() {
    m_texture.Release();
    glDeleteVertexArrays(1, &m_vao);
    vbo.Release();
}
