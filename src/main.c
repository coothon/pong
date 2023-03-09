#define _DEFAULT_SOURCE 1
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "include/common.h"
#include "include/game.h"
#include "include/render.h"


game pong;

int main(void) {
	if (!init_game(&pong)) exit(EXIT_FAILURE);

	game_object triangle = { 0 };
	if (!init_game_object(&triangle, "../screen_space.vert", "../uv.frag",
	                      triangle_vertices, NUM_TRIANGLE_VERTICES,
	                      triangle_indices, NUM_TRIANGLE_INDICES)) {
		stop_game(&pong);
		exit(EXIT_FAILURE);
	}
	scale_game_object(&triangle, 100.0f, 100.0f);

	game_object paddle = { 0 };
	if (!init_game_object(&paddle, "../screen_space.vert", "../uv.frag",
	                      rect_vertices, NUM_RECT_VERTICES, rect_indices,
	                      NUM_RECT_INDICES)) {
		stop_game(&pong);
		exit(EXIT_FAILURE);
	}
	scale_game_object(&paddle, (GLfloat)PADDLE_WIDTH, (GLfloat)PADDLE_SIZE);

	double temptime = 0.0;
	while (!glfwWindowShouldClose(pong.win)) {
		temptime        = pong.time;
		pong.time       = glfwGetTime();
		pong.delta_time = pong.time - temptime;

		glClear(GL_COLOR_BUFFER_BIT);

		move_paddles();

		move_game_object(&paddle, PADDLE1_POS_X, pong.paddle1 * PADDLE1_POS_Y);
		draw_game_object(&paddle);
		draw_game_object(&triangle);

		glfwSwapBuffers(pong.win);
		glfwPollEvents();
	}

	stop_game(&pong);
	return EXIT_SUCCESS;
}
