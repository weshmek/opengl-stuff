#version 460

uniform mat4 R[2];
in vec3 position;
in vec4 in_colour;

out vec4 frag_colour;

void main(void) {
	gl_Position = R[0] * R[1] * vec4(position.xyz, 10.0);
	frag_colour = in_colour;

}
