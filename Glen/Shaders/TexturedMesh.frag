#version 330 core

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	int hasSpecularMap;
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
	DirectionalLight directionalLight;
	PointLight pointLights[10];
	vec4 cameraPosition;
	int pointLightCount;
};

in VS_OUT {
    vec3 worldPos;
    vec3 vertNormal;
    vec2 texCoords;
} vsOut;

uniform Material material;

out vec4 FragColor;

vec3 calculatePointLight (PointLight pointLight, vec3 normal, vec3 viewDir, vec3 diffuseColor, float specularIntensity) {

	float specularStrength = mix(0.1, specularIntensity, step(1.0f, material.hasSpecularMap));

	vec3 lightDir = normalize(pointLight.position.xyz - vsOut.worldPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0),32);
	float distance = length(pointLight.position.xyz - vsOut.worldPos);

	float attenuation = 1.0/(1.0 + 0.0025f *distance + 0.00007f * (distance*distance));

	vec3 ambient  = pointLight.ambient.xyz  * diffuseColor * attenuation;
	vec3 diffuse  = pointLight.diffuse.xyz  * diff * diffuseColor * attenuation;
	vec3 specular = pointLight.specular.xyz * spec * specularStrength * attenuation;

	return (ambient + diffuse + specular);
}

vec3 calculateDirectionalLight (vec3 normal, vec3 viewDir, vec3 diffuseColor, float specularIntensity) {
	vec4 clipPos = projectionMatrix * viewMatrix * vec4(vsOut.worldPos, 1.0);
	float fragDepth = (clipPos.z/ clipPos.w) * 0.5 + 0.5;
	//vec3 diffuseColor = debug_color(fragDepth);
	float specularStrength = mix(0.1, specularIntensity, step(1.0f, material.hasSpecularMap));

	vec3 lightDir = normalize(-directionalLight.direction.xyz);
    float diff = max(dot(normal, lightDir), 0.0);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0),32);

	vec3 ambient  = directionalLight.ambient.xyz  * diffuseColor;
	vec3 diffuse  = (directionalLight.diffuse.xyz  * diff) * diffuseColor;
	vec3 specular = (directionalLight.specular.xyz * spec) * specularStrength;

	return (ambient + diffuse + specular);
}

void main()
{
	vec4 diffuseColor = texture(material.texture_diffuse, vsOut.texCoords);
	float specularIntensity = texture(material.texture_specular, vsOut.texCoords).r;
	if(diffuseColor.a == 0 && (material.hasSpecularMap ==0 || specularIntensity < 0)) {
		discard;
	}
	vec3 viewDir = normalize(vec3(cameraPosition) - vsOut.worldPos);
	vec3 result = vec3(0,0,0);

	result += calculateDirectionalLight(vsOut.vertNormal, viewDir, diffuseColor.xyz, specularIntensity);

	for(int i = 0; i < pointLightCount; i++) {
		result += calculatePointLight(pointLights[i], vsOut.vertNormal, viewDir,  diffuseColor.xyz, specularIntensity);
	}

	FragColor = vec4(result,1.0);
		
}