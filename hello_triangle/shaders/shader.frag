#version 450

// input
layout (location = 0) in fragColor;

// output
layout (location = 0) out outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}