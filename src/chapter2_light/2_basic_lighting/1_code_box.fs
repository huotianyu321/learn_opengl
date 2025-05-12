#version 330 core
out vec4 FragColor;

in vec3 FragPos; 
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor; 
uniform vec3 lightPos; // ��Դλ��
uniform vec3 viewPos; // ���λ��


void main()
{
    // ������
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // ������
    float diffuseStrength = 1.0;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    // �����������ļнǣ�����������ǿ��
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;

    // ���淴��
    float specularStrength = 0.5��
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    // ���淴��ǿ��
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}