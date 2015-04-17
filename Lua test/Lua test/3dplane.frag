#version 440
uniform sampler2D tex;

//in vec4 frag_position;
//in vec2 frag_tex_coord;
//in vec3 frag_normal;

out vec4 color;

void
main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
	//color = texture(tex, frag_tex_coord);
}
