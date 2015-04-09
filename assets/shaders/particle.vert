#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 UVCoor;

uniform mat4 MVP;
out vec2 UV;


void main()
{
	vec4 poop = MVP * vec4(vPosition.xy, 1.0, 1.0);
	UV = UVCoor;
	gl_Position = poop;
}