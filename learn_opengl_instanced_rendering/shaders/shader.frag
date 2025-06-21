#version 330 core
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

out vec4 FragColor;

void main()
{
    float gamma = 2.2;

    FragColor.rgb = pow(texture(texture_diffuse1, TexCoords).rgb, vec3(1 / gamma));
}