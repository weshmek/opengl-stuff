#version 460

uniform vec3 camera_pos;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 R[2];
uniform mat4 S;
uniform int timer;

in vec3 position;
in vec4 in_colour;
in int random_offset;
in int target_id;

out vec4 frag_colour;
out int target_colour;

uniform float z_offsets[4];

void main(void) {
	const float pi = acos(0.0);
	const int animation_timer = (random_offset + timer);
	const float theta = (2 * pi * float(animation_timer)) / 50;
	mat4 perspective_matrix = mat4( 1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 1.0,
					0.0, 0.0, -1.0, 0.0);

	const mat4 anim_rot_matrix = mat4(    cos(theta), 0.0, -sin(theta), 0.0,
					0.0, 1.0, 0.0, 0.0,
					sin(theta), 0.0, cos(theta), 0.0,
					0.0, 0.0, 0.0, 1.0);
	vec4 r_position = anim_rot_matrix * vec4(position, 1.0);
	gl_Position = S * perspective_matrix * R[0] * R[1] * P * vec4(r_position.xy, r_position.z + ( 50 * ( gl_BaseInstance + gl_InstanceID)), 1.0) ;
	frag_colour = in_colour;
	target_colour = target_id;
}
