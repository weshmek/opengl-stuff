#version 460

in vec4 frag_colour;
in flat int target_colour;
out vec4 out_colour;
out int out_id;

void main(void) {
	out_colour = frag_colour;
	out_id = target_colour;
}
