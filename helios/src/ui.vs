#version 460 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec4 a_color;

out vec4 color;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(a_pos, 0.0f, 1.0f);
    color = a_color;
}