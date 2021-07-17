#version 330

in vec4 frag_colour;
out vec4 out_colour;

void main(void) {
	out_colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	out_colour = frag_colour;
}
