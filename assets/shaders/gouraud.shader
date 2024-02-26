#type vertex
#version 410 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec4 v_FragCol;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    vec3 lightpos = vec3(2.0, 2.0, 0.0);
    vec3 lightcol = vec3(1.0, 1.0, 1.0);

    // simple lighting(gouraud shading)
    vec3 normal = normalize(a_Normal);
    vec3 lightdir = normalize(lightpos - a_Position);
    float diffuse = max(dot(normal, lightdir), 0.0);
    v_FragCol = vec4(diffuse * lightcol, 1.0);

    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core
layout (location = 0) out vec4 color;

in vec4 v_FragCol;

uniform vec4 u_Color;

void main()
{
    color = v_FragCol * u_Color;
}