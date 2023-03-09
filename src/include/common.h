#ifndef PONG_COMMON_H_
#define PONG_COMMON_H_

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GAME_WIDTH 1280
#define GAME_HEIGHT 720
#define GAME_ASPECT_RATIO ((GLfloat)(GAME_WIDTH) / (GLfloat)(GAME_HEIGHT))
#define PADDLE_SPEED 1.75
#define PADDLE_SIZE 200
#define PADDLE_WIDTH 50
#define PADDLE1_POS_X -(GAME_WIDTH - PADDLE_WIDTH)
#define PADDLE1_POS_Y (GAME_HEIGHT - PADDLE_SIZE)
#define PADDLE2_POS_X (GAME_WIDTH - PADDLE_WIDTH)
#define PADDLE2_POS_Y (GAME_HEIGHT - PADDLE_SIZE)

#define GLFW_TO_SCREEN_X(pos) \
(GLfloat)((pos)) - ((GLfloat)(pong.window_w) / 2.0f)
#define GLFW_TO_SCREEN_Y(pos) \
-((GLfloat)((pos)) - ((GLfloat)(pong.window_h) / 2.0f))

// For accessing ID array of render object.
enum gl_data_array {
	DATA_VAO = 0,
	DATA_VBO,
	DATA_EBO,
	DATA_VERTEX_SHADER,
	DATA_FRAGMENT_SHADER,
	DATA_SHADER_PROGRAM,
	DATA_ARRAY_SIZE,
};

// For accessing ID array of uniforms.
enum gl_uniform_id_array {
	ID_TIME = 0, // float
	ID_VIEWPORT, // vec2
	ID_SCALE,    // vec2
	ID_POSITION, // vec2
	ID_ARRAY_SIZE,
};

// For accessing value array of uniforms. Float only.
enum gl_uniform_value_array {
	UNIFORM_TIME = 0,
	UNIFORM_VIEWPORT_X,
	UNIFORM_VIEWPORT_Y,
	UNIFORM_ARRAY_SIZE,
};

typedef struct vertex {
	GLfloat position[2];
	GLfloat colour[4];
	GLfloat uv[2];
} vertex;

#define NUM_TRIANGLE_VERTICES 3
static const vertex triangle_vertices[NUM_TRIANGLE_VERTICES] = {
	{
		{
			-0.5f,
			-0.5f,
		},
		{
			1.0f,
			0.0f,
			0.0f,
			1.0f,
		},
		{
			0.0f,
			1.0f,
		},
	},
	{
		{
			0.5f,
			-0.5f,
		},
		{
			0.0f,
			1.0f,
			0.0f,
			1.0f,
		},
		{
			1.0f,
			1.0f,
		},
	},
	{
		{
			0.0f,
			0.5f,
		},
		{
			0.0f,
			0.0f,
			1.0f,
			1.0f,
		},
		{
			0.5f,
			0.0f,
		},
	},
};

#define NUM_TRIANGLE_INDICES 3
static const GLuint triangle_indices[NUM_TRIANGLE_INDICES] = { 0, 1, 2 };

#define NUM_RECT_VERTICES 4
static const vertex rect_vertices[NUM_RECT_VERTICES] = {
	{
		{
			0.5f,
			0.5f,
		},
		{
			1.0f,
			1.0f,
			1.0f,
			1.0f,
		},
		{
			1.0f,
			1.0f,
		},
	},
	{
		{
			0.5f,
			-0.5f,
		},
		{
			1.0f,
			1.0f,
			1.0f,
			1.0f,
		},
		{
			1.0f,
			0.0f,
		},
	},
	{
		{
			-0.5f,
			-0.5f,
		},
		{
			1.0f,
			1.0f,
			1.0f,
			1.0f,
		},
		{
			0.0f,
			0.0f,
		},
	},
	{
		{
			-0.5f,
			0.5f,
		},
		{
			1.0f,
			1.0f,
			1.0f,
			1.0f,
		},
		{
			0.0f,
			1.0f,
		},
	},
};

#define NUM_RECT_INDICES 6
static const GLuint rect_indices[NUM_RECT_INDICES] = { 0, 3, 2, 0, 1, 2 };

#endif // PONG_COMMON_H_
