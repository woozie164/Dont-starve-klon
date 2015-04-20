#version 440
layout (binding = 0) uniform sampler2D tex;

in vec4 position;
in vec2 vs_tex_coord;

out vec4 color;

void
main()
{
	color = texture(tex, vs_tex_coord);
}
