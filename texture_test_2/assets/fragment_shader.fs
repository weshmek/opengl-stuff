#version 330

in vec2 frag_texture;
in vec4 frag_colour; 

out vec4 out_colour;

uniform sampler2D tex;

void main(void) {
	out_colour = texture(tex, frag_texture) + frag_colour;


}
