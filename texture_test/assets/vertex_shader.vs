#version 330

in vec3 position;
in vec4 in_colour;
//in vec2 texcoord;

out vec4 frag_colour;
//out vec2 frag_texture;

void main(void) {
	gl_Position = vec4(position, 1.0);
	frag_colour = in_colour;
	//frag_texture = texcoord;
}
