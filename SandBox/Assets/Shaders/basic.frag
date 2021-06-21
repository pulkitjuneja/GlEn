#version 330 core
out vec4 FragColor; 

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
	mat4 lightSpaceMatrix;
	DirectionalLight directionalLight;
	vec4 cameraPosition;
	int pointLightCount;
};

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_normal;
	sampler2D texture_metallic;
	sampler2D texture_roughness;
	sampler2D texture_occlusion;
	int hasSpecularMap;
	int hasNormalMap;
	float usePBRWorkflow;
};

in VS_OUT {
    vec3 worldPos;
    vec3 vertNormal;
    vec2 texCoords;
	mat3 TBN;
} vsOut;

uniform Material material;

void main()
{ 
	vec4 diffuseColor = texture(material.texture_diffuse, vsOut.texCoords);
	float specularIntensity = texture(material.texture_specular, vsOut.texCoords).r;
	if(diffuseColor.a == 0 && (material.hasSpecularMap ==0 || specularIntensity < 0)) {
		discard;
	}
}