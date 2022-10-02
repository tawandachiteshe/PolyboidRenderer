#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

out vec4 vColor;
out vec2 vUV;

uniform mat4 uViewProj;
uniform mat4 uTransform;

void main() {

    vColor = aColor;
    vUV = aUV;
    
    gl_Position =    uViewProj * uTransform * vec4(aPosition, 1.0f) ;
    
}