#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
// this would take location 3, 4, 5 & 6 because a mat4 is 4 vec4s
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0f);
    TexCoords = aTexCoords;
};