#ifndef ENTITY_H
#define ENTITY_H

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <string>
#include "SpriteSheet.h"
#include "ShaderProgram.h"
#include "Matrix.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

enum EntityType { ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_EVENT };

class Entity {
public:
	Entity();
	Entity(SpriteSheet sprite);

	bool collidesWith(Entity* entity);
	void Draw(ShaderProgram* program);

	float x;
	float y;

	float initial_x;
	float initial_y;

	float width;
	float height;

	float direction_x;
	float direction_y;

	float velocity_x;
	float velocity_y;

	float acceleration_x;
	float acceleration_y;

	float friction_x;
	float friction_y;
	
	float gravity_y;
	float gravity_x;
	
	bool isStatic;

	float rotation;

	bool enableCollisions;
	bool isVisible;

	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
	bool collidedTop;

	SpriteSheet sprite;
	EntityType entityType;

	int numFrames;
	int fixedFrames;
	int currentIndex;
	int idx;
	int spriteNum;
	float animationElapsed;
	float framesPerSecond;

};

#endif

