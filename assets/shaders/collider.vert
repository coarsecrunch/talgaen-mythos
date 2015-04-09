#version 330 core

layout(location = 0) in vec3 vPosition;

uniform float quadColor[4];
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 oColor;

void main()
{
	vec4 pos = (projection * view * model) * vec4(vPosition.xy, 1.0, 1.0);

	oColor = vec4(quadColor[0], quadColor[1], quadColor[2], quadColor[3]);

	gl_Position = pos;
}