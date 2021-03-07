#version 330 core
out vec4 FragColor;

in vec3 vertNormal;
in vec2 vertTexCoords;

struct Material {
	int diffuseCount;
	int specularCount;
	sampler2D texture_diffuse[5];
	sampler2D texture_specular[5];
};

uniform Material material;

void main()
{
	FragColor = texture(material.texture_diffuse[0],vertTexCoords);
}
