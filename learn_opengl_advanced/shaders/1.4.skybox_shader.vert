#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    // tex coords (aka direction in the cube map) is the same as the the
    // position because the cube is centered at 0
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    // manipulate the depth value to be the maximum possible (to trick openGL
    // that the skybox is the furthest object in the scene) to benefrit from
    // early depth testing
    gl_Position = pos.xyww;
}