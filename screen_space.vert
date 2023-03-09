#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec2 aUV;

uniform vec2 viewport;
uniform vec2 object_position;
uniform vec2 scale;

out vec4 color;
out vec2 uv;

void main(void) {
	color = aCol;
	uv = aUV;
	gl_Position = vec4(((aPos / viewport) * scale) + (object_position / viewport), 0.0, 1.0);
}
