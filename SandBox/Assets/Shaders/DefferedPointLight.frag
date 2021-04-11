#version 330 core

out vec4 FragColor;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;

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

in vec4 fragPos;

flat in int lightIndex;

void main () {
	vec2 fragTexcoords = (fragPos.xy / fragPos.w) * 0.5 + 0.5;
	vec4 colorData = texture(albedoTexture,fragTexcoords);
	vec3 worldPos = texture(positionTexture, fragTexcoords).xyz;
	vec3 worldNormal = texture(normalTexture, fragTexcoords).xyz;
	vec3 diffuseColor = colorData.xyz;
	float specularStrength = colorData.w;

	vec3 lightPosition = pointLights[lightIndex].position.xyz;
	float radius = pointLights[lightIndex].radius;
	vec4 lightDiffuse = pointLights[lightIndex].diffuse;
	vec4 lightSpecular = pointLights[lightIndex].specular;

	vec3 lightToPosVector = lightPosition - worldPos;
	float lightDist = length(lightToPosVector);

	// use normalize instead ?
	vec3 lightDir = lightToPosVector / (lightDist);
	float ztest = step(0.0, radius - lightDist);

	float attenuation = 1.0 - lightDist / radius;
	vec3 viewDir = normalize(cameraPosition.xyz - worldPos);
	vec3 halfDir = normalize(lightDir + viewDir);

	float diff = max(dot(worldNormal,lightDir),0.0);
	float spec = pow(max(dot(halfDir, worldNormal), 0.0),32);

	vec3 diffuse = diff*diffuseColor*lightDiffuse.xyz;
	vec3 specular = spec*lightSpecular.xyz*specularStrength;

	FragColor = vec4((diffuse + specular)*ztest*attenuation, 1.0);
}