#version 330 core
#define MAX_ACTIVE_TEXTURE_UNITS 32

uniform sampler2D textures[MAX_ACTIVE_TEXTURE_UNITS];

in DATA
{
	vec4 position;
	vec4 color;
	vec2 uv;
	float tid;
} fs_in;

out vec4 oColor;

void main()
{	
	
	if (fs_in.tid >= 0)
	{
		oColor = texture2D(textures[ int(fs_in.tid) ], fs_in.uv);
	}
	else
	{
		oColor = fs_in.color;
	}
}