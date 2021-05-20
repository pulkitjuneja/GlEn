#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_normal;
	sampler2D texture_metallic;
	sampler2D texture_roughness;
	sampler2D texture_oclussion;
	int hasSpecularMap;
	int hasNormalMap;
	// Temp solution remove
	int usePBRWorkflow;
};

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

in VS_OUT {
    vec3 worldPos;
    vec3 vertNormal;
    vec2 texCoords;
	mat3 TBN;
} vsOut;

in vec3 FragPos;

uniform Material material;

void main()
{    
	vec3 normal = texture(material.texture_normal, vsOut.texCoords).rgb;
	normal = normal * 2.0 - 1.0f;
	normal = normalize(vsOut.TBN * normal); 
	// normal = mat3(viewMatrix) * normal; 
	vec4 diffuseColor = texture(material.texture_diffuse, vsOut.texCoords);
	float specularIntensity = texture(material.texture_specular, vsOut.texCoords).r;
	if(diffuseColor.a == 0 && (material.hasSpecularMap ==0 || specularIntensity < 0)) {
		discard;
	}
	gPosition.xyz = FragPos;
	gPosition.w = texture(material.texture_metallic, vsOut.texCoords).r;
    gNormal.xyz = mix(vsOut.vertNormal, normal, step(1.0f, material.hasNormalMap));
	gNormal.w = texture(material.texture_roughness, vsOut.texCoords).r;
	gAlbedoSpec.xyz = diffuseColor.xyz;
	gAlbedoSpec.w = mix(mix(0.1f, specularIntensity, step(1.0f, material.hasSpecularMap)), 
	texture(material.texture_oclussion, vsOut.texCoords).r,step(1.0f, material.usePBRWorkflow));
}