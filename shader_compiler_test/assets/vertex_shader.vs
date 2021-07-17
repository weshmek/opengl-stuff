#version 330

in vec3 position;
in vec4 in_colour;

out vec4 frag_colour;

uniform vec3 offsets[2];

void main(void) {
	gl_Position = vec4(position, 1.0) + vec4(offsets[gl_InstanceID], 0.0);
	frag_colour = in_colour;
}
