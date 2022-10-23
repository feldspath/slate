#version 460 core

#define M_PI 3.141592

out vec4 FragColor;

layout (std140) uniform Matrices
{
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 view_matrix_inv;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float power;
};

uniform PointLight lights[16];
uniform int light_count = 0;

uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 specular;

int specular_exp = 64;

// world space frag info
in vec3 fragPos;
in vec3 fragNormal;

void main() {
    vec3 camera_pos = vec3(view_matrix_inv * vec4(0.0f, 0.0f, 0.0f, 1.0f));
    vec3 view_dir = normalize(camera_pos - fragPos);

    // ambient (not physically accurate)
    vec3 col = ambient * 0.1f; 

    for (int i = 0; i < 1; ++i) {
        // vectors
        vec3 diff = lights[i].position - fragPos;
        vec3 light_dir = normalize(diff);
        vec3 wh = normalize(view_dir+light_dir);
        float sqdist = dot(diff, diff);

        vec3 radiance = lights[i].color * lights[i].power / (4.0f * sqdist * M_PI);
        
        // diffuse
        vec3 diffuse_col = diffuse * radiance * max(dot(fragNormal, light_dir), 0.0f);

        // specular
        vec3 specular_col = radiance * specular * (specular_exp + 2) / (specular_exp * M_PI) * pow(max(dot(wh, fragNormal), 0.0f), specular_exp);
        
        col += diffuse_col + specular_col;
    }

    FragColor = vec4(col, 1.0f);
}