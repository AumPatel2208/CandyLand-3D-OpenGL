#pragma once


#include "Common.h"
#include "GameWindow.h"
#include "HudManager.h"
#include "Texture.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.
class CFrameBufferObject;
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class GameObject;
class Car;
class Pickup;
class SpeedPowerUp;
class Prism;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;

struct LightInfo {
	glm::vec4 position;
	glm::vec3 La;
	glm::vec3 Ld;
	glm::vec3 Ls;

	// https://learnopengl.com/Lighting/Light-casters
	float constant;
	float linear;
	float quadratic;
	float intensity;
};

class Game {
	struct WorldPrism {
		string name;
		Prism* prism;
	};
	
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();
	void RenderMenu(int pass);
	void RenderScene(int pass);

	
	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	CSphere *m_pSphere;
	Car *mCar;
	// Pickup *mPickup;
	// GO_Pickup *mGOPickup;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CPlane *m_pPlane;

	CCatmullRom *m_pCatmullRom;
	// Player* m_player;
	Pickup *mPickup;
	
	SpeedPowerUp* mSpeedPowerUp;
	GameObject* mRock;
	Prism* mPrism;
	vector<WorldPrism> mWorldPrisms;
	vector<glm::vec3> pickupPositions;
	vector<glm::vec3> speedPowerUpPositions;
	vector<glm::vec3> worldPrismsPositions;
	vector<glm::vec3> worldPrismsHeightScales;
	// vector<string> worldPrismsPositionsNames;
	vector<int> worldPrismsIndexes;
	glm::vec3 skyColour = glm::vec3(0.5f,0.5f,0.5f);
	CFrameBufferObject *m_pFBO;

	vector<LightInfo> mPointLights;
	// glm::vec3 generateWorldPrismScale();
	


	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;
	bool isWireFrame = false;
	int mPlayerScore = 0;


public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();
	void ManageCollisions();
	bool CheckCollision(glm::vec3 aPos, float aRadius, glm::vec3 bPos, float bRadius);
	

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	void CreatePickups(int amount);
	void createWorldPrisms();
	void generateWorldPrismPositions(int count);
	void speedSetter();
	void speedUISetter();
	void accelerate(float accelerant);
	void updateSkyColour();
	const float maxSpeed = 0.1f;
	const float minSpeed = 0.f;
	float currentSpeed = 0.f;
	const float boostSpeed = 0.2f;
	float mMovementSpeedCarCamera = 0.0001f;
	

	
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
	double mSeconds = 0;

	int mPlayerLane = 0; // -1 is left, +1 is right
	float mPlayerXOffset = 0.f;
	// const float mPlayerXOffsetLimit = 10.f;
	float mPlayerXOffsetLimit;
	// glm::mat4 prevModelViewProj;
	float mSpeedPowerUpTimer = 0.f;
	
	unsigned int quadVAO, quadVBO;

	bool isGameOver = false;
	
	// HUD
	CTexture mHudTexture;

	HudManager mHudManager = {};





};
