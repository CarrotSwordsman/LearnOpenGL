#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main() {
    // mix线性插值，返回0.8*第一个参数+0.2*第二个参数
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);  // 混合多张纹理
    
    // 用uniform作为mix的参数，改变纹理可见度
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);  
}