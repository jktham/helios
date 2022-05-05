#version 460 core

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coord;
in vec4 color;

uniform sampler2D texture1;

out vec4 frag_color;

void main()
{
    frag_color = color * texture(texture1, tex_coord);
}