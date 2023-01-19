#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 rgba;

out vec4 fragmentColor;

void main(){
    gl_Position = vec4(pos,1.0);
    fragmentColor = rgba;
}