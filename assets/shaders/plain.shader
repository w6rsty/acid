#type vertex
#version 410 core
layout (location = 0) in vec3 a_Position;

out vec3 v_FragPos;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_FragPos = a_Position;
    gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core
layout (location = 0) out vec4 color;

in vec3 v_FragPos;

uniform vec4 u_Color;

void main()
{
    color = u_Color * vec4(v_FragPos, 1.0);
}