#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 DebugColor;

uniform sampler2D currentColorTexture;
uniform sampler2D currentDepthTexture;
uniform sampler2D velocityTexture;
uniform sampler2D colorAntiAliased;

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

layout (std140) uniform taaUniforms {
	mat4 prevViewMatrix;
	mat4 prevProjectionMatrix;
	mat4 jitteredProjMatrix;
	mat4 inverseJitteredProjMatrix;
	vec2 jitter;
	float feedback;
};

uniform mat3 YCoCGMatrix = mat3(0.25f, 0.5f, -0.25f, 0.5f, 0.f, 0.5f, 0.25f, -0.5f, -0.25f);
uniform mat3 RGBMatrix = mat3(1.f, 1.f, 1.f, 1.f, 0.f, -1.f, -1.f, 1.f, -1.f);

vec3 YCoCg(vec3 _inRGB)
{
  return _inRGB;
  return YCoCGMatrix * _inRGB;
}

vec3 RGB(vec3 _inYCoCG)
{
  return _inYCoCG;
  return RGBMatrix * _inYCoCG;
}

vec3 sampleRenderYCoCg(vec2 uv)
{
  return YCoCg(texture(currentColorTexture, uv).rgb);
}

bool clipped = false;

vec2 pixelSize = vec2(1/1920, 1/1080);

vec2 pixelOffsets3x3[9] = vec2[](
	vec2(0,0),
	vec2( pixelSize.x, 0.0f),
	vec2(-pixelSize.x, 0.0f),
	vec2(0.0f,  pixelSize.y),
	vec2(0.0f, -pixelSize.y),
	vec2( pixelSize.x,  pixelSize.y),
	vec2(-pixelSize.x,  pixelSize.y),
	vec2( pixelSize.x, -pixelSize.y),
	vec2(-pixelSize.x, -pixelSize.y)
);

vec2 pixelOffsets2x2[5] = vec2[] (
	vec2(0,0),
	vec2( pixelSize.x, 0.0f),
	vec2( -pixelSize.x, 0.0f),
	vec2(0.0f,  pixelSize.y),
	vec2(0.0f,  -pixelSize.y)
);

vec3 minSample3x3(vec2 uvCURRENT) {
	vec3 minSamp = vec3(1.0f, 1.0f, 1.0f);
	for( int i = 0; i<9; i++) {
		vec3 sample = sampleRenderYCoCg(pixelOffsets3x3[i] + uvCURRENT);
		minSamp = min(sample, minSamp);
	}
	return minSamp;
}

vec3 minSampleDirectNeighbours(vec2 uvCURRENT) {
	vec3 minSamp = vec3(1.0f, 1.0f, 1.0f);
	for(int i = 0; i<5; i++) {
		vec3 sample = sampleRenderYCoCg(pixelOffsets2x2[i] + uvCURRENT);
		minSamp = min(sample, minSamp);
	}
	return minSamp;
}

vec3 maxSample3x3(vec2 uvCURRENT) {
	vec3 maxSamp = vec3(0.0f, 0.0f, 0.0f);
	for( int i = 0; i<9; i++) {
		vec3 sample = sampleRenderYCoCg(pixelOffsets3x3[i] + uvCURRENT);
		maxSamp = max(sample, maxSamp);
	}
	return maxSamp;
}

vec3 maxSampleDirectNeighbours(vec2 uvCURRENT) {
	vec3 maxSamp = vec3(0.0f, 0.0f, 0.0f);
	for( int i = 0; i<5; i++) {
		vec3 sample = sampleRenderYCoCg(pixelOffsets2x2[i] + uvCURRENT);
		maxSamp = max(sample, maxSamp);
	}
	return maxSamp;
}

vec2 frontMostNeigbourCoord(vec2 coord) {

	float minSample = 10;
	int neighbour = -1;
	for( int i = 0; i<9; i++) {
		float depth = texture(currentDepthTexture, pixelOffsets3x3[i] + coord).r;
		// TODO convert this to step and mix later
		if(depth < minSample) {minSample = depth; neighbour = i;}
	}

	return pixelOffsets3x3[neighbour] + coord;
}

vec3 clipNeighbourhood(vec3 colourSample, vec2 uvCURRENT){
	vec3 colorMin = mix(minSample3x3(uvCURRENT), minSampleDirectNeighbours(uvCURRENT), 0.5f);
	vec3 colorMax = mix(maxSample3x3(uvCURRENT), maxSampleDirectNeighbours(uvCURRENT), 0.5f);

	vec3 YCoCgSample = YCoCg(colourSample.rgb);

	 vec3 aabbCentre = 0.5f * (colorMax + colorMin);
	 vec3 aabbWidth = 0.5f * (colorMax - colorMin);
	 vec3 centreToSample = YCoCgSample - aabbCentre;
	 vec3 ctsUnitSpace = centreToSample / aabbWidth;
	 float maximumDimension = max(abs(ctsUnitSpace.x), max(abs(ctsUnitSpace.y), abs(ctsUnitSpace.z)));
	  if (maximumDimension > 1.f)
	  {
		vec3 clippedColour = aabbCentre + (centreToSample / maximumDimension);
		clipped = true;
		return RGB(clippedColour);
	  }
	  else {return RGB(YCoCgSample);}
}



void main () {
	vec2 uvCurrent = gl_FragCoord.xy / textureSize(velocityTexture, 0).xy;
	vec2 invJitter = jitter * -0.5f;
	vec4 colourCURRENT  = texture(currentColorTexture, uvCurrent - invJitter);
	float depth = texture(currentDepthTexture, uvCurrent - invJitter).r;

	float z = depth * 2.0 - 1;
	vec4 projectedPos = vec4((uvCurrent) * 2.f - 1.f, z, 1.f);
	vec4 worldSpacePosition = inverseJitteredProjMatrix * projectedPos;
	worldSpacePosition /= worldSpacePosition.w;

	vec4 projectedPosHistory = prevProjectionMatrix * prevViewMatrix * worldSpacePosition;
	vec2 uvHistory = 0.5 * (projectedPosHistory.xy / projectedPosHistory.w) + 0.5;

	vec2 vel = uvHistory - uvCurrent;
  
	 vel += texture(velocityTexture, frontMostNeigbourCoord(uvCurrent - invJitter)).rg * pixelSize;
	uvHistory = uvCurrent + vel;

	vec4 colorHistory = texture(colorAntiAliased, vec2(uvHistory));

	vec3 colorHistoryClipped = clipNeighbourhood(colorHistory.rgb, uvCurrent);
	
	// How does this feedback work exactly
	if (colorHistory.a == 0.f) {FragColor.a = float(clipped);}
	else {FragColor.a = mix(colorHistory.a, float(clipped), feedback);}

	 float clipBlendFactor = FragColor.a;
	//Blend between history color and clipped color. Q: why not use feedback uniform directly
	vec3 colorHistoryClippedBlended = mix(colorHistory.rgb, colorHistoryClipped, clamp(clipBlendFactor, 0.f, 1.f));
	FragColor.rgb = mix(colorHistoryClippedBlended, colourCURRENT.rgb, feedback);
	DebugColor = colorHistory;

}