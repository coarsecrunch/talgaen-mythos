#version 330 core

uniform sampler2D tex0;

out vec4 fColor;
in vec2 UV;

void main()
{
	fColor = texture2D(tex0, UV);
}