#version 330 core

out vec4 FragColor;

uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D depthTexture;

uniform sampler2DArrayShadow shadowMap;

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

void main()
{   
	vec2 texSize = textureSize(normalTexture, 0).xy;
	vec2 texCoord = gl_FragCoord.xy / texSize;

	float z = texture(depthTexture, texCoord).r * 2.0 - 1;
	float x = texCoord.x * 2 - 1;
    float y = texCoord.y * 2 - 1;
	vec4 projectedPos = vec4(x, y, z, 1.0f);

	vec4 viewPosition =	inverseProjectionMatrix * projectedPos;
	viewPosition.xyz /= viewPosition.w;
	vec4 worldPos = inverseViewMatrix * vec4(viewPosition.xyz, 1.0f);

	vec3 worldNormal = texture(normalTexture, texCoord).xyz;
	vec3 viewDir = normalize(vec3(cameraPosition) - worldPos.xyz);
	float fragDepth = projectedPos.z * 0.5 + 0.5;
	vec4 colorData = texture(albedoTexture,texCoord); 
	vec3 diffuseColor = colorData.xyz;
	float specularStrength = colorData.w;
	vec3 lightDir = normalize(-directionalLight.direction.xyz);
	float diff = max(dot(worldNormal, lightDir), 0.0);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, worldNormal), 0.0),32);

	float shadow = ShadowCalculation(worldPos.xyz, fragDepth, worldNormal, lightDir);

	vec3 ambient  = directionalLight.ambient.xyz  * diffuseColor;
	vec3 diffuse  = (1.0 - shadow)*(directionalLight.diffuse.xyz  * diff) * diffuseColor;
	vec3 specular = (1.0 - shadow)*(directionalLight.specular.xyz * spec) * specularStrength;

	vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}  