#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 vNormal;
out vec2 vUV;
out vec3 vFragPos;

uniform mat4 uViewProj;
uniform mat4 uTransform;

void main() {

   
    gl_Position = uViewProj * uTransform * vec4(aPosition, 1.0f) ;
    
    vNormal = mat3(transpose(inverse(uTransform))) * aNormal;
    vUV = aUV;
    vFragPos = vec3(uTransform * vec4(aPosition, 1.0));

 
}