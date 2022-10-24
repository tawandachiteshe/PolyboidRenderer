#version 450 core

out vec4 FragColor;

in vec3 vNormal;
in vec2 vUV;
in vec3 vFragPos;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;


void main() {

    vec3 pos = vec3(0, 2, 3);
    vec3 viewPos = vec3(0, 1, 10);
    vec3 lightColor = vec3(0.45, 0.56, 0.65);
    float specularStrength = 0.9;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(pos - vFragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
     
    float ambientStrength = 0.9;
    vec3 ambient = ambientStrength  * lightColor;
  

    FragColor =  texture(uTexture1, vUV) * vec4(ambient + diffuse + specular, 1.0);
}