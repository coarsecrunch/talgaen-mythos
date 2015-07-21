#version 330 core
#define MAX_ACTIVE_TEXTURE_UNITS 32

uniform sampler2D textures[MAX_ACTIVE_TEXTURE_UNITS];

in DATA
{
	vec4 position;
	vec4 color;
	vec2 uv;
	float tid;
	float transparencyScale;
} fs_in;

out vec4 oColor;

void main()
{	
	vec4 tempColor = fs_in.color;
	
	if (fs_in.tid >= -0.01f)
	{
		tempColor = fs_in.color * texture2D(textures[ int(fs_in.tid) ], fs_in.uv);
	}

	tempColor[3] *= fs_in.transparencyScale;
	oColor = tempColor;
}