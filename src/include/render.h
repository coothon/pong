#ifndef PONG_RENDER_H_
#define PONG_RENDER_H_

#define _DEFAULT_SOURCE 1
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "common.h"
#include "file.h"

typedef struct render_object {
	GLuint  gl_data[DATA_ARRAY_SIZE];
	GLuint  uniform_id[ID_ARRAY_SIZE];
	GLfloat uniform_data[UNIFORM_ARRAY_SIZE];
	vertex *vertices;
	GLint   vertices_num;
	GLuint *indices;
	GLint   indices_num;
} render_object;

typedef struct game_object {
	render_object render;
	GLfloat       position[2];
	GLfloat       scale[2];
} game_object;

typedef struct game {
	GLint       window_w, window_h;
	GLFWwindow *win;
	GLdouble    cursor_x, cursor_y;
	GLint       user_input_up;
	GLint       user_input_down;
	GLfloat     paddle1;
	GLfloat     paddle2;
	double      delta_time;
	double      time;
} game;

bool init_game(game *g);
void stop_game(game *g);
bool init_game_object(game_object *go, const char *vertex_shader_path,
                      const char *fragment_shader_path, vertex *vertices,
                      GLint vertices_num, GLuint *indices, GLint indices_num);
void draw_game_object(game_object *go);

// GLFW callbacks.
void glfw_error_cb(GLint e, const GLchar *desc);
void glfw_key_cb(GLFWwindow *w, GLint key, GLint scancode, GLint action,
                 GLint mods);
void glfw_framebuffer_size_cb(GLFWwindow *w, GLint fbwidth, GLint fbheight);
void glfw_cursorpos_cb(GLFWwindow *w, GLdouble x, GLdouble y);
void glfw_click_cb(GLFWwindow *w, int button, int action, int mods);
void glfw_click_cb(GLFWwindow *w, int button, int action, int mods);
void glfw_scroll_cb(GLFWwindow *w, double x, double y);


#endif // PONG_RENDER_H_
