#version 460 core

layout (location = 0) in vec3 a_pos;

out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_pos, 1.0f);
    frag_pos = vec3(model * vec4(a_pos, 1.0f));
}