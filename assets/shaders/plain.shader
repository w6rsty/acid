#type vertex
#version 410 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_FragPos = a_Position;
    v_Normal = a_Normal;
    v_TexCoord = a_TexCoord;
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core
layout (location = 0) out vec4 color;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform vec4 u_Color;

void main()
{   
    color = vec4(v_Normal, 1.0);
}