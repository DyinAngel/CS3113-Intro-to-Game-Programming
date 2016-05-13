#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#pragma once
#include <vector>
#include <string>
#include <time.h>
#include <istream>
#include "ShaderProgram.h"
#include "Entity.h"
#include "SpriteSheet.h"
using namespace std;

enum GameState { STATE_MAIN_MENU, STATE_GAME_RUNNING, STATE_GAME_OVER };
enum CharacterState {IDLE, RUNNING, JUMPING_UP, JUMPING_DOWN, SLIDING};

class ClassDemoApp {
public:

	ClassDemoApp();
	~ClassDemoApp();

	void Initalize();

	void ReadFile(std::string levelFile);
	bool ReadHeader(ifstream &stream);
	bool ReadLayerData(ifstream &stream);
	bool ReadEntityData(ifstream &stream);
	void PlaceEntity(string type, float placeX, float placeY);

	void Render();
	void RenderMainMenu();
	void SetupGameLevels();
	void RenderGameRunning();
	void RenderGameOver();
	void UpdateAndRender();

	void Reset();
	GLuint LoadTexture(const char *image_path);
	GLuint Load24BitTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing);

	void loadAllTextures();

	GLuint spriteSheetTexturePlatforms;
	int platformX, platformY = 30;
	GLuint spriteSheetTexturePlayers;
	int playerX, playerY = 4;
	GLuint textTexture;

	int playerSpriteIndex = 0;
	int crateSpriteIndex;
	int wallSpriteIndex;
	int trapSpriteIndex;

	void DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY);
	void Draw(GLuint textureID);
	void scrollBackground();

	Entity player;
	Entity backgroundEntity; 
	float backgroundOffset = 0.025f;
	std::vector<Entity> entities;

	GameState gameState;
	CharacterState characterState;
	bool done = false;
	float currentTime;
	float ticks;
	float elapsed;
	float pace;
	int score = 0;
	float lastFrameTicks;
	int playerCurrentFrame = 0;

	SDL_Window* displayWindow;

	Mix_Music* maintheme;

	ShaderProgram* program;
	GLuint text;
	GLuint background;
	GLuint titleScreen;

	Matrix modelMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;

	int mapHeight;
	int mapWidth;
	unsigned char** levelData;
	int level;

	// Character Annimation Textures;
	SpriteSheet idle[12];
	SpriteSheet jumpDown;
	SpriteSheet jumpUp;
	SpriteSheet movement[6];
	SpriteSheet saw[4];
	SpriteSheet sliding;
	SpriteSheet boom;

	float lerp(float v0, float v1, float t);
};