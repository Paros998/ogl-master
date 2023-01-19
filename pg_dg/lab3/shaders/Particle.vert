#version 460 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 rgba;

out vec4 fragmentColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    gl_Position = projection * view * model * vec4(pos,1.0);
    fragmentColor = rgba;
}