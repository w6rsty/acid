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
    vec3 Direction;

    vec3 Ambient;
    float AmbientIntensity;

    vec3 Diffuse;
    float DiffuseIntensity;

    vec3 Specular;
    float SpecularIntensity;
    float Shininess;
};

uniform vec3 u_CamPos;
uniform Light u_Light;

void main()
{   
    vec4 texColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord);
    if (texColor.a < 0.1)
        discard;

    vec3 sky = vec3(0.3, 0.3, 0.3);

    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;

    vec3 lightDir = normalize(u_Light.Position - v_Position);
    float distance = length(u_Light.Position - v_Position);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 ambient = u_Light.Ambient * u_Light.AmbientIntensity;
    vec3 norm = normalize(v_Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.Diffuse * (diff * u_Light.DiffuseIntensity * attenuation);

    vec3 viewDir = normalize(u_CamPos - v_Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Light.Shininess);
    vec3 specular = u_Light.Specular * (spec * u_Light.SpecularIntensity * attenuation);

    vec3 result = (ambient + diffuse + specular + sky) * vec3(texColor) * vec3(v_Color);
    color = vec4(result, 1.0);
}