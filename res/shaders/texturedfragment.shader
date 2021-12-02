#version 330 core

out vec4 fragColor;

in vec2 textureCoordinate;

uniform vec3 color;
uniform sampler2D textureData;

void main()
{
	fragColor = texture(textureData, textureCoordinate) * vec4(color, 1.0f);
}