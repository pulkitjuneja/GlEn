#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 modelMatrix;

struct PointLight {
	vec4 position;
	vec4 diffuse;
	float radius;
	float intensity;
};

struct DirectionalLight {
	vec4 direction;
	vec4 diffuse;
	float intensity;
};

layout (std140) uniform perFrameUniforms
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 inverseProjectionMatrix;
	mat4 inverseViewMatrix;
	DirectionalLight directionalLight;
	vec4 cameraPosition;
	int pointLightCount;
};

out VS_OUT {
    vec3 worldPos;
    vec3 vertNormal;
    vec2 texCoords;
} vsOut;


void main() {
	vec4 homogenousVertexPosition = vec4(position.x, position.y, position.z, 1.0);
	vsOut.vertNormal = normal;
	vsOut.texCoords = texCoords;
	vsOut.worldPos = position;
	gl_Position =  projectionMatrix*viewMatrix*modelMatrix*homogenousVertexPosition;
}
	