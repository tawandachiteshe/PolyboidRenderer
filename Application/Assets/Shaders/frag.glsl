#version 450 core

out vec4 FragColor;

in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;


void main() {
    FragColor =  texture(uTexture3, vUV) * texture(uTexture1, vUV) * texture(uTexture2, vUV);
}