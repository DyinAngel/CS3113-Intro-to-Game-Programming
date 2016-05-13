#include <SDL_mixer.h>
#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include "ClassDemoApp.h"
#include "Entity.h"
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

ClassDemoApp::ClassDemoApp() {
	gameState = STATE_MAIN_MENU;
	Initalize();
	done = false;
}

ClassDemoApp::~ClassDemoApp() {
	delete program;
	Mix_FreeMusic(maintheme);
	SDL_Quit();
}

void ClassDemoApp::Initalize() {
	// SDL Setup
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_OPENGL);

	// OpenGL Setup
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif
	
	// Setup Projection
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, 640, 640);

	// Program setup;
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	projectionMatrix.setOrthoProjection(-4.0f, 4.0, -4.0f, 4.0f, -1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUseProgram(program->programID);

	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	loadAllTextures();

	// Made the width huge as he said to ensure absolutely
	// Maximum looping.
	// No idea how to loop around will look into it later
	backgroundEntity = Entity(SpriteSheet(background, .1, 1, 10000, 1, 8.0f));

	lastFrameTicks = 0.0f;
	score = 0;
}

void ClassDemoApp::loadAllTextures() {
	text = LoadTexture("images/font1.png");
	background = Load24BitTexture("images/limbo.png");
	titleScreen = Load24BitTexture("images/menu.png");

	// Character Textures/Annimations
	// Vectors for textures
	// Idle Character Textures
	// GLuint idle[12];
		idle[0] = SpriteSheet(LoadTexture("images/character/idle/frame-1.png"), 1, 1, 1, 1, 1.5f);
		idle[1] = SpriteSheet(LoadTexture("images/character/idle/frame-2.png"), 1, 1, 1, 1, 1.5f);
		idle[2] = SpriteSheet(LoadTexture("images/character/idle/frame-3.png"), 1, 1, 1, 1, 1.5f);
		idle[3] = SpriteSheet(LoadTexture("images/character/idle/frame-4.png"), 1, 1, 1, 1, 1.5f);
		idle[4] = SpriteSheet(LoadTexture("images/character/idle/frame-5.png"), 1, 1, 1, 1, 1.5f);
		idle[5] = SpriteSheet(LoadTexture("images/character/idle/frame-6.png"), 1, 1, 1, 1, 1.5f);
		idle[6] = SpriteSheet(LoadTexture("images/character/idle/frame-7.png"), 1, 1, 1, 1, 1.5f);
		idle[7] = SpriteSheet(LoadTexture("images/character/idle/frame-8.png"), 1, 1, 1, 1, 1.5f);
		idle[8] = SpriteSheet(LoadTexture("images/character/idle/frame-9.png"), 1, 1, 1, 1, 1.5f);
		idle[9] = SpriteSheet(LoadTexture("images/character/idle/frame-10.png"), 1, 1, 1, 1, 1.5f);
		idle[10] = SpriteSheet(LoadTexture("images/character/idle/frame-11.png"), 1, 1, 1, 1, 1.5f);
		idle[11] = SpriteSheet(LoadTexture("images/character/idle/frame-12.png"), 1, 1, 1, 1, 1.5f);
	// jump Down Character Textures
	// GLuint jumpDown
		jumpDown = SpriteSheet(LoadTexture("images/character/jumpDown/pose.png"), 1, 1, 1, 1, 1.5f);
	// jump Up Character Textures
	// GLuint jumpUp
		jumpUp = SpriteSheet(LoadTexture("images/character/jumpUp/pose.png"), 1, 1, 1, 1, 1.5f);
	// movement Character Textures
	// GLuint movement[6]
		movement[0] = SpriteSheet(LoadTexture("images/character/movement/frame-1.png"), 1, 1, 1, 1, 1.5f);
		movement[1] = SpriteSheet(LoadTexture("images/character/movement/frame-2.png"), 1, 1, 1, 1, 1.5f);
		movement[2] = SpriteSheet(LoadTexture("images/character/movement/frame-3.png"), 1, 1, 1, 1, 1.5f);
		movement[3] = SpriteSheet(LoadTexture("images/character/movement/frame-4.png"), 1, 1, 1, 1, 1.5f);
		movement[4] = SpriteSheet(LoadTexture("images/character/movement/frame-5.png"), 1, 1, 1, 1, 1.5f);
		movement[5] = SpriteSheet(LoadTexture("images/character/movement/frame-6.png"), 1, 1, 1, 1, 1.5f);
	// saw Character Textures
	// GLuint saw[4]
		saw[0] = SpriteSheet(LoadTexture("images/character/saw/frame-1.png"), 1, 1, 1, 1, 1.5f);
		saw[1] = SpriteSheet(LoadTexture("images/character/saw/frame-2.png"), 1, 1, 1, 1, 1.5f);
		saw[2] = SpriteSheet(LoadTexture("images/character/saw/frame-3.png"), 1, 1, 1, 1, 1.5f);
		saw[3] = SpriteSheet(LoadTexture("images/character/saw/frame-4.png"), 1, 1, 1, 1, 1.5f);
	// sliding Texture
	// GLuint sliding
		sliding = SpriteSheet(LoadTexture("images/character/sliding/pose.png"), 1, 1, 1, 1, 1.5f);
	//GLuint blase;
		boom = SpriteSheet(LoadTexture("images/Shoot-blast-A.png"), 1, 1, 1, 1, 1.5f);
	
}

void ClassDemoApp::UpdateAndRender() {

	ticks = (float)SDL_GetTicks() / 1000000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	currentTime += elapsed;

	SDL_Event event;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if (gameState == STATE_MAIN_MENU) {
			if (keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_RETURN2]){
				gameState = STATE_GAME_RUNNING;
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

			}
		}
		if (gameState == STATE_GAME_RUNNING) {
			if (keys[SDL_SCANCODE_SPACE]) {
				characterState = RUNNING;
			}
			if (characterState == RUNNING) {

			}
			if (keys[SDL_SCANCODE_UP]) {
				characterState = JUMPING_UP;
			}
			if (keys[SDL_SCANCODE_DOWN]) {
				characterState = SLIDING;
			}
		}
	}
	Render();
}

void ClassDemoApp::Render() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (gameState)
	{
	case STATE_MAIN_MENU:
	{
		RenderMainMenu();
		break;
	}
	case STATE_GAME_RUNNING:
	{
		RenderGameRunning();
		break;
	}
	case STATE_GAME_OVER:
	{
		RenderGameOver();
		break;
	}
	}

}

void ClassDemoApp::RenderMainMenu(){
	// Make the Title Screen an entity like the background
	Entity mainMenu = Entity(SpriteSheet(titleScreen, 1, 1, 1, 1, 8.0f));
	mainMenu.Draw(program);

	modelMatrix.identity();
	modelMatrix.Translate(-3.0f, 3.0f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(textTexture, "Welcome to Enigma.", 0.4f, -0.20f);

	modelMatrix.identity();
	modelMatrix.Translate(-3.0f, 2.6f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(textTexture, "A Puzzle Based Platformer.", 0.4f, -0.20f);

	modelMatrix.identity();
	modelMatrix.Translate(-3.0f, 1.8f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(textTexture, "Controls:", 0.4f, -0.20f);

	modelMatrix.identity();
	modelMatrix.Translate(-3.0f,1.4f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(textTexture, "UP Arrow: Jump", 0.4f, -0.20f);

	modelMatrix.identity();
	modelMatrix.Translate(-3.0f, 0.2f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(textTexture, "DOWN Arrow: Crouch", 0.4f, -0.20f);

	modelMatrix.identity();
	modelMatrix.Translate(-3.0f, -0.5f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(textTexture, "Press enter to start.", 0.4f, -0.20f);

	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::RenderGameRunning(){
	modelMatrix.identity();
	backgroundEntity.Draw(program);

	int frames;

	if (characterState == IDLE) {
		frames = sizeof(idle) / sizeof(idle[0]);
		if (currentTime > .0002)
		{
			playerCurrentFrame++;
			currentTime = 0;
			if (playerCurrentFrame >= frames) { playerCurrentFrame = 0; }
		}
		player.sprite = idle[playerCurrentFrame];
		player.x = -2.0f; 
		player.y = -1.0f;
		player.Draw(program);
	}

	if (characterState == RUNNING) {
		frames = sizeof(movement) / sizeof(movement[0]);
		if (currentTime > .00025) {
			playerCurrentFrame++;
			currentTime = 0;
			if (playerCurrentFrame >= frames) { playerCurrentFrame = 0; };
		}
		player.sprite = movement[playerCurrentFrame];
		player.x = -2.0f;
		player.y = -1.0f;
		backgroundEntity.Draw(program);
		player.Draw(program);
		scrollBackground();
	}
	
	if (characterState == JUMPING_UP){
		player.sprite = jumpUp;
		player.velocity_y = 0.2f;
		player.y += elapsed * player.velocity_y * FIXED_TIMESTEP;
		modelMatrix.identity();
		modelMatrix.Translate(0.0, player.y, 0.0f);
		program->setModelMatrix(modelMatrix);
		player.Draw(program);
		if (player.y >= 2.0f) {
			player.sprite = jumpDown;
			player.velocity_y = -.2f;
			player.y -= elapsed * player.velocity_y;
			player.Draw(program);
		}
		scrollBackground();
	}
	if (characterState == SLIDING) {
		player.sprite = sliding;
		player.x = -2.0f;
		player.y = -2.0f;
		player.Draw(program);
		characterState = RUNNING;
	}

	// GENERATE RANDOM OBSTICLE
	vector<Entity> enemies;
	Entity sawBlade;
	sawBlade.sprite = saw[0];
	sawBlade.x = 4.0f;
	sawBlade.y = -1.0f;
	sawBlade.velocity_x = -0.25f;
	enemies.push_back(sawBlade);
	Entity fireBlast;
	fireBlast.sprite = boom;
	fireBlast.x = 4.0f;
	fireBlast.y = 0.0f;
	fireBlast.velocity_x = -0.25f;
	enemies.push_back(fireBlast);

	int count = 0;
	count += elapsed;
	if (count % 2 != 0){
		modelMatrix.identity();
		modelMatrix.Translate(enemies[1].y += enemies[1].velocity_x, 0.0f, 0.0f);
		program->setModelMatrix(modelMatrix);
		enemies[1].Draw(program);
	}
	else if (count % 2 == 0){
		modelMatrix.identity();
		modelMatrix.Translate(enemies[0].y += enemies[0].velocity_x, 0.0f, 0.0f);
		program->setModelMatrix(modelMatrix);
		enemies[0].Draw(program);
	}

	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::RenderGameOver(){

}

GLuint ClassDemoApp::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);
	return textureID;
}

GLuint ClassDemoApp::Load24BitTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	return textureID;
}

void ClassDemoApp::DrawText(int fontTexture, std::string text, float size, float spacing)
{
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++)
	{
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(),
		{
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		}
		);
		texCoordData.insert(texCoordData.end(),
		{
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		}
		);
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
};

void ClassDemoApp::scrollBackground() {
	backgroundEntity.x = backgroundOffset;
	backgroundOffset -= 0.025f;
}

float ClassDemoApp::lerp(float v0, float v1, float t) {
	return (1.0 - t) * v0 + t * v1;
}