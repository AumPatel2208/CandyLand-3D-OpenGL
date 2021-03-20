/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 4.0a 30/01/2016
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "GameObject.h"
#include "Sphere.h"
#include "Car.h"
#include "Pickup.h"
#include "SpeedPowerUp.h"
#include "Prism.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CatmullRom.h"

// Constructor
Game::Game() {
    m_pSkybox = NULL;
    m_pCamera = NULL;
    m_pShaderPrograms = NULL;
    m_pPlanarTerrain = NULL;
    m_pFtFont = NULL;
    m_pBarrelMesh = NULL;
    mPickup = NULL;
    mSpeedPowerUp = NULL;
    mPrism = NULL;
    m_pHorseMesh = NULL;
    m_pSphere = NULL;
    mCar = NULL;
    m_pHighResolutionTimer = NULL;
    m_pAudio = NULL;
    m_pCatmullRom = NULL;

    m_dt = 0.0;
    m_framesPerSecond = 0;
    m_frameCount = 0;
    m_elapsedTime = 0.0f;


}

// Destructor
Game::~Game() {
    //game objects
    delete m_pCamera;
    delete m_pSkybox;
    delete m_pPlanarTerrain;
    delete m_pFtFont;
    delete m_pBarrelMesh;
    delete mPickup;
    delete mSpeedPowerUp;
    delete mPrism;
    delete m_pHorseMesh;
    delete m_pSphere;
    delete mCar;
    delete m_pAudio;
    delete m_pCatmullRom;
    // delete m_player;

    if (m_pShaderPrograms != NULL) {
        for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
            delete (*m_pShaderPrograms)[i];
    }
    delete m_pShaderPrograms;

    //setup objects
    delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() {
    // Set the clear colour and depth
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);

    /// Create objects
    m_pCamera = new CCamera;
    m_pSkybox = new CSkybox;
    m_pShaderPrograms = new vector<CShaderProgram*>;
    m_pPlanarTerrain = new CPlane;
    m_pFtFont = new CFreeTypeFont;
    m_pBarrelMesh = new COpenAssetImportMesh;
    mPickup = new Pickup;
    mSpeedPowerUp = new SpeedPowerUp;
    mPrism= new Prism;
    m_pHorseMesh = new COpenAssetImportMesh;
    m_pSphere = new CSphere;
    mCar = new Car;
    m_pAudio = new CAudio;
    m_pCatmullRom = new CCatmullRom;
    // m_player = new Player;

    RECT dimensions = m_gameWindow.GetDimensions();

    int width = dimensions.right - dimensions.left;
    int height = dimensions.bottom - dimensions.top;

    // Set the orthographic and perspective projection matrices based on the image size
    m_pCamera->SetOrthographicProjectionMatrix(width, height);
    m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float)width / (float)height, 0.5f, 5000.0f);

    // Load shaders
    vector<CShader> shShaders;
    vector<string> sShaderFileNames;
    sShaderFileNames.push_back("mainShader.vert");
    sShaderFileNames.push_back("mainShader.frag");
    sShaderFileNames.push_back("textShader.vert");
    sShaderFileNames.push_back("textShader.frag");

    for (int i = 0; i < (int)sShaderFileNames.size(); i++) {
        string sExt = sShaderFileNames[i].substr((int)sShaderFileNames[i].size() - 4, 4);
        int iShaderType;
        if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
        else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
        else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
        else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
        else iShaderType = GL_TESS_EVALUATION_SHADER;
        CShader shader;
        shader.LoadShader("resources\\shaders\\" + sShaderFileNames[i], iShaderType);
        shShaders.push_back(shader);
    }

    // Create the main shader program
    CShaderProgram* pMainProgram = new CShaderProgram;
    pMainProgram->CreateProgram();
    pMainProgram->AddShaderToProgram(&shShaders[0]);
    pMainProgram->AddShaderToProgram(&shShaders[1]);
    pMainProgram->LinkProgram();
    m_pShaderPrograms->push_back(pMainProgram);

    // Create a shader program for fonts
    CShaderProgram* pFontProgram = new CShaderProgram;
    pFontProgram->CreateProgram();
    pFontProgram->AddShaderToProgram(&shShaders[2]);
    pFontProgram->AddShaderToProgram(&shShaders[3]);
    pFontProgram->LinkProgram();
    m_pShaderPrograms->push_back(pFontProgram);

    // You can follow this pattern to load additional shaders

    // Create the skybox
    // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
    m_pSkybox->Create(2500.0f);

    // Create the planar terrain
    // https://3djungle.net/textures/ground/7137/
    m_pPlanarTerrain->Create("resources\\textures\\", "Ground Seamless Texture #7137 512x512.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

    m_pFtFont->LoadSystemFont("arial.ttf", 32);
    m_pFtFont->SetShaderProgram(pFontProgram);

    // Load some meshes in OBJ format
    m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj"); // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
    m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj"); // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013

    // create player
    // m_player->Initialise();

    // Create a sphere
    m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

    // create car
    // mCar->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    mCar->Create(); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

    // mPickup->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    // mGOPickup->Create();
    mPickup->Create();
    mSpeedPowerUp->Create(1.f, 1.f, 1.f);
    mPrism->Create(10.f, 10.f,20.f);
    
    // cull faces
    // glEnable(GL_CULL_FACE);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Initialise audio and play background music
    m_pAudio->Initialise();
    m_pAudio->LoadEventSound("Resources\\Audio\\Boing.wav"); // Royalty free sound from freesound.org
    m_pAudio->LoadMusicStream("Resources\\Audio\\DST-Garote.mp3"); // Royalty free music from http://www.nosoapradio.us/
    // m_pAudio->PlayMusicStream();

    glm::vec3 p0 = glm::vec3(-500, 10, -200);
    glm::vec3 p1 = glm::vec3(0, 10, -200);
    glm::vec3 p2 = glm::vec3(0, 10, 200);
    glm::vec3 p3 = glm::vec3(-500, 10, 200);
    m_pCatmullRom->CreateCentreline();
    m_pCatmullRom->CreateOffsetCurves();
    m_pCatmullRom->CreateTrack();

    pickupPositions.push_back(mPickup->position());
    pickupPositions.push_back(glm::vec3(10, 10, 0));
    // pickupPositions.push_back(m_pCatmullRom->GeneratePositionOnPath());
    CreatePickups(20);


}

// Render method runs repeatedly in a loop
void Game::Render() {

    // Clear the buffers and enable depth testing (z-buffering)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Set up a matrix stack
    glutil::MatrixStack modelViewMatrixStack;
    modelViewMatrixStack.SetIdentity();

    // Use the main shader program 
    CShaderProgram* pMainProgram = (*m_pShaderPrograms)[0];
    pMainProgram->UseProgram();
    pMainProgram->SetUniform("bUseTexture", true);
    pMainProgram->SetUniform("sampler0", 0);
    pMainProgram->SetUniform("CubeMapTex", 1);


    // Set the projection matrix
    pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

    // Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
    // Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
    modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
    glm::mat4 viewMatrix = modelViewMatrixStack.Top();
    glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);


    // Set light and materials in main shader program
    glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
    pMainProgram->SetUniform("light1.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
    pMainProgram->SetUniform("light1.La", glm::vec3(1.0f, 1.0f,1.0f)); // Ambient colour of light
    pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f,1.0f)); // Diffuse colour of light
    pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f,1.0f)); // Specular colour of light
    pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f)); // Ambient material reflectance
    pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f)); // Diffuse material reflectance
    pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f)); // Specular material reflectance
    pMainProgram->SetUniform("material1.shininess", 15.0f); // Shininess material property


    // Render the skybox and terrain with full ambient reflectance 
    modelViewMatrixStack.Push();
    {
        pMainProgram->SetUniform("renderSkybox", true);
        // Translate the modelview matrix to the camera eye point so skybox stays centred around camera
        glm::vec3 vEye = m_pCamera->GetPosition();
        modelViewMatrixStack.Translate(vEye);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pSkybox->Render();
        pMainProgram->SetUniform("renderSkybox", false);
    }
    modelViewMatrixStack.Pop();

    // Render the planar terrain
    modelViewMatrixStack.Push();
    {
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pPlanarTerrain->Render();
    }
    modelViewMatrixStack.Pop();


    // Turn on diffuse + specular materials
    pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f)); // Ambient material reflectance
    pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f)); // Diffuse material reflectance
    pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f)); // Specular material reflectance	


    // Render the horse 
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
        modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
        modelViewMatrixStack.Scale(2.5f);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pHorseMesh->Render();
    }
    modelViewMatrixStack.Pop();

    // render player
    // m_player->Render(modelViewMatrixStack, pMainProgram, m_pCamera);

    // Render the barrel 
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(glm::vec3(100.0f, 0.0f, 0.0f));
        modelViewMatrixStack.Scale(5.0f);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pBarrelMesh->Render();
    }
    modelViewMatrixStack.Pop();

    for (glm::vec3 p : pickupPositions) {
        // Render the pickup 
        modelViewMatrixStack.Push();
        {
            // modelViewMatrixStack.Translate(mPickup->position());
            modelViewMatrixStack.Translate(p);
            modelViewMatrixStack.Translate(glm::vec3(0, mPickup->spinHeight, 0));
            modelViewMatrixStack.Rotate(mPickup->rotationAxis(), mPickup->rotationAmount());
            modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), mPickup->spinAmount);
            // modelViewMatrixStack.Scale(2.5f);
            modelViewMatrixStack.Scale(mPickup->scale());
            pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
            pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
            mPickup->Render();
            // if(mPickup->showCollisionSphere) {
            //    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //    mPickup->getCollisionSphere()->Render();
            //     // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            // }
        }
        modelViewMatrixStack.Pop();

    }


    // Render the sphere
    if (mPickup->showCollisionSphere) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        modelViewMatrixStack.Push();
        {
            modelViewMatrixStack.Translate(glm::vec3(mPickup->position()));
            modelViewMatrixStack.Translate(glm::vec3(0.0f, mPickup->collisionHeight(), 0.0f));
            modelViewMatrixStack.Scale(mPickup->collisionRadius());
            pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
            pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
            // To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
            //pMainProgram->SetUniform("bUseTexture", false);
            // m_pSphere->Render();
            mPickup->getCollisionSphere()->Render();
        }
        modelViewMatrixStack.Pop();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    // Render the car
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(mCar->position());
        modelViewMatrixStack *= mCar->getRotationOnPath();
        modelViewMatrixStack.Rotate(mCar->rotationAxis(), mCar->rotationAmount());
        modelViewMatrixStack.Scale(mCar->scale());
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        // To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
        //pMainProgram->SetUniform("bUseTexture", false);
        mCar->Render();
    }
    modelViewMatrixStack.Pop();
    
    // Render the sphere on the car
    if (mCar->showCollisionSphere) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        modelViewMatrixStack.Push();
        {
            modelViewMatrixStack.Translate(glm::vec3(mCar->position()));
            modelViewMatrixStack.Translate(glm::vec3(0.0f, mCar->collisionHeight(), 0.0f));
            modelViewMatrixStack.Scale(mCar->collisionRadius());
            // modelViewMatrixStack.Scale(mCar->collisionScale());
            modelViewMatrixStack *= mCar->getRotationOnPath();
            pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
            pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
            // To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
            //pMainProgram->SetUniform("bUseTexture", false);
            // m_pSphere->Render();
            mCar->getCollisionSphere()->Render();
        }
        modelViewMatrixStack.Pop();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Render the Speed PowerUP
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(mSpeedPowerUp->position());
        modelViewMatrixStack.Translate(glm::vec3(0, mSpeedPowerUp->spinHeight, 0));
        modelViewMatrixStack.Rotate(mSpeedPowerUp->rotationAxis(), mSpeedPowerUp->rotationAmount());
        modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), mSpeedPowerUp->spinAmount);
        modelViewMatrixStack.Scale(mSpeedPowerUp->scale());
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        // To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
        //pMainProgram->SetUniform("bUseTexture", false);
        mSpeedPowerUp->Render();
    }
    modelViewMatrixStack.Pop();
    
 

    for (glm::vec3 p : speedPowerUpPositions) {
        // Render the pickup 
        modelViewMatrixStack.Push();
        {
            modelViewMatrixStack.Translate(p);
            modelViewMatrixStack.Translate(glm::vec3(0, mSpeedPowerUp->spinHeight, 0));
            modelViewMatrixStack.Rotate(mSpeedPowerUp->rotationAxis(), mSpeedPowerUp->rotationAmount());
            modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), mSpeedPowerUp->spinAmount);
            modelViewMatrixStack.Scale(mSpeedPowerUp->scale());
            pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
            pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
            mSpeedPowerUp->Render();

        }
        modelViewMatrixStack.Pop();

    }

    
    // Render the sphere
    if (mSpeedPowerUp->showCollisionSphere) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
        modelViewMatrixStack.Push();
        {
            modelViewMatrixStack.Translate(glm::vec3(mSpeedPowerUp->position()));
            modelViewMatrixStack.Translate(glm::vec3(0, mSpeedPowerUp->spinHeight, 0));
            modelViewMatrixStack.Translate(glm::vec3(0.0f, mSpeedPowerUp->collisionHeight(), 0.0f));
            modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), mSpeedPowerUp->spinAmount);
            modelViewMatrixStack.Scale(mSpeedPowerUp->collisionRadius());
            // modelViewMatrixStack.Scale(mSpeedPowerUp->collisionScale());
            pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
            pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
            // To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
            //pMainProgram->SetUniform("bUseTexture", false);
            // m_pSphere->Render();
            mSpeedPowerUp->getCollisionSphere()->Render();
        }
        modelViewMatrixStack.Pop();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    // Render the Prism 
    modelViewMatrixStack.Push();
    {
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        modelViewMatrixStack.Translate(mPrism->position());
        modelViewMatrixStack.Rotate(mPrism->rotationAxis(), mPrism->rotationAmount());
        modelViewMatrixStack.Scale(mPrism->scale());
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        // To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
        //pMainProgram->SetUniform("bUseTexture", false);
        mPrism->Render();
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
    }
    modelViewMatrixStack.Pop();

    modelViewMatrixStack.Push();
    {
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        pMainProgram->SetUniform("bUseTexture", false); // turn off texturing
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pCatmullRom->RenderCentreline();
        m_pCatmullRom->RenderOffsetCurves();
        m_pCatmullRom->RenderTrack();
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
    }
    modelViewMatrixStack.Pop();

    CShaderProgram* fontProgram = (*m_pShaderPrograms)[1];

    RECT dimensions = m_gameWindow.GetDimensions();
    int height = dimensions.bottom - dimensions.top;
    // Use the font shader program and render the text
    fontProgram->UseProgram();
    glDisable(GL_DEPTH_TEST);
    fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
    fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
    fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_pFtFont->Render(20, height - 40, 20, "Score: %i", mPlayerScore);
    // Draw the 2D graphics after the 3D graphics
    DisplayFrameRate();

    // Swap buffers to show the rendered image
    SwapBuffers(m_gameWindow.Hdc());

}


// Update method runs repeatedly with the Render method
void Game::Update() {
    // Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
    // m_pCamera->Set(glm::vec3(0, 300, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));

    // m_car->Update(m_dt, m_pCamera);

    m_pCamera->Update(m_dt, m_pCatmullRom);
    mCar->Update(m_dt, m_pCatmullRom);
     float speed = m_dt * mMovementSpeedCarCamera;

    if (GetKeyState(VK_UP) & 0x80) {
        m_pCamera->addSpeed(speed);
        mCar->addSpeed(speed);
    }
    else if (GetKeyState(VK_DOWN) & 0x80) {
        m_pCamera->addSpeed(-speed);
        mCar->addSpeed(-speed);
    }

    if (GetKeyState(VK_LEFT) & 0x80) {
        if (mPlayerLane == 1)
            mPlayerLane = 0;
        else if (mPlayerLane == 0)
            mPlayerLane = -1;
        mPlayerOffset -= m_dt * mCar->getOffsetSpeed();
        mCar->addXOffset(mPlayerOffset);


    }
    else if (GetKeyState(VK_RIGHT) & 0x80) {
        if (mPlayerLane == -1)
            mPlayerLane = 0;
        else if (mPlayerLane == 0)
            mPlayerLane = 1;
        mPlayerOffset += m_dt * mCar->getOffsetSpeed();
        mCar->addXOffset(mPlayerOffset);
    }

    ManageCollisions();
    mPickup->Update(m_dt);
    mSpeedPowerUp->Update(m_dt);

    m_pAudio->Update();
}

void Game::ManageCollisions() {

    // PICKUP COLLISIONS
    vector<vector<glm::vec3>::iterator> pickupsToRemove;

    for (auto p = pickupPositions.begin(); p != pickupPositions.end(); ++p) {
        if (CheckCollision(*p, mPickup->collisionRadius(), m_pCamera->GetPosition(), m_pCamera->collisionRadius())) {
            mPickup->showCollisionSphere = false;
            mPlayerScore += 1;
            // note to remove pickup
            pickupsToRemove.push_back(p);
        }
        if (CheckCollision(*p, mPickup->collisionRadius(), mCar->position(), mCar->collisionRadius())) {
            mPickup->showCollisionSphere = false;
            mPlayerScore += 1;
            // note to remove pickup
            pickupsToRemove.push_back(p);
        }
    }

    // remove pickup
    for (auto& i : pickupsToRemove) {
        pickupPositions.erase(i);
    }

    // PICKUP COLLISIONS
    vector<vector<glm::vec3>::iterator> speedPowerUpsToRemove;

    for (auto p = speedPowerUpPositions.begin(); p != speedPowerUpPositions.end(); ++p) {
        if (CheckCollision(*p, mSpeedPowerUp->collisionRadius(), m_pCamera->GetPosition(), m_pCamera->collisionRadius())) {
            mSpeedPowerUp->showCollisionSphere = false;
            // increase speed
            // mMovementSpeedCarCamera += 0.002f;
            
            // note to remove pickup
            speedPowerUpsToRemove.push_back(p);
        }
        if (CheckCollision(*p, mSpeedPowerUp->collisionRadius(), mCar->position(), mCar->collisionRadius())) {
            mSpeedPowerUp->showCollisionSphere = false;
            // mMovementSpeedCarCamera += 0.002f;
            // note to remove pickup
            speedPowerUpsToRemove.push_back(p);
        }
    }

    // remove pickup
    for (auto& i : speedPowerUpsToRemove) {
        speedPowerUpPositions.erase(i);
    }

    

}

bool Game::CheckCollision(glm::vec3 aPos, float aRadius, glm::vec3 bPos, float bRadius) {
    const float distance = glm::distance(aPos, bPos);

    if (distance < aRadius || distance < bRadius) {
        return true;
    }

    return false;
}


void Game::DisplayFrameRate() {


    CShaderProgram* fontProgram = (*m_pShaderPrograms)[1];

    RECT dimensions = m_gameWindow.GetDimensions();
    int height = dimensions.bottom - dimensions.top;

    // Increase the elapsed time and frame counter
    m_elapsedTime += m_dt;
    m_frameCount++;

    // Now we want to subtract the current time by the last time that was stored
    // to see if the time elapsed has been over a second, which means we found our FPS.
    if (m_elapsedTime > 1000) {
        m_elapsedTime = 0;
        m_framesPerSecond = m_frameCount;

        // Reset the frames per second
        m_frameCount = 0;
    }

    if (m_framesPerSecond > 0) {
        // Use the font shader program and render the text
        fontProgram->UseProgram();
        glDisable(GL_DEPTH_TEST);
        fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
        fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
    }
}

// The game loop runs repeatedly until game over
void Game::GameLoop() {
    /*
    // Fixed timer
    dDt = pHighResolutionTimer->Elapsed();
    if (dDt > 1000.0 / (double) Game::FPS) {
        pHighResolutionTimer->Start();
        Update();
        Render();
    }
    */


    // Variable timer
    m_pHighResolutionTimer->Start();
    Update();
    Render();
    m_dt = m_pHighResolutionTimer->Elapsed();


}

void Game::CreatePickups(int amount) {
    for (int i = 0; i < amount; ++i) {
        pickupPositions.push_back(m_pCatmullRom->GeneratePositionOnPath());
    }
    
    for (int i = 0; i < amount; ++i) {
        speedPowerUpPositions.push_back(m_pCatmullRom->GeneratePositionOnPath());
    }
}


WPARAM Game::Execute() {
    m_pHighResolutionTimer = new CHighResolutionTimer;
    m_gameWindow.Init(m_hInstance);

    if (!m_gameWindow.Hdc()) {
        return 1;
    }

    Initialise();

    m_pHighResolutionTimer->Start();


    MSG msg;

    while (1) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (m_appActive) {
            GameLoop();
        }
        else Sleep(200); // Do not consume processor power if application isn't active
    }

    m_gameWindow.Deinit();

    return (msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
    LRESULT result = 0;

    switch (message) {


    case WM_ACTIVATE: {
        switch (LOWORD(w_param)) {
        case WA_ACTIVE:
        case WA_CLICKACTIVE:
            m_appActive = true;
            m_pHighResolutionTimer->Start();
            break;
        case WA_INACTIVE:
            m_appActive = false;
            break;
        }
        break;
    }

    case WM_SIZE:
        RECT dimensions;
        GetClientRect(window, &dimensions);
        m_gameWindow.SetDimensions(dimensions);
        break;

    case WM_PAINT:
        PAINTSTRUCT ps;
        BeginPaint(window, &ps);
        EndPaint(window, &ps);
        break;

    case WM_KEYDOWN:
        switch (w_param) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case '1':
            m_pAudio->PlayEventSound();
            break;

        case 0x46:
            m_pCamera->FlipCameraState();
            break;

        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        result = DefWindowProc(window, message, w_param, l_param);
        break;
    }

    return result;
}

Game& Game::GetInstance() {
    static Game instance;

    return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) {
    m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
    return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) {
    Game& game = Game::GetInstance();
    game.SetHinstance(hinstance);

    return game.Execute();
}
