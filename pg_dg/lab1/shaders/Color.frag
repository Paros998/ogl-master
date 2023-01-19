#version 330 core

// Ouput data
out vec4 FragColor;

// input
in vec4 fragmentColor;

void main()
{
	FragColor = fragmentColor;
}