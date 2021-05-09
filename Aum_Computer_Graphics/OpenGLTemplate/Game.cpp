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
#include "FrameBufferObject.h"

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
    m_pPlane = NULL;

    m_dt = 0.0;
    m_framesPerSecond = 0;
    m_frameCount = 0;
    m_elapsedTime = 0.0f;

    m_pFBO = NULL;
    mRock = NULL;


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
    delete m_pPlane;
    delete mRock;
    // delete m_player;

    // deleting the prisms in the world
    // if (mWorldPrisms != nullptr) {
    for (unsigned int i = 0; i < mWorldPrisms.size(); i++)
        delete (mWorldPrisms)[i].prism;
    // } 

    if (m_pShaderPrograms != NULL) {
        for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
            delete (*m_pShaderPrograms)[i];
    }
    delete m_pShaderPrograms;

    //setup objects
    delete m_pHighResolutionTimer;

    delete m_pFBO;
}

void Game::createWorldPrisms() {
    vector<std::string> prismsToGenerate;
    prismsToGenerate.push_back("r5h20s3");
    prismsToGenerate.push_back("r10h10s5");
    prismsToGenerate.push_back("r20h4s8");
    prismsToGenerate.push_back("r50h1s6");
    prismsToGenerate.push_back("r40h3s3");
    prismsToGenerate.push_back("r7h20s10");


    for (auto& toGenerate : prismsToGenerate) {
        WorldPrism prism;
        prism.name = toGenerate;
        prism.prism = new Prism;

        // interpret the input string to create a prism
        auto rPos = toGenerate.find('r');
        auto hPos = toGenerate.find('h');
        auto sPos = toGenerate.find('s');

        auto sradius = toGenerate.substr(rPos + 1, hPos - rPos - 1);
        auto sheight = toGenerate.substr(hPos + 1, sPos - hPos - 1);
        auto ssideCount = toGenerate.substr(sPos + 1, toGenerate.size() - sPos - 1);
        auto radius = std::stof(sradius);
        auto height = std::stof(sheight);
        auto sideCount = std::stof(ssideCount);

        prism.prism->Create(radius, height, sideCount);


        mWorldPrisms.push_back(prism);
    }
}

void Game::generateWorldPrismPositions(int count) {
    // generate a position that is not on the track or too close to another prism.
    // Lower and higher bound for random positions
    const float LO = -200.f;
    const float HI = 200.f;
    const float height = 0.f;
    // string prismName = mWorldPrisms[rand() % mWorldPrisms.size()].name;

    for (int i = 0; i < count; ++i) {
        bool foundPosition = false;
        glm::vec3 pos;
        while (!foundPosition) {
            // avoid distance(path.controlPoint , pos) < path.width)
            //https://stackoverflow.com/questions/686353/random-float-number-generation

            float x = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
            float y = height;
            float z = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
            pos = glm::vec3(x, y, z);

            foundPosition = true;
            for (auto& pathPoint : m_pCatmullRom->GetControlPoints()) {
                if (glm::distance(pos, pathPoint) < m_pCatmullRom->GetWidth() * 3) {
                    foundPosition = false;
                    break;
                }
            }
        }
        worldPrismsHeightScales.push_back(glm::vec3(1.f, 1.f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (10.f - 1.f))), 1.f));
        // we have the position and the index of the shape it will use
        worldPrismsPositions.push_back(pos);
        worldPrismsIndexes.push_back(rand() % mWorldPrisms.size());
    }

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
    mPrism = new Prism;
    m_pHorseMesh = new COpenAssetImportMesh;
    m_pSphere = new CSphere;
    mCar = new Car;
    m_pAudio = new CAudio;
    m_pCatmullRom = new CCatmullRom;
    m_pFBO = new CFrameBufferObject;
    m_pPlane = new CPlane;
    mRock = new GameObject;

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
    sShaderFileNames.push_back("radialBlur.vert");
    sShaderFileNames.push_back("radialBlur.frag");
    sShaderFileNames.push_back("hudShader.vert");
    sShaderFileNames.push_back("hudShader.frag");

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

    // Create the radialBlur shader program
    CShaderProgram* pRadialBlurProgram = new CShaderProgram;
    pRadialBlurProgram->CreateProgram();
    pRadialBlurProgram->AddShaderToProgram(&shShaders[4]);
    pRadialBlurProgram->AddShaderToProgram(&shShaders[5]);
    pRadialBlurProgram->LinkProgram();
    m_pShaderPrograms->push_back(pRadialBlurProgram);

    // Create the hud shader program
    CShaderProgram* pHudProgram = new CShaderProgram;
    pHudProgram->CreateProgram();
    pHudProgram->AddShaderToProgram(&shShaders[6]);
    pHudProgram->AddShaderToProgram(&shShaders[7]);
    pHudProgram->LinkProgram();
    m_pShaderPrograms->push_back(pHudProgram);

    // You can follow this pattern to load additional shaders

    // Create the skybox
    // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
    m_pSkybox->Create(2500.0f);

    // Create the planar terrain
    // https://3djungle.net/textures/ground/7137/
    m_pPlanarTerrain->Create("resources\\textures\\", "Ground Seamless Texture #7137 512x512.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

    // Create the plane for the tv
    // temp texture as the fbo texture will override it
    m_pPlane->Create("resources\\textures\\", "grassfloor01.jpg", 40.0f, 30.0f, 1.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

    m_pFtFont->LoadSystemFont("arial.ttf", 32);
    m_pFtFont->SetShaderProgram(pFontProgram);

    // Load some meshes in OBJ format
    m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj"); // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
    m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj"); // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013

    // create player
    // m_player->Initialise();

    // Create a sphere
    m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

    mRock->Create("resources\\models\\Rock_6\\Rock_6.fbx");
    // create car
    // mCar->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    mCar->Create(); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

    // mPickup->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    // mGOPickup->Create();
    mPickup->Create();
    mSpeedPowerUp->Create(1.f, 1.f, 1.f);
    mPrism->Create(10.f, 10.f, 20.f);


    // cull faces
    glEnable(GL_CULL_FACE);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Initialise audio and play background music
    m_pAudio->Initialise();
    m_pAudio->LoadEventSound("Resources\\Audio\\Boing.wav"); // Royalty free sound from freesound.org
    m_pAudio->LoadMusicStream("Resources\\Audio\\DST-Garote.mp3"); // Royalty free music from http://www.nosoapradio.us/
    // m_pAudio->PlayMusicStream();

    m_pFBO->Create(width, height);

    // glm::vec3 p0 = glm::vec3(-500, 10, -200);
    // glm::vec3 p1 = glm::vec3(0, 10, -200);
    // glm::vec3 p2 = glm::vec3(0, 10, 200);
    // glm::vec3 p3 = glm::vec3(-500, 10, 200);
    m_pCatmullRom->CreateCentreline();
    m_pCatmullRom->CreateOffsetCurves();
    m_pCatmullRom->CreateTrack();

    pickupPositions.push_back(mPickup->position());
    pickupPositions.push_back(glm::vec3(10, 10, 0));
    // pickupPositions.push_back(m_pCatmullRom->GeneratePositionOnPath());
    CreatePickups(50);

    // needs to be called after creating the catmull rom path otherwise the prisms will go through path
    // creating multiple sized prisms to be reused
    createWorldPrisms();
    // creating position points to use those prisms
    generateWorldPrismPositions(50);

    mPlayerXOffsetLimit = m_pCatmullRom->GetWidth() / 2.f;

    // CREATING QUAD
    // https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/5.1.framebuffers/framebuffers.cpp   
    float quadVertices[] = {
        // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    // screen quad VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    mHudManager.Create();

    // HUD SETUP
    mHudTexture.Load("resources\\textures\\hud\\santa_png\\santa.png");

    mHudTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    mHudTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mHudTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    mHudTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
}

// Render method runs repeatedly in a loop
void Game::Render() {
    m_pFBO->Bind();
    RenderScene(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RenderScene(1);


    RECT dimensions = m_gameWindow.GetDimensions();
    int height = dimensions.bottom - dimensions.top;
    int width = dimensions.right - dimensions.left;
    if (width<0) {
        width*=-1;
    }



    // 2d rendering
    // radial blur shader
    CShaderProgram* pBlur = (*m_pShaderPrograms)[2];
    pBlur->UseProgram();

    // THIS ONE
    // 2d quad rendering the blur
    if (mSpeedPowerUpTimer > 0) {
        glBindVertexArray(quadVAO);
        m_pFBO->BindTexture(0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // Render HUD
    CShaderProgram* pHudShader = (*m_pShaderPrograms)[3];
    pHudShader->UseProgram();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Draw hud
    // Disable depth testing to render the hud items on top of each other
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(quadVAO);
    mHudTexture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    mHudManager.Render(pHudShader);

    // Font Shader
    CShaderProgram* fontProgram = (*m_pShaderPrograms)[1];
    fontProgram->UseProgram();

    // Use the font shader program and render the text
    // fontProgram->UseProgram();
    glDisable(GL_DEPTH_TEST);
    fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
    fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
    fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_pFtFont->Render(20, height - 40, 20, "Score: %i", mPlayerScore);
    m_pFtFont->Render(100,  40, 100, "%i", mPlayerScore);
	if(isGameOver)
		m_pFtFont->Render(width/2 - 250,  height/2 - 25, 100, "Game Over");
    // m_pFtFont->Render(20, height - 40, 20, "Score: %f", currentSpeed);

    // Draw the 2D graphics after the 3D graphics
    DisplayFrameRate();

    // Swap buffers to show the rendered image
    SwapBuffers(m_gameWindow.Hdc());

}

void Game::RenderScene(int pass) {

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
    pMainProgram->SetUniform("isFloor", false);


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
    pMainProgram->SetUniform("light1.La", glm::vec3(.5f, .5f, .5f)); // Ambient colour of light
    pMainProgram->SetUniform("light1.Ld", glm::vec3(.5f, .5f, .5f)); // Diffuse colour of light
    pMainProgram->SetUniform("light1.Ls", glm::vec3(.5f, .5f, .5f)); // Specular colour of light
    pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f)); // Ambient material reflectance
    pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f)); // Diffuse material reflectance
    pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f)); // Specular material reflectance
    pMainProgram->SetUniform("material1.shininess", 15.0f); // Shininess material property

    // Fog's colour
    pMainProgram->SetUniform("skyColour", skyColour); // Shininess material property

    // player point light
    pMainProgram->SetUniform("pointLight1.position", viewMatrix * glm::vec4(mCar->position().x, mCar->position().y + 2.f, mCar->position().z, 1.f));
    pMainProgram->SetUniform("pointLight1.La", glm::vec3(0.f, 1.f, 0.f));
    pMainProgram->SetUniform("pointLight1.Ld", glm::vec3(0.f, 1.f, 0.f));
    pMainProgram->SetUniform("pointLight1.Ls", glm::vec3(0.f, 1.f, 0.f));
    pMainProgram->SetUniform("pointLight1.constant", 1.0f); // keep 1
    // to increase the spread of light, decrease these numbers
    pMainProgram->SetUniform("pointLight1.linear", 0.35f); // depends on distance you want
    pMainProgram->SetUniform("pointLight1.quadratic", 0.44f); //
    pMainProgram->SetUniform("pointLight1.intensity", 7.f); //




    // Render the skybox and terrain with full ambient reflectance 
    modelViewMatrixStack.Push();
    {
        pMainProgram->SetUniform("renderSkybox", true);
        pMainProgram->SetUniform("isFloor", true);
        // Translate the modelview matrix to the camera eye point so skybox stays centred around camera
        glm::vec3 vEye = m_pCamera->GetPosition();
        modelViewMatrixStack.Translate(vEye);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pSkybox->Render();
        pMainProgram->SetUniform("renderSkybox", false);
        pMainProgram->SetUniform("isFloor", false);
    }
    modelViewMatrixStack.Pop();

    // Render the planar terrain
    modelViewMatrixStack.Push();
    {
        pMainProgram->SetUniform("isFloor", true);

        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pPlanarTerrain->Render();
        pMainProgram->SetUniform("isFloor", false);
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

    // Render the Rocks 
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(glm::vec3(500.0f, 0.0f, 500.0f));
        modelViewMatrixStack.Scale(0.5f);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        mRock->Render();
    }
    modelViewMatrixStack.Pop();
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(glm::vec3(400.0f, 0.0f, 0.0f));
        modelViewMatrixStack.Scale(0.1f);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        mRock->Render();
    }
    modelViewMatrixStack.Pop();
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(glm::vec3(400.0f, 0.0f, -100.0f));
        modelViewMatrixStack.Scale(0.2f);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        mRock->Render();
    }
    modelViewMatrixStack.Pop();
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(glm::vec3(-500.0f, 0.0f, 200.0f));
        modelViewMatrixStack.Scale(0.5f);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        mRock->Render();
    }
    modelViewMatrixStack.Pop();
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(glm::vec3(-300.0f, 0.0f, 0.0f));
        modelViewMatrixStack.Scale(0.1f);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        mRock->Render();
    }
    modelViewMatrixStack.Pop();
    modelViewMatrixStack.Push();
    {
        modelViewMatrixStack.Translate(glm::vec3(100.0f, 0.0f, -200.0f));
        modelViewMatrixStack.Scale(0.2f);
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        mRock->Render();
    }
    modelViewMatrixStack.Pop();


    pMainProgram->SetUniform("numberOfLights", (int)(pickupPositions.size() + speedPowerUpPositions.size()));
    for (int i = 0; i < pickupPositions.size(); ++i) {
        // render the series of pickups
        pMainProgram->SetUniform("pointLights[" + to_string(i) + "].position", viewMatrix * glm::vec4(pickupPositions[i].x, pickupPositions[i].y + 3.f, pickupPositions[i].z, 1.f));
        pMainProgram->SetUniform("pointLights[" + to_string(i) + "].La", glm::vec3(0.57f, 0.64f, 0.12f));
        pMainProgram->SetUniform("pointLights[" + to_string(i) + "].Ld", glm::vec3(0.57f, 0.64f, 0.12f));
        pMainProgram->SetUniform("pointLights[" + to_string(i) + "].Ls", glm::vec3(0.57f, 0.64f, 0.12f));
        pMainProgram->SetUniform("pointLights[" + to_string(i) + "].constant", 1.0f); // keep 1
        // to increase the spread of light, decrease these numbers
        pMainProgram->SetUniform("pointLights[" + to_string(i) + "].linear", 0.22f); // depends on distance you want
        pMainProgram->SetUniform("pointLights[" + to_string(i) + "].quadratic", 0.20f); //
        pMainProgram->SetUniform("pointLights[" + to_string(i) + "].intensity", 5.f); //
    }
    for (int i = 0; i < speedPowerUpPositions.size(); ++i) {
        // render the series of pickups
        pMainProgram->SetUniform("pointLights[" + to_string(i + pickupPositions.size()) + "].position", viewMatrix * glm::vec4(speedPowerUpPositions[i].x, speedPowerUpPositions[i].y + 3.f, speedPowerUpPositions[i].z, 1.f));
        pMainProgram->SetUniform("pointLights[" + to_string(i + pickupPositions.size()) + "].La", glm::vec3(0.10f, 0.30f, 0.90f));
        pMainProgram->SetUniform("pointLights[" + to_string(i + pickupPositions.size()) + "].Ld", glm::vec3(0.10f, 0.30f, 0.90f));
        pMainProgram->SetUniform("pointLights[" + to_string(i + pickupPositions.size()) + "].Ls", glm::vec3(0.10f, 0.30f, 0.90f));
        pMainProgram->SetUniform("pointLights[" + to_string(i + pickupPositions.size()) + "].constant", 1.0f); // keep 1
        // to increase the spread of light, decrease these numbers
        pMainProgram->SetUniform("pointLights[" + to_string(i + pickupPositions.size()) + "].linear", 0.22f); // depends on distance you want
        pMainProgram->SetUniform("pointLights[" + to_string(i + pickupPositions.size()) + "].quadratic", .20f); //
        pMainProgram->SetUniform("pointLights[" + to_string(i + pickupPositions.size()) + "].intensity", 7.f); //
    }


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
        }
        modelViewMatrixStack.Pop();
    }


    // Render the collision sphere on basic pickup
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


    // Render the collision sphere for speed powerup
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
        mPrism->Render();
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
    }
    modelViewMatrixStack.Pop();

    // Render the WORLDPrisms
    for (int i = 0; i < worldPrismsPositions.size(); ++i) {

        modelViewMatrixStack.Push();
        {
            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
            modelViewMatrixStack.Translate(worldPrismsPositions[i]);
            // modelViewMatrixStack.Translate(30,30,30);
            modelViewMatrixStack.Rotate(mWorldPrisms[worldPrismsIndexes[i]].prism->rotationAxis(), mWorldPrisms[worldPrismsIndexes[i]].prism->rotationAmount());
            modelViewMatrixStack.Scale(mWorldPrisms[worldPrismsIndexes[i]].prism->scale());
            modelViewMatrixStack.Scale(worldPrismsHeightScales[i]);
            pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
            pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
            // To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
            //pMainProgram->SetUniform("bUseTexture", false);
            mWorldPrisms[worldPrismsIndexes[i]].prism->Render();
            // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
        }
        modelViewMatrixStack.Pop();
    }


    modelViewMatrixStack.Push();
    {
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        pMainProgram->SetUniform("bUseTexture", true); // turn off texturing
        pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        // m_pCatmullRom->RenderCentreline();
        // m_pCatmullRom->RenderOffsetCurves();
        m_pCatmullRom->RenderTrack();
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
    }
    modelViewMatrixStack.Pop();

}


// Update method runs repeatedly with the Render method
void Game::Update() {
    // Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
    m_pCamera->Update(m_dt, m_pCatmullRom);
    mCar->Update(m_dt, m_pCatmullRom);
    float accelerant = m_dt * mMovementSpeedCarCamera;

    // Keyboard inputs
    // accelerate decelerate
    if (GetKeyState(VK_UP) & 0x80) {
        // m_pCamera->addSpeed(accelerant);
        // mCar->addSpeed(accelerant);
        accelerate(accelerant);
    }
    else if (GetKeyState(VK_DOWN) & 0x80) {
        // m_pCamera->addSpeed(-accelerant);
        // mCar->addSpeed(-accelerant);
        accelerate(-accelerant);
    }

    // left right movements
    if (GetKeyState(VK_LEFT) & 0x80) {
        mPlayerXOffset -= m_dt * mCar->getXOffsetSpeed();
        mCar->setXOffset(mPlayerXOffset);
        m_pCamera->setXOffset(mCar->getXOffset());

    }
    else if (GetKeyState(VK_RIGHT) & 0x80) {
        mPlayerXOffset += m_dt * mCar->getXOffsetSpeed();
        mCar->setXOffset(mPlayerXOffset);
        m_pCamera->setXOffset(mCar->getXOffset());
    }

    // collisions
    ManageCollisions();
    // spin pickups
    mPickup->Update(m_dt);
    mSpeedPowerUp->Update(m_dt);


    // set the speed of the camera and car
    speedSetter();
    // set UI based on the speed
    speedUISetter();


    // change sky colour
    updateSkyColour();

    // meh
    m_pAudio->Update();
    // timer for speed power up
    if (mSpeedPowerUpTimer <= 0.f) { }
    else {
        // m_pCamera->addSpeed(speed*2.f);
        // mCar->addSpeed(speed*2.f);
        mSpeedPowerUpTimer -= static_cast<float>(m_dt);
    }
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


    vector<glm::vec3> removedPickupPositions;

    for (auto p = pickupPositions.begin(); p != pickupPositions.end(); ++p) {
        bool isFound = false;
        for (auto&& toRemove : pickupsToRemove) {
            if (toRemove == p) {
                isFound = true;
                break;
            }
        }
        if (isFound) continue;

        removedPickupPositions.push_back(*p);
    }
    pickupPositions = removedPickupPositions;

    // if the pickups are all picked up
    // <=2 as there are a few pickups in the center of the world
    if (pickupPositions.size() <= 2) {
        isGameOver = true;
    }

    // SPEED COLLISIONS
    vector<vector<glm::vec3>::iterator> speedPowerUpsToRemove;

    for (auto p = speedPowerUpPositions.begin(); p != speedPowerUpPositions.end(); ++p) {
        if (CheckCollision(*p, mSpeedPowerUp->collisionRadius(), m_pCamera->GetPosition(), m_pCamera->collisionRadius())) {
            mSpeedPowerUp->showCollisionSphere = false;
            mSpeedPowerUpTimer = 1000.f;

            // note to remove pickup
            speedPowerUpsToRemove.push_back(p);
        }
        if (CheckCollision(*p, mSpeedPowerUp->collisionRadius(), mCar->position(), mCar->collisionRadius())) {
            mSpeedPowerUp->showCollisionSphere = false;
            mSpeedPowerUpTimer = 1000.f;
            // note to remove pickup
            speedPowerUpsToRemove.push_back(p);
        }
    }

    vector<glm::vec3> removedSpeedPowerUpPositions;

    for (auto p = speedPowerUpPositions.begin(); p != speedPowerUpPositions.end(); ++p) {
        bool isFound = false;
        for (auto&& toRemove : speedPowerUpsToRemove) {
            if (toRemove == p) {
                isFound = true;
                break;
            }
        }
        if (isFound) continue;

        removedSpeedPowerUpPositions.push_back(*p);
    }
    speedPowerUpPositions = removedSpeedPowerUpPositions;

}

bool Game::CheckCollision(glm::vec3 aPos, float aRadius, glm::vec3 bPos, float bRadius) {
    const float distance = glm::distance(aPos, bPos);

    if (distance < aRadius || distance < bRadius) {
        return true;
    }

    return false;
}

void Game::speedSetter() {
    if (mSpeedPowerUpTimer > 0) {
        m_pCamera->setSpeed(boostSpeed);
        mCar->setSpeed(boostSpeed);
    }
    else {
        m_pCamera->setSpeed(currentSpeed);
        mCar->setSpeed(currentSpeed);
    }
}

void Game::speedUISetter() {
    const float step = maxSpeed / 6.f;
    float cumulativeStep = 0.f;

    if (mSpeedPowerUpTimer > 0.f) {
        mHudManager.setSpeedometerIndex(6);
    }
    else {
        for (int i = 0; i < 6; ++i) {
            if (currentSpeed <= cumulativeStep) {
                mHudManager.setSpeedometerIndex(i);
                break;
            }
            cumulativeStep += step;
        }
    }
}

void Game::accelerate(float accelerant) {
    if (currentSpeed <= maxSpeed && currentSpeed >= minSpeed) {
        currentSpeed += accelerant;
    }

    // keep within the boundary
    if (currentSpeed > maxSpeed) {
        if (mSpeedPowerUpTimer <= 0.f) {
            currentSpeed = maxSpeed;
        }
    }
    if (currentSpeed < minSpeed) {
        currentSpeed = minSpeed;
    }
}

void Game::updateSkyColour() {
    // could be interpolated slower for a much less epileptic change
    const float HI = 100;
    const float LO = -100;

    float r = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
    float g = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
    float b = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
    r /= 10000;
    g /= 10000;
    b /= 10000;
    skyColour += glm::vec3(r, g, b);
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

        case 0x46: // 'F' change camera state
            m_pCamera->ChangeCameraType();
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
