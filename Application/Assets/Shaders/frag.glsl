#version 460 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vUV;

uniform int uTexture;
uniform int uTexture2;

uniform sampler2D u_Textures[32];

void main() {
    FragColor =  texture(u_Textures[1], vUV);
}