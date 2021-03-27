#pragma once

#include "./include/glm/gtc/type_ptr.hpp"
#include "./include/glm/gtc/matrix_transform.hpp"

class CCatmullRom;

class CCamera {
public:
    CCamera(); // Constructor - sets default values for camera position, viewvector, upvector, and speed
    ~CCamera(); // Destructor

    glm::vec3 GetPosition() const; // Gets the position of the camera centre of projection
    glm::vec3 GetView() const; // Gets the position of the camera view point
    glm::vec3 GetUpVector() const; // Gets the camera up vector
    glm::vec3 GetStrafeVector() const; // Gets the camera strafe vector
    glm::mat4* GetPerspectiveProjectionMatrix(); // Gets the camera perspective projection matrix
    glm::mat4* GetOrthographicProjectionMatrix(); // Gets the camera orthographic projection matrix
    glm::mat4 GetViewMatrix(); // Gets the camera view matrix - note this is not stored in the class but returned using glm::lookAt() in GetViewMatrix()

    // Set the camera position, viewpoint, and up vector
    void Set(const glm::vec3& position, const glm::vec3& viewpoint, const glm::vec3& upVector);

    // Rotate the camera viewpoint -- this effectively rotates the camera
    void RotateViewPoint(float angle, const glm::vec3& viewPoint);

    // Respond to mouse movement to rotate the camera
    void SetViewByMouse();

    
    void SetViewBySpline(double dt, CCatmullRom* catmullRom);

    // Respond to keyboard presses on arrow keys to translate the camera
    void TranslateByKeyboard(double dt);

    // Strafe the camera (move it side to side)
    void Strafe(double direction);

    // Advance the camera (move it forward or backward)
    void Advance(double direction);

    // Update the camera
    void Update(double dt, CCatmullRom* catmullRom);

    // Set the projection matrices
    void SetPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane);
    void SetOrthographicProjectionMatrix(int width, int height);

    glm::mat3 ComputeNormalMatrix(const glm::mat4& modelViewMatrix);

    void addSpeed (float speed) {
        m_cameraSpeed += speed;
    }

    void setSpeed (float speed) {
        m_cameraSpeed = speed;
    }

    float getSpeed() {
        return m_cameraSpeed;
    }

    float collisionRadius(){return mCollisionRadius;}

    void ChangeCameraType() {

        cameraType+=1;
        if(cameraType>FREE_VIEW) {
            cameraType = 0;
        }
    }

    void setXOffset(float offset) {
        mPositionOffset = offset;
    }

private:
    glm::vec3 m_position; // The position of the camera's centre of projection
    glm::vec3 m_view; // The camera's viewpoint (point where the camera is looking)
    glm::vec3 m_upVector; // The camera's up vector
    glm::vec3 m_strafeVector; // The camera's strafe vector

    float m_speed; // How fast the camera moves

    glm::mat4 m_perspectiveProjectionMatrix; // Perspective projection matrix
    glm::mat4 m_orthographicProjectionMatrix; // Orthographic projection matrix


    float m_cameraRotation;
    float m_cameraSpeed;
    float m_currentDistance;
    

    // offset from car in first person
    const float firstPersonCameraOffsetFromCar = 10;
    
    static const glm::uint FIRST_PERSON = 0;
    static const glm::uint THIRD_PERSON = 1;
    static const glm::uint SIDE_VIEW = 2;
    static const glm::uint OTHER_SIDE_VIEW = 3;
    static const glm::uint TOP_VIEW = 4;
    static const glm::uint FREE_VIEW = 5;
    int cameraType = 0;
    float mPositionOffset = 0;

    float mCollisionRadius = 2.f;;


};
