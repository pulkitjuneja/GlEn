#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec4 tangent;

uniform mat4 modelMatrix;
uniform mat4 prevModelMatrix;

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

out VS_OUT {
    vec4 worldPos;
	vec2 screenSpaceVel;
    vec3 vertNormal;
    vec2 texCoords;
	vec4 previousClipPostion;
	vec4 clipPosition;
	mat3 TBN;
} vsOut;


// TODO: create a different uniform structured for deffered
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

layout (std140) uniform taaUniforms {
	mat4 VPPrevNoJitter;
	mat4 VPPrevJittered;
	mat4 VPCurrentJittered;
	mat4 VPCurrentJitteredInverse;
	vec2 jitter;
	float feedback;
};
 

out vec4 FragPos;

void main() {
	vec4 homogenousVertexPosition = vec4(position, 1.0);
	vsOut.texCoords = texCoords;
	vsOut.worldPos = modelMatrix* homogenousVertexPosition;
	mat3 normalMatrix = mat3(modelMatrix);
	vec3 T = normalize(vec3(modelMatrix * vec4(tangent.xyz,1.0)));
	vec3 N = normalize(normalMatrix * normal);
	vsOut.vertNormal = N;
	vec3 B = cross(N,T) * tangent.w;
	vsOut.TBN = mat3(T, B, N);
	FragPos = (modelMatrix*homogenousVertexPosition);

	// TODO maske this configurable
	gl_Position = VPCurrentJittered * FragPos;

	// velocity calculation
	vsOut.clipPosition = projectionMatrix * viewMatrix * FragPos;
	vsOut.previousClipPostion = VPPrevNoJitter * prevModelMatrix * homogenousVertexPosition;
	vsOut.screenSpaceVel = (vsOut.previousClipPostion - vsOut.clipPosition).xy;
}