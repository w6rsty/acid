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

struct Light
{
    vec3 Position;

    vec3 Ambient;
    float AmbientIntensity;

    vec3 Diffuse;
    float DiffuseIntensity;

    vec3 Specular;
    float SpecularIntensity;
    float Shininess;
};

uniform vec3 u_CamPos;

#define MAX_POINT_LIGHTS 5
uniform Light u_Lights[MAX_POINT_LIGHTS];

vec3 CalcSpotLight(vec3 normal, Light light, vec3 viewPos, vec3 pos)
{
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;

    vec3 lightDir = normalize(light.Position - pos);
    float distance = length(light.Position - pos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 ambient = light.Ambient * light.AmbientIntensity;
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Diffuse * (diff * light.DiffuseIntensity * attenuation);

    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.Shininess);
    vec3 specular = light.Specular * (spec * light.SpecularIntensity * attenuation);

    return ambient + diffuse + specular;
}

void main()
{   
    vec4 texColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord);
    if (texColor.a < 0.1)
        discard;

    vec3 result = vec3(0.0);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        result += CalcSpotLight(v_Normal, u_Lights[i], u_CamPos, v_Position);
    }

    color = vec4(result * texColor.rgb * v_Color.rgb, 1.0);
}
