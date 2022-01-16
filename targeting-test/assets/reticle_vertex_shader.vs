#version 460

in vec3 vrt_pos;
in vec4 vrt_col;

out vec4 frag_col;


void main(void) {

	gl_Position = vec4(vrt_pos, 2 *vrt_pos.z);
	frag_col = vrt_col;

}
