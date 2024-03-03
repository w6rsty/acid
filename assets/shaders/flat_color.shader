#type vertex
#version 410 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

layout (std140) uniform Matrices
{
    mat4 Projection;
    mat4 View;
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
    gl_Position = Projection * View * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core
layout (location = 0) out vec4 color;

uniform vec4 u_TintColor;

in VertexOut
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} VtxOut;

void main()
{   
    color = u_TintColor;
}