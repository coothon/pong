#include "include/render.h"

extern game pong;

bool init_game(game *g) {
	if (!g) return false;

	if (!glfwInit()) {
		fprintf(stderr, "[ERROR] GLFW initialization failed.\n");
		return false;
	}

	glfwSetErrorCallback(glfw_error_cb);

	glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	g->win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Pong", NULL,
	                          NULL);
	if (!g->win) {
		fprintf(stderr, "[ERROR] GLFW window creation failed.\n");
		return false;
	}

	glfwMakeContextCurrent(g->win);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwGetFramebufferSize(g->win, &g->window_w, &g->window_h);
	glViewport(0, 0, g->window_w, g->window_h);

	glfwSetFramebufferSizeCallback(g->win, glfw_framebuffer_size_cb);
	glfwSetKeyCallback(g->win, glfw_key_cb);

	glfwSwapInterval(0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	return true;
}

void stop_game(game *g) {
	glfwDestroyWindow(g->win);
	glfwTerminate();
}

bool init_game_object(game_object *go, const char *vertex_shader_path,
                      const char *fragment_shader_path, vertex *vertices,
                      GLint vertices_num, GLuint *indices, GLint indices_num) {
	const char *shaders[2] = { 0 };
	shaders[0]             = read_entire_file(vertex_shader_path);
	if (!shaders[0]) {
		fprintf(stderr, "[ERROR] Vertex shader not found.");
		return false;
	}

	shaders[1] = read_entire_file(fragment_shader_path);
	if (!shaders[1]) {
		free((void *)shaders[0]);
		fprintf(stderr, "[ERROR] Fragment shader not found.");
		return false;
	}

	go->render.vertices     = vertices;
	go->render.vertices_num = vertices_num;
	go->render.indices      = indices;
	go->render.indices_num  = indices_num;

	glGenVertexArrays(1, &go->render.gl_data[DATA_VAO]);
	glGenBuffers(1, &go->render.gl_data[DATA_VBO]);
	glGenBuffers(1, &go->render.gl_data[DATA_EBO]);

	glBindVertexArray(go->render.gl_data[DATA_VAO]);
	glBindBuffer(GL_ARRAY_BUFFER, go->render.gl_data[DATA_VBO]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * go->render.vertices_num,
	             go->render.vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->render.gl_data[DATA_EBO]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             sizeof(GLuint) * go->render.indices_num, go->render.indices,
	             GL_STATIC_DRAW);

	go->render.gl_data[DATA_VERTEX_SHADER] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(go->render.gl_data[DATA_VERTEX_SHADER], 1, &shaders[0],
	               NULL);
	glCompileShader(go->render.gl_data[DATA_VERTEX_SHADER]);
	/* Check. */ {
		GLint  shader_status;
		GLchar shader_log[512];
		glGetShaderiv(go->render.gl_data[DATA_VERTEX_SHADER], GL_COMPILE_STATUS,
		              &shader_status);
		if (!shader_status) {
			glGetShaderInfoLog(go->render.gl_data[DATA_VERTEX_SHADER], 512,
			                   NULL, shader_log);
			fprintf(stderr, "[ERROR] %s", shader_log);
			free((void *)shaders[0]);
			free((void *)shaders[1]);
			return false;
		}
	}

	go->render.gl_data[DATA_FRAGMENT_SHADER] =
		glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(go->render.gl_data[DATA_FRAGMENT_SHADER], 1, &shaders[1],
	               NULL);
	glCompileShader(go->render.gl_data[DATA_FRAGMENT_SHADER]);
	/* Check. */ {
		GLint  shader_status;
		GLchar shader_log[512];
		glGetShaderiv(go->render.gl_data[DATA_FRAGMENT_SHADER],
		              GL_COMPILE_STATUS, &shader_status);
		if (!shader_status) {
			glGetShaderInfoLog(go->render.gl_data[DATA_FRAGMENT_SHADER], 512,
			                   NULL, shader_log);
			fprintf(stderr, "[ERROR] %s", shader_log);
			free((void *)shaders[0]);
			free((void *)shaders[1]);
			return false;
		}
	}

	go->render.gl_data[DATA_SHADER_PROGRAM] = glCreateProgram();
	glAttachShader(go->render.gl_data[DATA_SHADER_PROGRAM],
	               go->render.gl_data[DATA_VERTEX_SHADER]);
	glAttachShader(go->render.gl_data[DATA_SHADER_PROGRAM],
	               go->render.gl_data[DATA_FRAGMENT_SHADER]);
	glLinkProgram(go->render.gl_data[DATA_SHADER_PROGRAM]);
	/* Check. */ {
		GLint  program_status;
		GLchar program_log[512];
		glGetProgramiv(go->render.gl_data[DATA_SHADER_PROGRAM], GL_LINK_STATUS,
		               &program_status);
		if (!program_status) {
			glGetProgramInfoLog(go->render.gl_data[DATA_SHADER_PROGRAM], 512,
			                    NULL, program_log);
			fprintf(stderr, "[ERROR] %s", program_log);
			free((void *)shaders[0]);
			free((void *)shaders[1]);
			return false;
		}
	}

	glDeleteShader(go->render.gl_data[DATA_VERTEX_SHADER]);
	glDeleteShader(go->render.gl_data[DATA_FRAGMENT_SHADER]);
	free((void *)shaders[0]);
	free((void *)shaders[1]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex),
	                      (void *)offsetof(vertex, colour));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
	                      (void *)offsetof(vertex, uv));

	go->render.uniform_id[ID_TIME] =
		glGetUniformLocation(go->render.gl_data[DATA_SHADER_PROGRAM], "time");
	go->render.uniform_id[ID_VIEWPORT] = glGetUniformLocation(
		go->render.gl_data[DATA_SHADER_PROGRAM], "viewport");
	go->render.uniform_id[ID_SCALE] =
		glGetUniformLocation(go->render.gl_data[DATA_SHADER_PROGRAM], "scale");
	go->render.uniform_id[ID_POSITION] = glGetUniformLocation(
		go->render.gl_data[DATA_SHADER_PROGRAM], "object_position");

	return true;
}

void draw_game_object(game_object *go) {
	glBindVertexArray(go->render.gl_data[DATA_VAO]);
	glUseProgram(go->render.gl_data[DATA_SHADER_PROGRAM]);

	glUniform1f(go->render.uniform_id[ID_TIME], (GLfloat)glfwGetTime());
	glUniform2f(go->render.uniform_id[ID_VIEWPORT], (GLfloat)GAME_WIDTH,
	            (GLfloat)GAME_HEIGHT);
	glUniform2f(go->render.uniform_id[ID_POSITION], go->position[0],
	            go->position[1]);
	glUniform2f(go->render.uniform_id[ID_SCALE], go->scale[0], go->scale[1]);

	glDrawElements(GL_TRIANGLES, go->render.indices_num, GL_UNSIGNED_INT, 0);
}

void glfw_error_cb(GLint e, const GLchar *desc) { fprintf(stderr, desc); }

void glfw_key_cb(GLFWwindow *w, GLint key, GLint scancode, GLint action,
                 GLint mods) {
	(void)scancode;
	(void)mods;

	switch (action) {
	case GLFW_PRESS: {
		switch (key) {
		case GLFW_KEY_ESCAPE: {
			glfwSetWindowShouldClose(w, GLFW_TRUE);
		} break;

		case GLFW_KEY_UP: {
			pong.user_input_up = 1;
		} break;

		case GLFW_KEY_DOWN: {
			pong.user_input_down = 1;
		} break;

		default:
			return;
		}
	} break;

	case GLFW_RELEASE: {
		switch (key) {
		case GLFW_KEY_UP: {
			pong.user_input_up = 0;
		} break;

		case GLFW_KEY_DOWN: {
			pong.user_input_down = 0;
		} break;

		default:
			return;
		}
	} break;

	default:
		return;
	}
}

void glfw_framebuffer_size_cb(GLFWwindow *w, GLint fbwidth, GLint fbheight) {
	// Calculate the scaling factor based on the minimum dimension and the game
	// size, to ensure the entire game is visible at all times.
	GLfloat aspect_ratio   = (GLfloat)(fbwidth) / (GLfloat)(fbheight);
	GLfloat scaling_factor = 0.0f;
	if (aspect_ratio > GAME_ASPECT_RATIO) {
		// The window is wider than the game, so scale based on the game height
		scaling_factor = (GLfloat)(GAME_HEIGHT) / (GLfloat)(fbheight);
	} else {
		// The window is taller than the game, so scale based on the game width
		scaling_factor = (GLfloat)(GAME_WIDTH) / (GLfloat)(fbwidth);
	}

	// Calculate the scaled size of the game based on the scaling factor
	GLfloat scaled_width  = (GLfloat)(GAME_WIDTH) / scaling_factor;
	GLfloat scaled_height = (GLfloat)(GAME_HEIGHT) / scaling_factor;

	GLint sw       = (GLint)(scaled_width);
	GLint sh       = (GLint)(scaled_height);
	pong.window_w  = sw;
	pong.window_h  = sh;
	GLint x_offset = (fbwidth - sw) / 2;
	GLint y_offset = (fbheight - sh) / 2;

	glViewport(x_offset, y_offset, sw, sh);
}

void glfw_key_cb(GLFWwindow *w, GLint key, GLint scancode, GLint action,
                 GLint mods);
void glfw_framebuffer_size_cb(GLFWwindow *w, GLint fbwidth, GLint fbheight);
void glfw_cursorpos_cb(GLFWwindow *w, GLdouble x, GLdouble y);
void glfw_click_cb(GLFWwindow *w, int button, int action, int mods);
void glfw_click_cb(GLFWwindow *w, int button, int action, int mods);
void glfw_scroll_cb(GLFWwindow *w, double x, double y);