#ifndef PONG_GAME_H_
#define PONG_GAME_H_

#define _DEFAULT_SOURCE 1
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "common.h"
#include "render.h"

void translate_game_object(game_object *go, GLfloat x, GLfloat y);
void move_game_object(game_object *go, GLfloat x, GLfloat y);
void scale_game_object(game_object *go, GLfloat x, GLfloat y);
void move_paddles(void);

#endif // PONG_GAME_H_
