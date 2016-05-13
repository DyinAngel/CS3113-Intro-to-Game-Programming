#include "Entity.h"

Entity::Entity() {
	friction_x = 0.0f;
	friction_y = 0.0f;
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0f;
	gravity_y = -4.0f;
	enableCollisions = true;
	collidedBottom = false;
	collidedRight = false;
	collidedLeft = false;
	collidedTop = false;
	isStatic = true;

	animationElapsed = 0.0f;

	isVisible = false;
}

Entity::Entity(SpriteSheet sprite) : friction_x(0), friction_y(0), velocity_x(0), velocity_y(0), acceleration_x(0), acceleration_y(0), gravity_y(0), sprite(sprite), x(0), y(0), width(0), height(0){}

float lerp(float v0, float v1, float t) {
	return (1.0 - t) * v0 + t * v1;
}

bool Entity::collidesWith(Entity* other) {
	return !(
		((y + (height / 2)) <= (other->y - (other->height / 2))) ||
		((y - (height / 2)) >= (other->y + (other->height / 2))) ||
		((x + (width / 2)) <= (other->x - (other->width / 2))) ||
		((x - (width / 2)) >= (other->x + (other->width / 2)))
	);
}

void Entity::Draw(ShaderProgram *program) {
	Matrix modelMatrix = Matrix();
	modelMatrix.Translate(x, y, 0);
	program->setModelMatrix(modelMatrix);

	float vertices[] = {
		-width / 2, -height / 2,
		width / 2, height / 2,
		-width / 2, height / 2,
		width / 2, height / 2,
		-width / 2, -height / 2,
		width / 2, -height / 2
	};

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	sprite.Draw(program);

	glDisableVertexAttribArray(program->positionAttribute);
}