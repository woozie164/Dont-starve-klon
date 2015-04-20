#version 440

uniform mat4 projection;
uniform mat4 view;

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec2 in_tex_coord;

out vec4 position;
out vec2 vs_tex_coord;

void
main()
{
	vec4 position = vec4(vPosition, 1.0);
	gl_Position = projection * view * position;
	vs_tex_coord = in_tex_coord;
}
