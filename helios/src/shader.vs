#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_color;

out vec4 color;

void main()
{
    gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
    color = a_color;
}