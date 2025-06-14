#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    // applying transformation of the model, using the normal matrix
    // this operation is expensive and we shouldn't be doing for each vertex
    // this operation is needed if you are applying a non-uniform scaling
    // if not you can just multiply the normal by the model
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal =  normalMatrix * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
};