#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emmision;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;   // 聚光
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // // 聚光
    // vec3 lightDir = normalize(light.position - FragPos);
    // float theta = dot(lightDir, normalize(-light.direction));
    // if (theta > light.cutOff) { // 这里是比较余弦的值
    //     // 环境光
    //     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //     // 漫反射
    //     vec3 norm = normalize(Normal);
    //     float diff = max(dot(norm, lightDir), 0.0);
    //     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //     // 镜面反射
    //     vec3 viewDir = normalize(viewPos - FragPos);
    //     vec3 reflectDir = reflect(-lightDir, norm); // 这里将入射光线按法向量对称过去
    //     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //     vec3 specular = light.specular * spec * (vec3(texture(material.specular, TexCoords)));
    //     // 衰减
    //     float distance = length(light.position - FragPos);
    //     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    //     // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
    //     diffuse   *= attenuation;
    //     specular *= attenuation;   
            
    //     vec3 result = ambient + diffuse + specular;
    //     // 简单冯模型
    //     FragColor = vec4(result, 1.0);
    // }
    // else
    //     FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
    
    // 边缘软化，设置内光切和外光切
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // 这里将入射光线按法向量对称过去
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * (vec3(texture(material.specular, TexCoords)));
    // 边缘软化
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   // clmap函数保证强度在0-1之间
    diffuse *= intensity;
    specular *= intensity;
    // 这里不对环境光处理，保证范围外有光
    // 衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;   

    vec3 result = ambient + diffuse + specular;
    // 简单冯模型
    FragColor = vec4(result, 1.0);

}