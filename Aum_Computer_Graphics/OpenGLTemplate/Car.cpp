#include "Common.h"

#define _USE_MATH_DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "Car.h"
#include <math.h>

#include "camera.h"
#include "CatmullRom.h"
#include "OpenAssetImportMesh.h"


Car::Car(): m_vao(0), m_numTriangles(0) {
    mCarMesh = NULL;
    mPosition = glm::vec3(0.0f, 10.0f, 150.0f);
    mScale = glm::vec3(0.2f);
    mRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    mRotationAmount = 0.0f;
    mSpeed = 0.1f;
    
    mForward = glm::vec3(0,0,1);
    mUp = glm::vec3(0,1,0);

    m_currentDistance = 0.0f;

    // mForward = glm::rotate(mForward, mRotationAxis, mRotationAmount, mUp);
}

Car::~Car() {
    delete mCarMesh;
}

// Create a unit sphere 
void Car::Create(string a_sDirectory, string a_sFilename, int slicesIn, int stacksIn) {


    mCarMesh = new COpenAssetImportMesh;
    // m_pHorseMesh->Load("resources\\models\\car\\Cartoon_Lowpoly_Car.obj"); // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
    mCarMesh->Load("resources\\models\\car\\future_car_6_FINAL.fbx"); // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
    //
    // // check if filename passed in -- if so, load texture
    //
    // m_texture.Load(a_sDirectory + a_sFilename);
    //
    // m_directory = a_sDirectory;
    // m_filename = a_sFilename;
    //
    // m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    // m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    //
    // glGenVertexArrays(1, &m_vao);
    // glBindVertexArray(m_vao);
    //
    // vbo.Create();
    // vbo.Bind();

    /* Comment
        // // Compute vertex attributes and store in VBO
        // int vertexCount = 0;
        // for (int stacks = 0; stacks < stacksIn; stacks++) {
        // 	float phi = (stacks / (float) (stacksIn - 1)) * (float) M_PI;
        // 	for (int slices = 0; slices <= slicesIn; slices++) {
        // 		float theta = (slices / (float) slicesIn) * 2 * (float) M_PI;
        // 		
        // 		glm::vec3 v = glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
        // 		glm::vec2 t = glm::vec2(slices / (float) slicesIn, stacks / (float) stacksIn);
        // 		glm::vec3 n = v;
        //
        // 		m_vbo.AddVertexData(&v, sizeof(glm::vec3));
        // 		m_vbo.AddVertexData(&t, sizeof(glm::vec2));
        // 		m_vbo.AddVertexData(&n, sizeof(glm::vec3));
        //
        // 		vertexCount++;
        //
        // 	}
        // }
        //
        // // Compute indices and store in VBO
        // m_numTriangles = 0;
        // for (int stacks = 0; stacks < stacksIn; stacks++) {
        // 	for (int slices = 0; slices < slicesIn; slices++) {
        // 		unsigned int nextSlice = slices + 1;
        // 		unsigned int nextStack = (stacks + 1) % stacksIn;
        //
        // 		unsigned int index0 = stacks * (slicesIn+1) + slices;
        // 		unsigned int index1 = nextStack * (slicesIn+1) + slices;
        // 		unsigned int index2 = stacks * (slicesIn+1) + nextSlice;
        // 		unsigned int index3 = nextStack * (slicesIn+1) + nextSlice;
        //
        // 		m_vbo.AddIndexData(&index0, sizeof(unsigned int));
        // 		m_vbo.AddIndexData(&index1, sizeof(unsigned int));
        // 		m_vbo.AddIndexData(&index2, sizeof(unsigned int));
        // 		m_numTriangles++;
        //
        // 		m_vbo.AddIndexData(&index2, sizeof(unsigned int));
        // 		m_vbo.AddIndexData(&index1, sizeof(unsigned int));
        // 		m_vbo.AddIndexData(&index3, sizeof(unsigned int));
        // 		m_numTriangles++;
        //
        // 	}
        // }
     ////////////////////
        // float scale = 5.f;
        //
        // // get the vertices
        // triangles.push_back(glm::vec3(1.151076, 2.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(3.151077, 0.801694, -7.866735) * scale);
        // triangles.push_back(glm::vec3(1.151076, 2.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(3.151077, 0.801694, -5.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 2.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 0.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 2.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 0.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(1.151076, 0.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 0.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(1.151076, 2.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 2.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 0.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(1.151076, 0.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 2.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(1.151076, 2.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(-1.848924, 1.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(3.151077, 1.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(-1.848924, 1.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(3.151077, 1.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 1.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(1.151076, 1.135028, -7.866735) * scale);
        // triangles.push_back(glm::vec3(1.151076, 1.135028, -5.866735) * scale);
        // triangles.push_back(glm::vec3(-0.848924, 1.135028, -5.866735) * scale);
        //
        // vector<glm::vec3> normals;
        // normals.push_back(glm::vec3(0.0000, 0.0000, 1.0000));
        // normals.push_back(glm::vec3(0.3162, -0.9487, 0.0000));
        // normals.push_back(glm::vec3(-0.7071, 0.7071, 0.0000));
        // normals.push_back(glm::vec3(0.4472, 0.8944, 0.0000));
        // normals.push_back(glm::vec3(0.0000, 0.0000, -1.0000));
        // normals.push_back(glm::vec3(0.0000, 1.0000, 0.0000));
        // normals.push_back(glm::vec3(-0.7071, -0.7071, 0.0000));
        // normals.push_back(glm::vec3(0.0000, -1.0000, 0.0000));
        // normals.push_back(glm::vec3(1.0000, 0.0000, 0.0000));
        //
        // vector<glm::vec2> texCoord;
        //
        // texCoord.push_back(glm::vec2(0.500000, 0.937500));
        // texCoord.push_back(glm::vec2(0.625000, 1.000000));
        // texCoord.push_back(glm::vec2(0.500000, 1.000000));
        // texCoord.push_back(glm::vec2(0.375000, 0.500000));
        // texCoord.push_back(glm::vec2(0.312500, 0.750000));
        // texCoord.push_back(glm::vec2(0.312500, 0.500000));
        // texCoord.push_back(glm::vec2(0.625000, 0.000000));
        // texCoord.push_back(glm::vec2(0.500000, 0.250000));
        // texCoord.push_back(glm::vec2(0.500000, 0.000000));
        // texCoord.push_back(glm::vec2(0.500000, 0.500000));
        // texCoord.push_back(glm::vec2(0.625000, 0.750000));
        // texCoord.push_back(glm::vec2(0.500000, 0.750000));
        // texCoord.push_back(glm::vec2(0.500000, 0.437500));
        // texCoord.push_back(glm::vec2(0.625000, 0.500000));
        // texCoord.push_back(glm::vec2(0.375000, 0.250000));
        // texCoord.push_back(glm::vec2(0.500000, 0.312500));
        // texCoord.push_back(glm::vec2(0.375000, 0.312500));
        // texCoord.push_back(glm::vec2(0.625000, 0.812500));
        // texCoord.push_back(glm::vec2(0.500000, 0.812500));
        // texCoord.push_back(glm::vec2(0.625000, 0.312500));
        // texCoord.push_back(glm::vec2(0.375000, 0.937500));
        // texCoord.push_back(glm::vec2(0.375000, 0.812500));
        // texCoord.push_back(glm::vec2(0.875000, 0.500000));
        // texCoord.push_back(glm::vec2(0.812500, 0.750000));
        // texCoord.push_back(glm::vec2(0.375000, 0.750000));
        // texCoord.push_back(glm::vec2(0.375000, 0.437500));
        // texCoord.push_back(glm::vec2(0.375000, 0.000000));
        // texCoord.push_back(glm::vec2(0.375000, 1.000000));
        // texCoord.push_back(glm::vec2(0.125000, 0.750000));
        // texCoord.push_back(glm::vec2(0.625000, 0.437500));
        // texCoord.push_back(glm::vec2(0.625000, 0.937500));
        // texCoord.push_back(glm::vec2(0.625000, 0.250000));
        // texCoord.push_back(glm::vec2(0.687500, 0.500000));
        // texCoord.push_back(glm::vec2(0.812500, 0.500000));
        // texCoord.push_back(glm::vec2(0.687500, 0.750000));
        // texCoord.push_back(glm::vec2(0.875000, 0.750000));
        // texCoord.push_back(glm::vec2(0.125000, 0.500000));
        // texCoord.push_back(glm::vec2(0.187500, 0.500000));
        // texCoord.push_back(glm::vec2(0.187500, 0.750000));
        //
        // // vbo region
        // {
        // 	vbo.AddData(&triangles[24 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[1 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[2 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[17 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[3 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[4 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[14 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[5 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[6 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[7 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[19 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[8 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[17 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[9 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[18 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[10 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[11 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[20 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[12 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[22 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[13 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[14 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[18 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[10 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[15 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[21 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[16 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[10 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[17 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[11 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[18 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[24 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[1 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[23 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[19 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[19 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[8 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[15 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[20 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[21 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[16 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[23 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[19 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[13 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[21 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[14 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[22 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[23 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[12 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[24 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[14 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[11 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[23 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[19 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[20 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[12 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[25 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[23 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[19 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[14 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[22 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[22 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[13 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[4 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[26 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[17 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[9 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[15 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[27 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[13 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[21 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[17 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[3 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[28 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[6 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[14 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[5 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[29 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[21 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[16 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[26 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[10 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[17 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[16 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[30 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[21 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[16 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[15 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[20 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[24 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[1 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[12 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[31 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[2 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[4 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[25 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[14 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[5 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[7 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[32 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[19 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[8 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[25 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[4 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[18 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[10 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[18 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[10 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[14 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[11 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[20 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[12 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[25 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[18 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[10 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[22 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[13 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[16 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[30 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[14 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[15 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[19 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[8 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[21 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[16 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[11 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[18 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[12 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[31 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[24 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[1 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[19 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[8 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[32 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[15 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[20 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[23 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[19 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[24 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[1 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[13 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[21 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[14 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[16 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[33 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[15 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[34 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[14 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[15 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[34 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[23 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[11 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[35 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[11 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[14 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[23 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[36 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[12 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[24 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[12 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[24 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[11 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[35 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[14 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[3 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[11 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[11 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[18 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[23 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[19 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[4 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[25 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[20 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[12 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[23 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[19 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[22 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[13 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[18 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[10 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[2 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[4 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[17 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[9 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[19 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[8 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[15 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[7 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[13 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[21 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[24 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[1 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[17 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[3 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[37 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[10 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[38 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[6 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[14 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[5 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[13 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[39 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[29 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[1 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[29 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[6 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[37 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[6 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[8 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[21 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[16 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[22 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[13 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[9 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[26 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[16 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[30 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[22 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[13 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&triangles[21 - 1], sizeof(glm::vec3));
        // 	vbo.AddData(&texCoord[16 - 1], sizeof(glm::vec2));
        // 	vbo.AddData(&normals[5 - 1], sizeof(glm::vec3));
        // }
        //
        //
        // vbo.UploadDataToGPU(GL_STATIC_DRAW);
        //
        // GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
        //
        // // Vertex positions
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
        // // Texture coordinates
        // glEnableVertexAttribArray(1);
        // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
        // // Normal vectors
        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    */

}

// Render the sphere as a set of triangles
void Car::Render() {
    // glBindVertexArray(m_vao);
    // // m_texture.Bind();
    // // glDrawElements(GL_TRIANGLES, m_numTriangles * 3, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, triangles.size());

    mCarMesh->Render();

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

    p.y += 3.0f;

    glm::vec3 up = glm::rotate(glm::vec3(0, 1, 0), rotationAmount(), tangent);

    // car->setPosition(pNext);
    // move left and right
    p = p+normal*positionOffset;
    mPosition = p;
    
    // Set(p, p + 10.0f * tangent, up);

    // place car in front of camera for third person
    // glm::vec3 positionOffset = glm::vec3(1.f); 
    // mPosition = camera->GetPosition() +  ;
    
    //
    // if (GetKeyState(VK_UP) & 0x80) {
    //     Advance(1.0 );
    // }
    //
    // if (GetKeyState(VK_DOWN) & 0x80) {
    //     Advance(-1.0 );
    // }
    //
    // if (GetKeyState(VK_LEFT) & 0x80) {
    //     // Strafe(-1.0 * dt);
    //     mRotationAmount -= dt * 0.001f;
    //     // mForward = glRotatef(mRotationAmount, mRotationAxis.x,mRotationAxis.y,mRotationAxis.z);
    // }
    //
    // if (GetKeyState(VK_RIGHT) & 0x80) {
    //     // Strafe(1.0 * dt);
    //     mRotationAmount += dt * 0.001f;
    // }
    //
  
}

// Release memory on the GPU 
void Car::Release() {
    m_texture.Release();
    glDeleteVertexArrays(1, &m_vao);
    vbo.Release();
}
