#type vertex
#version 410 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;

out vec4 v_Color;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_Color = a_Color;
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core
layout (location = 0) out vec4 color;

in vec4 v_Color;

void main()
{   
    color = v_Color;
}