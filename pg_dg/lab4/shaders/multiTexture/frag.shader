#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_f;
uniform sampler2D texture_s;

void main()
{
    FragColor = mix(texture(texture_f,TexCoord),texture(texture_s,TexCoord),0.35);
}