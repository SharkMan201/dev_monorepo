#version 450


// hard coded positions and colors
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec3 inColor;

// pass colors down the pipeline
layout (location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}