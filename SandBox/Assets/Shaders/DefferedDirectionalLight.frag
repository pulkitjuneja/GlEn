#version 330 core

out vec4 FragColor;

uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D depthTexture;
uniform sampler2D PBRInfoTexture;

uniform samplerCube skybox;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLut;


uniform bool skyBoxCheck;

uniform sampler2DArrayShadow shadowMap;

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

layout (std140) uniform csmUniforms 
{
	int splitCount;
	mat4 textureMatrices[8];
	float farBounds[8];
};

in vec2 fragTexcoords;
in vec4 fragPos;

vec3 debug_color(float fragDepth)
{
	int index = 0;

	// Find shadow cascade.
	for (int i = 0; i < splitCount - 1; i++)
	{
		if (fragDepth > farBounds[i])
			index = i + 1;
	}

	if (index == 0)
		return vec3(0.5, 0.0, 0.0);
	else if (index == 1)
		return vec3(0.0, 0.5, 0.0);
	else if (index == 2)
		return vec3(0.0, 0.0, 0.5);
	else
		return vec3(2.0, 1.0, 1.0);
}

float ShadowCalculation(vec3 worldPos, float fragDepth, vec3 normal, vec3 lightDirection)
{
	int index = 0 ;
	float blend = 0.0f;

	for (int i = 0; i < splitCount - 1; i++) {
		if(fragDepth > farBounds[i])
			index = i + 1;
	}

	blend = clamp((fragDepth - farBounds[index]*0.995)*200.0,0.0,1.0); // what is this doing exactly ??
	vec4 lightSpacePos = textureMatrices[index] * vec4(worldPos, 1.0f);
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


void main()
{   
	vec2 texSize = textureSize(normalTexture, 0).xy;
	vec2 texCoord = gl_FragCoord.xy / texSize;

	float depth = texture(depthTexture, texCoord).r;
	float z = depth * 2.0 - 1;
	float x = texCoord.x * 2 - 1;
    float y = texCoord.y * 2 - 1;
	vec4 projectedPos = vec4(x, y, z, 1.0f);

	vec4 viewPosition =	inverseProjectionMatrix * projectedPos;
	viewPosition.xyz /= viewPosition.w;
	
	vec4 worldPos = inverseViewMatrix * vec4(viewPosition.xyz, 1.0f);
	vec3 worldNormal = texture(normalTexture, texCoord).xyz;
	vec3 viewDir = normalize(vec3(cameraPosition) - worldPos.xyz);
	vec3 reflection = reflect(-viewDir, worldNormal);
	float fragDepth = projectedPos.z * 0.5 + 0.5;
	vec4 colorData = texture(albedoTexture,texCoord); 
	vec3 diffuseColor = colorData.xyz;
	float specularStrength = colorData.w;\
	vec4 PBRInfo = texture(PBRInfoTexture, texCoord);

	vec3 lightDir = normalize(-directionalLight.direction.xyz);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	vec3 result;

	if(PBRInfo.w == 1.0f) {
		vec3 F0 = vec3(0.04); 
		F0  = mix(F0, diffuseColor, PBRInfo.x);
		vec3 F  = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);

		float NDF = DistributionGGX(worldNormal, halfwayDir, PBRInfo.y);       
		float G   = GeometrySmith(worldNormal, viewDir, lightDir, PBRInfo.y);  

		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * max(dot(worldNormal, viewDir), 0.0) * max(dot(worldNormal, lightDir), 0.0);
		vec3 specular     = numerator / max(denominator, 0.001);  

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - PBRInfo.x;	

		float NdotL = max(dot(worldNormal, lightDir), 0.0);
		vec3 radiance = directionalLight.diffuse.xyz * directionalLight.intensity;
		vec3 color = (kD * diffuseColor / 3.1415 + specular) * radiance * NdotL;

		// ambient term calculation based on irradiance map
		F = fresnelSchlickRoughness(max(dot(worldNormal, viewDir), 0.0), F0, PBRInfo.y);
		kS = F;
		kD = 1.0 - kS;
		kD *= 1.0 - PBRInfo.x;
		vec3 irradiance = texture(irradianceMap, worldNormal).rgb;
		vec3 diffuse = irradiance * diffuseColor;

		const float MAX_REFLECTION_LOD = 4.0;
		vec3 prefilteredColor = textureLod(prefilterMap, reflection,  PBRInfo.y * MAX_REFLECTION_LOD).rgb;
		vec2 brdf  = texture(brdfLut, vec2(max(dot(worldNormal, viewDir), 0.0), PBRInfo.y)).rg;
		vec3 specularComp = prefilteredColor * (F * brdf.x + brdf.y);

		vec3 ambient = (kD * diffuse + specularComp) * PBRInfo.z;
		result = color + ambient;	
	} else {
		
		float Ka = 0.04f;
		float Ks = specularStrength;
		float Kd = 1.0f - Ks;
		float diff = max(dot(worldNormal, lightDir), 0.0);
		float spec = pow(max(dot(halfwayDir, worldNormal), 0.0),32);

		float shadow = ShadowCalculation(worldPos.xyz, fragDepth, worldNormal, lightDir);

		float ambient  = Ka;
		float diffuse  = (1.0 - shadow)* directionalLight.intensity * diff * Kd;
		float specular = (1.0 - shadow)* directionalLight.intensity * spec * Ks;
		result = (ambient + diffuse) * diffuseColor * directionalLight.diffuse.xyz + specular * directionalLight.diffuse.xyz;
	}

	vec3 cameraRelativePostion = worldPos.xyz - cameraPosition.xyz;
	if(skyBoxCheck) {
		result = mix(result, textureLod(prefilterMap, cameraRelativePostion, 1.2).rgb, step(1.0, depth));
	} else {
		result = mix(result, texture(brdfLut, texCoord).rgb, step(1.0, depth));
	}
	
    FragColor = vec4(result, 1.0);
}  