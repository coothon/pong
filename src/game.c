#include "include/game.h"

extern game pong;

void move_paddles(void) {
	GLfloat move = 0.0f;
	if (pong.user_input_up)
		if (pong.user_input_down) move = 0.0f;
		else move = 1.0f;
	else if (pong.user_input_down) move = -1.0f;


	pong.paddle1 += move * (GLfloat)(PADDLE_SPEED * pong.delta_time);

	if (pong.paddle1 > 1.0f) pong.paddle1 = 1.0f;
	if (pong.paddle1 < -1.0f) pong.paddle1 = -1.0f;
}

void translate_game_object(game_object *go, GLfloat x, GLfloat y) {
	go->position[0] += x;
	go->position[1] += y;
}

void move_game_object(game_object *go, GLfloat x, GLfloat y) {
	go->position[0] = x;
	go->position[1] = y;
}

void scale_game_object(game_object *go, GLfloat x, GLfloat y) {
	go->scale[0] = x;
	go->scale[1] = y;
}
