#version 330 core

layout(location = 0) in vec3 position;
out vec4 fragPos;
flat out int lightIndex;

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
	mat4 lightSpaceMatrix;
	DirectionalLight directionalLight;
	PointLight pointLights[10];
	vec4 cameraPosition;
	int pointLightCount;
};

void main () {
	vec3 lightPosition = pointLights[gl_InstanceID].position.xyz;
	float radius = pointLights[gl_InstanceID].radius;
	lightIndex = gl_InstanceID;
	fragPos = projectionMatrix * viewMatrix * vec4((position * radius) + lightPosition, 1.0);
	gl_Position = fragPos;
}