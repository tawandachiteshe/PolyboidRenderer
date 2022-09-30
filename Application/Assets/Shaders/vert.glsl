﻿#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

out vec4 vColor;
out vec2 vUV;

void main() {

    vColor = aColor;
    vUV = aUV;
    
    gl_Position = vec4(aPosition, 1.0f);
    
}