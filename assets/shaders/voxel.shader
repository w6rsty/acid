#type vertex
#version 410 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Normal;
layout (location = 4) in float a_TexIndex;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 v_Normal;
out float v_TexIndex;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_Position = a_Position;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
    v_TexIndex = a_TexIndex;
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core
layout (location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in float v_TexIndex;

uniform sampler2D u_Textures[16];

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

struct DirLight
{
    vec3 Direction;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct SpotLight
{
    vec3 Position;
    vec3 Direction;

    float CutOff;
    float OuterCutOff;

    float Constant;
    float Linear;
    float Quadratic;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};


uniform vec3 u_CamPos;

#define MAX_POINT_LIGHTS 4
uniform DirLight u_DirLight;
uniform SpotLight u_SpotLight;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform float u_Gamma;

vec3 CalcPointLight(vec3 normal, PointLight light, vec3 viewDir, vec3 fragPos);
vec3 CalcSpotLight(vec3 normal, SpotLight light, vec3 viewDir, vec3 fragPos);
vec3 CalcDirLight(vec3 normal, DirLight light, vec3 viewDir);

void main()
{   
    vec4 texColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord);
    if (texColor.a < 0.1)
        discard;

    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_CamPos - v_Position);

    vec3 result = vec3(0.0);
    result += CalcDirLight(norm, u_DirLight, viewDir);
    result += CalcSpotLight(norm, u_SpotLight, viewDir, v_Position);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(norm, u_PointLights[i], viewDir, v_Position);
    }

    vec3 hdr = result * texColor.rgb * v_Color.rgb;
    vec3 mapped = pow(hdr, vec3(1.0 / u_Gamma));

    color = vec4(mapped, texColor.a);
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