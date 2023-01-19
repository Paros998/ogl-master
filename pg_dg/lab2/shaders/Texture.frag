#version 330 core

// Ouput data
out vec4 FragColor;

// input
in vec2 TexCoord;

uniform sampler2D Texture;
void main()
{
	FragColor = texture(Texture, TexCoord);
}