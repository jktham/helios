#version 460 core

struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coord;
in vec4 color;

uniform sampler2D body_texture;
uniform vec3 view_pos;

uniform Material material;
uniform Light light;

out vec4 frag_color;

void main()
{
    frag_color = color * vec4(material.color, 1.0f) * texture(body_texture, tex_coord);
}