#version 460 core
out vec4 out_colour;

in vec4 color;
in vec2 uv;

void main(void) {
	out_colour = vec4(uv, 0.0f, 1.0f);
}