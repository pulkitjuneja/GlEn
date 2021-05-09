#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 worldPos;
    vec3 vertNormal;
    vec2 texCoords;
} vsOut;

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	int hasSpecularMap;
};

uniform Material material;
uniform vec3 debugColor;

void main()
{
	FragColor = vec4(debugColor,1);
}
