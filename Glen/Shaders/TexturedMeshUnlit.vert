#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 vertNormal;
out vec2 vertTexCoords;

void main() {
	vec4 homogenousVertexPosition = vec4(position.x, position.y, position.z, 1.0);
	vertNormal = normal;
	vertTexCoords = texCoords;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * homogenousVertexPosition;
}
