#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>


CCatmullRom::CCatmullRom() {
    m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom() {}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    glm::vec3 a = p1;
    glm::vec3 b = 0.5f * (-p0 + p2);
    glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
    glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

    return a + b * t + c * t2 + d * t3;

}


void CCatmullRom::SetControlPoints() {
    // Set control points (m_controlPoints) here, or load from disk
    m_controlPoints.push_back(glm::vec3(100, 5, 0));
    m_controlPoints.push_back(glm::vec3(71, 5, 71));
    m_controlPoints.push_back(glm::vec3(0, 5, 100));
    m_controlPoints.push_back(glm::vec3(-71, 5, 71));
    m_controlPoints.push_back(glm::vec3(-100, 5, 0));
    m_controlPoints.push_back(glm::vec3(-71, 5, -71));
    m_controlPoints.push_back(glm::vec3(0, 5, -100));
    m_controlPoints.push_back(glm::vec3(71, 5, -71));


    // Optionally, set upvectors (m_controlUpVectors, one for each control point as well)
}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints() {
    int M = (int)m_controlPoints.size();

    float fAccumulatedLength = 0.0f;
    m_distances.push_back(fAccumulatedLength);
    for (int i = 1; i < M; i++) {
        fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
        m_distances.push_back(fAccumulatedLength);
    }

    // Get the distance from the last point to the first
    fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
    m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3& p, glm::vec3& up) {
    if (d < 0)
        return false;

    int M = (int)m_controlPoints.size();
    if (M == 0)
        return false;


    float fTotalLength = m_distances[m_distances.size() - 1];

    // The the current length along the control polygon; handle the case where we've looped around the track
    float fLength = d - (int)(d / fTotalLength) * fTotalLength;

    // Find the current segment
    int j = -1;
    for (int i = 0; i < (int)m_distances.size() - 1; i++) {
        if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
            j = i; // found it!
            break;
        }
    }

    if (j == -1)
        return false;

    // Interpolate on current segment -- get t
    float fSegmentLength = m_distances[j + 1] - m_distances[j];
    float t = (fLength - m_distances[j]) / fSegmentLength;

    // Get the indices of the four points along the control polygon for the current segment
    int iPrev = ((j - 1) + M) % M;
    int iCur = j;
    int iNext = (j + 1) % M;
    int iNextNext = (j + 2) % M;

    // Interpolate to get the point (and upvector)
    p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
    if (m_controlUpVectors.size() == m_controlPoints.size())
        up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));

    return true;
}


// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples) {
    glm::vec3 p, up;

    // Compute the lengths of each segment along the control polygon, and the total length
    ComputeLengthsAlongControlPoints();
    float fTotalLength = m_distances[m_distances.size() - 1];

    // The spacing will be based on the control polygon
    float fSpacing = fTotalLength / numSamples;

    // Call PointAt to sample the spline, to generate the points
    for (int i = 0; i < numSamples; i++) {
        Sample(i * fSpacing, p, up);
        m_centrelinePoints.push_back(p);
        if (m_controlUpVectors.size() > 0)
            m_centrelineUpVectors.push_back(up);

    }


    // Repeat once more for truly equidistant points
    m_controlPoints = m_centrelinePoints;
    m_controlUpVectors = m_centrelineUpVectors;
    m_centrelinePoints.clear();
    m_centrelineUpVectors.clear();
    m_distances.clear();
    ComputeLengthsAlongControlPoints();
    fTotalLength = m_distances[m_distances.size() - 1];
    fSpacing = fTotalLength / numSamples;
    for (int i = 0; i < numSamples; i++) {
        Sample(i * fSpacing, p, up);
        m_centrelinePoints.push_back(p);
        if (m_controlUpVectors.size() > 0)
            m_centrelineUpVectors.push_back(up);
    }

}


void CCatmullRom::CreateCentreline() {
    // Call Set Control Points
    // adds the control points to m_controlPoints
    SetControlPoints();

    // Call UniformlySampleControlPoints with the number of samples required
    // This will generate points along the centreline, and store them in m_centrelinePoints.
    UniformlySampleControlPoints(500);

    // Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
    // Use VAO to store state associated with vertices
    glGenVertexArrays(1, &m_vaoCentreline);
    glBindVertexArray(m_vaoCentreline);
    // Create a VBO
    CVertexBufferObject vbo;
    vbo.Create();
    vbo.Bind();
    glm::vec2 texCoord(0.0f, 0.0f);
    glm::vec3 normal(0.0f, 1.0f, 0.0f);

    for (auto& centrelinePoint : m_centrelinePoints) {
        vbo.AddData(&centrelinePoint, sizeof(glm::vec3));
        vbo.AddData(&texCoord, sizeof(glm::vec2));
        vbo.AddData(&normal, sizeof(glm::vec3));
    }

    // Upload the VBO to the GPU
    vbo.UploadDataToGPU(GL_STATIC_DRAW);
    // Set the vertex attribute locations
    GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetCurves() {

    for (int i = 0; i < m_centrelinePoints.size() - 1; ++i) {
        // w = path width
        float w = 20.f;
        glm::vec3 tangent = glm::normalize(m_centrelinePoints[i + 1] - m_centrelinePoints[i]);
        glm::vec3 normal = glm::normalize(glm::cross(tangent, glm::vec3(0, 1, 0)));
        glm::vec3 binormal = glm::normalize(glm::cross(normal, tangent));

        m_leftOffsetPoints.push_back(m_centrelinePoints[i] - (w / 2) * normal);
        m_rightOffsetPoints.push_back(m_centrelinePoints[i] + (w / 2) * normal);

    }

    // Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
    // Note it is possible to only use one VAO / VBO with all the points instead.

    // Use VAO to store state associated with vertices
    glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
    glBindVertexArray(m_vaoLeftOffsetCurve);
    // Create a VBO
    CVertexBufferObject vbo;
    vbo.Create();
    vbo.Bind();
    glm::vec2 texCoord(0.0f, 0.0f);
    glm::vec3 normal(0.0f, 1.0f, 0.0f);

    for (auto& leftOffsetPoint : m_leftOffsetPoints) {
        vbo.AddData(&leftOffsetPoint, sizeof(glm::vec3));
        vbo.AddData(&texCoord, sizeof(glm::vec2));
        vbo.AddData(&normal, sizeof(glm::vec3));
    }

    // Upload the VBO to the GPU
    vbo.UploadDataToGPU(GL_STATIC_DRAW);
    // Set the vertex attribute locations
    GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));


    // Use VAO to store state associated with vertices
    glGenVertexArrays(1, &m_vaoRightOffsetCurve);
    glBindVertexArray(m_vaoRightOffsetCurve);
    // Create a VBO
    CVertexBufferObject vbo2;
    vbo2.Create();
    vbo2.Bind();


    for (auto& rightOffsetPoint : m_rightOffsetPoints) {
        vbo2.AddData(&rightOffsetPoint, sizeof(glm::vec3));
        vbo2.AddData(&texCoord, sizeof(glm::vec2));
        vbo2.AddData(&normal, sizeof(glm::vec3));
    }

    // Upload the VBO to the GPU
    vbo2.UploadDataToGPU(GL_STATIC_DRAW);
    // Set the vertex attribute locations
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));


}


void CCatmullRom::CreateTrack() {

    // Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
    // Use VAO to store state associated with vertices
    glGenVertexArrays(1, &m_vaoTrack);
    glBindVertexArray(m_vaoTrack);

    GLsizei stride = sizeof(glm::vec3) * 3 + sizeof(glm::vec2) + sizeof(glm::vec3);

    CVertexBufferObject vboTrack;
    vboTrack.Create();
    vboTrack.Bind();
    glm::vec2 texCoord(0.0f, 0.0f);
    glm::vec3 normal(0.0f, 1.0f, 0.0f);
    // for (int i = 0; i < m_centrelinePoints.size() - 2; ++i) {
    // 	// Quad: l[i]   r[i] l[1]
    // 	//       l[i+1] r[i] r[i+1]
    //
    // 	glm::vec3 p1 = m_leftOffsetPoints[i];
    // 	glm::vec3 p2 = m_rightOffsetPoints[i];
    // 	glm::vec3 p3 = m_leftOffsetPoints[i + 1];
    //
    //
    // 	vboTrack.AddData(&p1, sizeof(glm::vec3));
    // 	vboTrack.AddData(&p2, sizeof(glm::vec3));
    // 	vboTrack.AddData(&p3, sizeof(glm::vec3));
    //
    // 	m_trackTriangles.push_back(p1);
    // 	m_trackTriangles.push_back(p2);
    // 	m_trackTriangles.push_back(p3);
    //
    // 	vboTrack.AddData(&texCoord, sizeof(glm::vec2));
    // 	vboTrack.AddData(&normal, sizeof(glm::vec3));
    // }

    // put the points in
    for (int i = 0; i < m_leftOffsetPoints.size() - 1; ++i) {
        m_trackTrianglePoints.push_back(m_leftOffsetPoints[i]);
        m_trackTrianglePoints.push_back(m_rightOffsetPoints[i]);
    }
    m_trackTrianglePoints.push_back(m_leftOffsetPoints[0]);
    m_trackTrianglePoints.push_back(m_rightOffsetPoints[0]);
    m_trackTrianglePoints.push_back(m_leftOffsetPoints[1]);
    m_trackTrianglePoints.push_back(m_rightOffsetPoints[1]);


    // one set of triangles
    for (int i = 0; i < m_trackTrianglePoints.size() - 2; ++i) {
        vboTrack.AddData(&m_trackTrianglePoints[i], sizeof(glm::vec3));
        vboTrack.AddData(&m_trackTrianglePoints[i + 1], sizeof(glm::vec3));
        vboTrack.AddData(&m_trackTrianglePoints[i + 2], sizeof(glm::vec3));
        vboTrack.AddData(&texCoord, sizeof(glm::vec2));
        vboTrack.AddData(&normal, sizeof(glm::vec3));
    }
    

    // janky fix for not having a triangle have a point at 0,0,0
    vboTrack.AddData(&m_trackTrianglePoints[0], sizeof(glm::vec3));
    vboTrack.AddData(&m_trackTrianglePoints[m_trackTrianglePoints.size()-2], sizeof(glm::vec3));
    vboTrack.AddData(&m_trackTrianglePoints[m_trackTrianglePoints.size()-1], sizeof(glm::vec3));
    vboTrack.AddData(&texCoord, sizeof(glm::vec2));
    vboTrack.AddData(&normal, sizeof(glm::vec3));
    
    vboTrack.AddData(&m_trackTrianglePoints[0], sizeof(glm::vec3));
    vboTrack.AddData(&m_trackTrianglePoints[m_trackTrianglePoints.size()-1], sizeof(glm::vec3));
    vboTrack.AddData(&m_trackTrianglePoints[1], sizeof(glm::vec3));
    vboTrack.AddData(&texCoord, sizeof(glm::vec2));
    vboTrack.AddData(&normal, sizeof(glm::vec3));	


    // Upload the VBO to the GPU
    vboTrack.UploadDataToGPU(GL_STATIC_DRAW);
    // Set the vertex attribute locations
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) * 3));

    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)((sizeof(glm::vec3) * 3) + sizeof(glm::vec2)));

}


void CCatmullRom::RenderCentreline() {
    // Bind the VAO m_vaoCentreline and render it
    glBindVertexArray(m_vaoCentreline);
    // m_texture.Bind();
    // Draw points and then a line loop
    // glDrawElements(GL_POINTS, m_centrelinePoints.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_LINE_LOOP, m_centrelinePoints.size(), GL_UNSIGNED_INT, 0);
    glPointSize(5);
    glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());
    glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
}

void CCatmullRom::RenderOffsetCurves() {
    // Bind the VAO m_vaoLeftOffsetCurve and render it
    glBindVertexArray(m_vaoLeftOffsetCurve);
    // m_texture.Bind();
    glPointSize(5);
    glDrawArrays(GL_POINTS, 0, m_leftOffsetPoints.size());
    glDrawArrays(GL_LINE_STRIP, 0, m_leftOffsetPoints.size());

    // Bind the VAO m_vaoRightOffsetCurve and render it
    glBindVertexArray(m_vaoRightOffsetCurve);
    // m_texture.Bind();
    glPointSize(5);
    glDrawArrays(GL_POINTS, 0, m_rightOffsetPoints.size());
    glDrawArrays(GL_LINE_STRIP, 0, m_rightOffsetPoints.size());
}


void CCatmullRom::RenderTrack() {
    // Bind the VAO m_vaoTrack and render it
    glBindVertexArray(m_vaoTrack);
    // m_texture.Bind();
    // glPointSize(5);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_trackTrianglePoints.size());

}

int CCatmullRom::CurrentLap(float d) {

    return (int)(d / m_distances.back());

}

glm::vec3 CCatmullRom::_dummy_vector(0.0f, 0.0f, 0.0f);
