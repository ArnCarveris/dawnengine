#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord0;

uniform mat4 model_matrix;
uniform mat4 mvp_matrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 Color;

void main()
{
    TexCoord = texcoord0;
    Color = vec3(1.0, 1.0, 1.0);
    Normal = (model_matrix * vec4(normal, 0.0)).xyz;
    gl_Position = mvp_matrix * vec4(position, 1.0);
}