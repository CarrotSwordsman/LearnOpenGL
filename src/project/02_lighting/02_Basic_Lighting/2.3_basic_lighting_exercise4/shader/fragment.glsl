#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 Result;


void main() {
    // 简单冯模型
    vec3 result = Result;

    FragColor = vec4(result, 1.0);
}