#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec4 tangent;

uniform mat4 modelMatrix;
//uniform mat4 normalMatrix;

struct PointLight {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
	float radius;
};

struct DirectionalLight {
	vec4 direction;
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
};

out VS_OUT {
    vec3 worldPos;
    vec3 vertNormal;
    vec2 texCoords;
	mat3 TBN;
} vsOut;


// TODO: create a different uniform structured for deffered
layout (std140) uniform perFrameUniforms
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 lightSpaceMatrix;
	DirectionalLight directionalLight;
	PointLight pointLights[10];
	vec4 cameraPosition;
	int pointLightCount;
};

 

out vec3 FragPos;

void main() {
	vec4 homogenousVertexPosition = vec4(position, 1.0);
	vsOut.texCoords = texCoords;
	vsOut.worldPos = vec3(modelMatrix* homogenousVertexPosition);
	mat3 normalMatrix = mat3(modelMatrix);
	vec3 T = normalize(vec3(modelMatrix * vec4(tangent.xyz,1.0)));
	vec3 N = normalize(normalMatrix * normal);
	vsOut.vertNormal = N;
	// re-orthogonalize T with respect to N
//	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N,T) * tangent.w;
	vsOut.TBN = mat3(T, B, N);
	FragPos = (modelMatrix*homogenousVertexPosition).xyz;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * homogenousVertexPosition;
}