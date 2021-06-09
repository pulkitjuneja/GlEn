#version 330 core

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_normal;
	sampler2D texture_metallic;
	sampler2D texture_roughness;
	sampler2D texture_occlusion;
	int hasSpecularMap;
	int hasNormalMap;
	// Temp solution remove
	float usePBRWorkflow;
};

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

layout (std140) uniform csmUniforms 
{
	int splitCount;
	mat4 textureMatrices[8];
	float farBounds[8];
};

in VS_OUT {
    vec3 worldPos;
    vec3 vertNormal;
    vec2 texCoords;
	mat3 TBN;
} vsOut;


uniform sampler2DArrayShadow shadowMap;

uniform Material material;

out vec4 FragColor;


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


vec3 calculatePointLight (PointLight pointLight, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 PBRInfo) {

	// float specularStrength = mix(0.1, specularIntensity, step(1.0f, material.hasSpecularMap));
	vec3 lightDir = normalize(pointLight.position.xyz - vsOut.worldPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float distance = length(pointLight.position.xyz - vsOut.worldPos);

		float attenuation = 1.0 - distance / pointLight.radius;
		vec3 F0 = vec3(0.04); 
		F0  = mix(F0, diffuseColor, PBRInfo.x);
		vec3 F  = fresnelSchlick(max(dot(halfDir, viewDir), 0.0), F0);

		float NDF = DistributionGGX(normal, halfDir, PBRInfo.y);       
		float G   = GeometrySmith(normal, viewDir, lightDir, PBRInfo.y);  

		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0);
		vec3 specular     = numerator / max(denominator, 0.001);  

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - PBRInfo.x;	

		float NdotL = max(dot(normal, lightDir), 0.0);
		vec3 radiance = pointLight.diffuse.xyz * attenuation * pointLight.intensity;
		vec3 color = (kD * diffuseColor / 3.1415 + specular) * radiance * NdotL;
		return color + vec3(0.03) * diffuseColor * PBRInfo.z;	
}


float ShadowCalculation(float fragDepth, vec3 normal, vec3 lightDirection)
{
	int index = 0 ;
	float blend = 0.0f;

	for (int i = 0; i < splitCount - 1; i++) {
		if(fragDepth > farBounds[i])
			index = i + 1;
	}

	blend = clamp((fragDepth - farBounds[index]*0.995)*200.0,0.0,1.0); // what is this doing exactly ??
	vec4 lightSpacePos = textureMatrices[index] * vec4(vsOut.worldPos, 1.0f);
	float currentDepth = lightSpacePos.z;
	float bias = max(0.0005 * (1.0 - dot(normal, lightDirection)), 0.0005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0).xy;

	for(int x = -1; x <= 1; ++x)
	{
	    for(int y = -1; y <= 1; ++y)
	    {
	        float pcfDepth = texture(shadowMap, vec4(lightSpacePos.xy + vec2(x, y) * texelSize, float(index), currentDepth - 0.0005)); 
	        shadow += pcfDepth;         
	    }    
	}
	shadow /= 9.0;

	return shadow;
}

//vec3 debug_color(float fragDepth)
//{
//	int index = 0;
//
//	// Find shadow cascade.
//	for (int i = 0; i < splitCount - 1; i++)
//	{
//		if (fragDepth > farBounds[i])
//			index = i + 1;
//	}
//
//	if (index == 0)
//		return vec3(0.5, 0.0, 0.0);
//	else if (index == 1)
//		return vec3(0.0, 0.5, 0.0);
//	else if (index == 2)
//		return vec3(0.0, 0.0, 0.5);
//	else
//		return vec3(2.0, 1.0, 1.0);
//}

vec3 calculateDirectionalLight (vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 PBRInfo) {
	vec4 clipPos = projectionMatrix * viewMatrix * vec4(vsOut.worldPos, 1.0);
	float fragDepth = (clipPos.z/ clipPos.w) * 0.5 + 0.5;
	//vec3 diffuseColor = debug_color(fragDepth);
	vec3 lightDir = normalize(-directionalLight.direction.xyz);
	vec3 halfwayDir = normalize(lightDir + viewDir);

  	vec3 F0 = vec3(0.04); 
	F0  = mix(F0, diffuseColor, PBRInfo.x);
	vec3 F  = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);

	float NDF = DistributionGGX(normal, halfwayDir, PBRInfo.y);       
	float G   = GeometrySmith(normal, viewDir, lightDir, PBRInfo.y);  

	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0);
	vec3 specular     = numerator / max(denominator, 0.001);  

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - PBRInfo.x;	

	float NdotL = max(dot(normal, lightDir), 0.0);
	vec3 radiance = directionalLight.diffuse.xyz * directionalLight.intensity;
	vec3 color = (kD * diffuseColor / 3.1415 + specular) * radiance * NdotL;
	return color + vec3(0.03) * diffuseColor * PBRInfo.z;	
}

void main()
{
	vec4 diffuseColor = texture(material.texture_diffuse, vsOut.texCoords);
	float specularIntensity = texture(material.texture_specular, vsOut.texCoords).r;
	if(diffuseColor.a == 0 && (material.hasSpecularMap ==0 || specularIntensity < 0)) {
		discard;
	}
	vec3 normal = texture(material.texture_normal, vsOut.texCoords).rgb;
	normal = normal * 2.0 - 1.0f;
	normal = normalize(vsOut.TBN * normal);
	normal = mix(vsOut.vertNormal, normal, step(1.0f, material.hasNormalMap));
	vec3 viewDir = normalize(vec3(cameraPosition) - vsOut.worldPos);
	vec3 result = vec3(0,0,0);
	float metallic = texture(material.texture_metallic, vsOut.texCoords).r;
	float roughness = texture(material.texture_roughness, vsOut.texCoords).r;
	float occlusion = texture(material.texture_occlusion, vsOut.texCoords).r;

	vec3 PBRInfo = vec3(metallic, roughness, occlusion);

	result += calculateDirectionalLight(normal, viewDir, diffuseColor.xyz, PBRInfo);

	for(int i = 0; i < pointLightCount; i++) {
		result += calculatePointLight(pointLights[i], normal, viewDir,  diffuseColor.xyz, PBRInfo);
	}

	FragColor = vec4(result,1.0);
		
}