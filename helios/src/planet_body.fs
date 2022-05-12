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
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);

    vec3 ambient = light.ambient * material.ambient;

    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = light.diffuse * (diff *  material.diffuse);

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 lum = ambient + diffuse + specular;

    frag_color = vec4(lum, 1.0f) * color * vec4(light.color, 1.0f) * vec4(material.color, 1.0f) * texture(body_texture, tex_coord);
}