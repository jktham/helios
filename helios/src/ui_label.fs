#version 460 core

in vec4 color;
in vec2 texcoord;

uniform sampler2D font_texture;

out vec4 frag_color;

void main()
{
    frag_color = color * texture(font_texture, texcoord);
}