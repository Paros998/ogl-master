#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D s_texture;

void main()
{
    FragColor = texture(s_texture,TexCoord);
}