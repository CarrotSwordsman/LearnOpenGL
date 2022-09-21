#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);  // 混合多张纹理
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, -TexCoord.y)), 0.2);    // 将纹理绕y轴对称
    // mix线性插值，返回0.8*第一个参数+0.2*第二个参数
}