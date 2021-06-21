#version 430 core

#define MAX_POINT_LIGHTS 4096

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
	vec4 cameraPosition;
	int pointLightCount;
};

layout(std430, binding = 5) readonly buffer point_light_buffer {
	PointLight point_lights[MAX_POINT_LIGHTS];
};

void main () {
	vec3 lightPosition = point_lights[gl_InstanceID].position.xyz;
	float radius = point_lights[gl_InstanceID].radius;
	lightIndex = gl_InstanceID;
	fragPos = projectionMatrix * viewMatrix * vec4((position * radius) + lightPosition, 1.0);
	gl_Position = fragPos;
}