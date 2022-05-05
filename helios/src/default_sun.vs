#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 3) in vec4 a_color;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_coord;
out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_pos, 1.0f);
    frag_pos = vec3(model * vec4(a_pos, 1.0f));
    normal = vec3(model * vec4(a_normal, 0.0f));
    tex_coord = a_tex_coord;
    color = a_color;
}