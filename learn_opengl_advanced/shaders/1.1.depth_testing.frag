#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    // reflections
//    vec3 I = normalize(Position - cameraPos);
//    vec3 R = reflect(I, normalize(Normal));

    // refractions
    float refraction_ratio = 1.0 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), refraction_ratio);

    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}