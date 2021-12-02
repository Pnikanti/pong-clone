#version 330 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;

out vec2 vertexPosition;
out vec2 textureCoordinate;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
	gl_Position = viewProjection * model * vec4(aPos, 1.0f);
	vertexPosition = gl_Position.xy;
	textureCoordinate = aTex;
}