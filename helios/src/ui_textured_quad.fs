#version 460 core

in vec4 color;
in vec2 texcoord;

uniform sampler2D quad_texture;

out vec4 frag_color;

void main()
{
    frag_color = color * texture(quad_texture, texcoord);
}