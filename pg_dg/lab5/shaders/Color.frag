#version 330 core

// Ouput data
out vec4 FragColor;

// input
in vec4 fragmentColor;

void main()
{
	float blackColor = (fragmentColor.r + fragmentColor.g + fragmentColor.b) / 3;
	FragColor = vec4(blackColor, blackColor, blackColor, fragmentColor.a);
}