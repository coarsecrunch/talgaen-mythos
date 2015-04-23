#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vUv;
layout(location = 3) in float vTid;


out DATA
{
	vec4 position;
	vec4 color;
	vec2 uv;
	float tid;
} vs_out;

void main()
{	
	gl_Position = vec4(vPosition.xy, 1.0, 1.0);

	vs_out.color = vColor;
	vs_out.uv = vUv;
	vs_out.tid = vTid;
}