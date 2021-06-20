#version 430 core
#define MAX_LIGHTS_PER_TILE 1024
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

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

uniform int numberOfTilesX;
uniform int totalLightCount;
uniform sampler2D depthMap;

out vec4 fragColor;

layout(std430, binding = 3) readonly buffer visible_lights_indices {
	int lights_indices[];
};

layout(std430, binding = 4) readonly buffer debug_depth_buffer {
	float depth_buffer[];
};


void main () {
	ivec2 location = ivec2(gl_FragCoord.xy);
	ivec2 tileID = location / ivec2(16, 16);
	uint index = tileID.y * numberOfTilesX + tileID.x;
	float depth = depth_buffer[index];

//	vec2 screenSize = vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
//	vec2 text = vec2(location) / screenSize;
//	float depth = texture(depthMap, text).r;
//
	uint offset = index * MAX_LIGHTS_PER_TILE;
	uint i;
	for (i = 0; i < MAX_LIGHTS_PER_TILE && lights_indices[offset + i]!= -1; i++);

	float ratio = float(i) / float(totalLightCount);
	fragColor = vec4(vec3(ratio, ratio, ratio), 0.7f);
}
