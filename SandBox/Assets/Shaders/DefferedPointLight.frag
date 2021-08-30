#version 430 core

#define MAX_POINT_LIGHTS 4096

out vec4 FragColor;

uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D depthTexture;
uniform sampler2D PBRInfoTexture;

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

uniform samplerCube irradianceMap;

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

in vec4 fragPos;

flat in int lightIndex;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.1415 * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  



void main () {
	vec2 texSize = textureSize(normalTexture, 0).xy;
	vec2 texCoord = gl_FragCoord.xy / texSize;

	vec4 colorData = texture(albedoTexture,texCoord);

	float z = texture(depthTexture, texCoord).r * 2.0 - 1;
	float x = texCoord.x * 2 - 1;
    float y = texCoord.y * 2 - 1;
	vec4 projectedPos = vec4(x, y, z, 1.0f);

	vec4 viewPosition =	inverseProjectionMatrix * projectedPos;
	viewPosition.xyz /= viewPosition.w;
	vec4 worldPos = inverseViewMatrix * vec4(viewPosition.xyz, 1.0f);

	vec3 worldNormal = texture(normalTexture, texCoord).xyz;
	vec3 diffuseColor = colorData.xyz;
	float specularStrength = colorData.w;

	vec3 lightPosition = point_lights[lightIndex].position.xyz;
	float radius = point_lights[lightIndex].radius;
	vec4 lightDiffuse = point_lights[lightIndex].diffuse;
	//vec4 lightSpecular = pointLights[lightIndex].specular;
	vec3 lightToPosVector = lightPosition - worldPos.xyz;
	float lightDist = length(lightToPosVector);
	vec3 lightDir = lightToPosVector / (lightDist);
	float ztest = step(0.0, radius - lightDist);
	vec4 PBRInfo = texture(PBRInfoTexture, texCoord);

	vec3 viewDir = normalize(cameraPosition.xyz - worldPos.xyz);
	vec3 halfDir = normalize(lightDir + viewDir);
	vec3 result;

	if(PBRInfo.w == 1.0f) {
		float attenuation = 1.0 - lightDist / radius;
		vec3 F0 = vec3(0.04); 
		F0  = mix(F0, diffuseColor, PBRInfo.x);
		vec3 F  = fresnelSchlick(max(dot(halfDir, viewDir), 0.0), F0);

		float NDF = DistributionGGX(worldNormal, halfDir, PBRInfo.y);       
		float G   = GeometrySmith(worldNormal, viewDir, lightDir, PBRInfo.y);  

		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * max(dot(worldNormal, viewDir), 0.0) * max(dot(worldNormal, lightDir), 0.0);
		vec3 specular     = numerator / max(denominator, 0.001);  

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - PBRInfo.x;	

		float NdotL = max(dot(worldNormal, lightDir), 0.0);
		vec3 radiance = lightDiffuse.xyz * attenuation * point_lights[lightIndex].intensity;
		vec3 color = (kD * diffuseColor / 3.1415 + specular) * radiance * NdotL;

		kS = fresnelSchlickRoughness(max(dot(worldNormal, viewDir), 0.0), F0, PBRInfo.y);
		kD = 1.0 - kS;
		vec3 irradiance = texture(irradianceMap, worldNormal).rgb;
		vec3 diffuse = irradiance * diffuseColor;
		vec3 ambient = kD * diffuse * PBRInfo.z;

		result = color;
		result *= ztest;
	
	} else {

		float Ka = 0.04f;
		float Ks = specularStrength;
		float Kd = 1.0f - Ks;
		float attenuation = 1.0 - lightDist / radius;

		float diff = max(dot(worldNormal,lightDir),0.0);
		float spec = pow(max(dot(halfDir, worldNormal), 0.0),32);

		float diffuse  = point_lights[lightIndex].intensity * diff * Kd;
		float specular = point_lights[lightIndex].intensity * spec * Ks;
		result = (diffuse) * diffuseColor * point_lights[lightIndex].diffuse.xyz + specular * point_lights[lightIndex].diffuse.xyz;
		result*= ztest * attenuation;
	}

	FragColor = vec4(result, 1.0);
}