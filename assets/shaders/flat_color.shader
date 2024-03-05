#type vertex
#version 410 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

layout (std140) uniform Matrices
{
    mat4 u_Projection;
    mat4 u_View;
};

uniform mat4 u_Model;

out VertexOut
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} VtxOut;

void main()
{
    VtxOut.FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    VtxOut.Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    VtxOut.TexCoord = a_TexCoord;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core
layout (location = 0) out vec4 color;

struct PointLight
{
    vec3 Position;

    float Constant;
    float Linear;
    float Quadratic;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct SpotLight
{
    vec3 Position;
    vec3 Direction;
    float Constant;
    float Linear;
    float Quadratic;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct DirLight
{
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16
#define MAX_DIR_LIGHTS 4

layout (std140) uniform Lights
{
    PointLight u_PointLights[MAX_POINT_LIGHTS];
    SpotLight u_SpotLights[MAX_SPOT_LIGHTS];
    DirLight u_DirLights[MAX_DIR_LIGHTS];
    int u_PointLightCount;
    int u_SpotLightCount;
    int u_DirLightCount;
};

uniform vec4 u_Tint;
uniform vec3 u_CamPos;

in VertexOut
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} VtxOut;

vec3 CalcPointLight(vec3 normal, PointLight light, vec3 viewDir, vec3 fragPos);
vec3 CalcSpotLight(vec3 normal, SpotLight light, vec3 viewDir, vec3 fragPos);
vec3 CalcDirLight(vec3 normal, DirLight light, vec3 viewDir);

void main()
{   
    vec3 normal = normalize(VtxOut.Normal);
    vec3 viewDir = normalize(u_CamPos - VtxOut.FragPos);

    vec3 result = vec3(0.0);
    for (int i = 0; i < u_PointLightCount; i++)
    {
        result += CalcPointLight(normal, u_PointLights[i], viewDir, VtxOut.FragPos);
    }
    for (int i = 0; i < u_SpotLightCount; i++)
    {
        result += CalcSpotLight(normal, u_SpotLights[i], viewDir, VtxOut.FragPos);
    }
    for (int i = 0; i < u_DirLightCount; i++)
    {
        result += CalcDirLight(normal, u_DirLights[i], viewDir);
    }

    color = vec4(result, 1.0) * u_Tint;
}

vec3 CalcPointLight(vec3 normal, PointLight light, vec3 viewDir, vec3 fragPos)
{
    float shininess = 2.0;

    vec3 lightDir = normalize(light.Position - fragPos);
    float distance = length(light.Position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    float diff = max(dot(normal, lightDir), 0.0);
    float illuminated = diff > 0.0 ? 1.0 : 0.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = light.Ambient * illuminated;
    vec3 diffuse = light.Diffuse * diff;
    vec3 specular = light.Specular * spec * illuminated;

    return attenuation * (ambient + diffuse + specular);
}

vec3 CalcSpotLight(vec3 normal, SpotLight light, vec3 viewDir, vec3 fragPos)
{
    float shininess = 10.0;

    vec3 lightDir = normalize(light.Position - fragPos);
    float distance = length(light.Position - fragPos);
    float up = pow(max(dot(normalize(-light.Direction), lightDir), 0.0), shininess);
    float attenuation = up / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    float diff = max(dot(normal, lightDir), 0.0);
    float illuminated = diff > 0.0 ? 1.0 : 0.0;
    float spec = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), shininess);

    vec3 ambient = light.Ambient * illuminated;
    vec3 diffuse = light.Diffuse * diff;
    vec3 specular = light.Specular * spec * illuminated;

    return attenuation * (ambient + diffuse + specular);
}

vec3 CalcDirLight(vec3 normal, DirLight light, vec3 viewDir)
{
    float shininess = 2.0;

    vec3 lightDir = normalize(-light.Direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    float diff = max(dot(normal, lightDir), 0.0);
    float illuminated = diff > 0.0 ? 1.0 : 0.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = light.Ambient;
    vec3 diffuse = light.Diffuse * diff;
    vec3 specular = light.Specular * spec * illuminated;

    return (ambient + diffuse + specular);
}