#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 lightSpaceMatrix;

uniform mat4 modelMatrix;
//uniform mat4 viewMatrix;
//uniform mat4 projectionMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(position, 1.0);
}  