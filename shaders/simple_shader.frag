#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragcolor;

void main()
{
	outColor = vec4(fragcolor, 1.0);
}