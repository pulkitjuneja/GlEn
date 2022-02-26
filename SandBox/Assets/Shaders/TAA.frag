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
	mat4 VPPrevNoJitter;
	mat4 VPPrevJittered;
	mat4 VPCurrentJittered;
	mat4 VPCurrentJitteredInverse;
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

vec3 tonemap(vec3 x)
{
	return x / (x + 1); // Reinhard tonemap
}
vec3 inverseTonemap(vec3 x)
{
	return x / (1 - x);
}

//bool clipped = true;

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

//vec3 clipNeighbourhood(vec3 colourSample, vec2 uvCURRENT){
//	vec3 colorMin = mix(minSample3x3(uvCURRENT), minSampleDirectNeighbours(uvCURRENT), 0.5f);
//	vec3 colorMax = mix(maxSample3x3(uvCURRENT), maxSampleDirectNeighbours(uvCURRENT), 0.5f);
//
//	vec3 YCoCgSample = YCoCg(colourSample.rgb);
//
//	 vec3 aabbCentre = 0.5f * (colorMax + colorMin);
//	 vec3 aabbWidth = 0.5f * (colorMax - colorMin);
//	 vec3 centreToSample = YCoCgSample - aabbCentre;
//	 vec3 ctsUnitSpace = centreToSample / aabbWidth;
//	 float maximumDimension = max(abs(ctsUnitSpace.x), max(abs(ctsUnitSpace.y), abs(ctsUnitSpace.z)));
//	  if (maximumDimension > 1.f)
//	  {
//		vec3 clippedColour = aabbCentre + (centreToSample / maximumDimension);
//		clipped = true;
//		return RGB(clippedColour);
//	  }
//	  else {return RGB(YCoCgSample);}
//}

vec3 clip_aabb(vec3 aabb_min, vec3 aabb_max, vec3 p, vec3 q) {
		vec3 r = q - p;
		vec3 rmax = aabb_max - p.xyz;
		vec3 rmin = aabb_min - p.xyz;

		const float eps = 0.00001;

		if (r.x > rmax.x + eps)
			r *= (rmax.x / r.x);
		if (r.y > rmax.y + eps)
			r *= (rmax.y / r.y);
		if (r.z > rmax.z + eps)
			r *= (rmax.z / r.z);

		if (r.x < rmin.x - eps)
			r *= (rmin.x / r.x);
		if (r.y < rmin.y - eps)
			r *= (rmin.y / r.y);
		if (r.z < rmin.z - eps)
			r *= (rmin.z / r.z);

		return p + r;
}

float mitchell(float x) {
    float B = 1.0 / 3.0;
    float C = 1.0 / 3.0;

    float ax = abs(x);
    if (ax < 1) {
        return ((12 - 9 * B - 6 * C) * ax * ax * ax + (-18 + 12 * B + 6 * C) * ax * ax + (6 - 2 * B)) / 6;
    } else if ((ax >= 1) && (ax < 2)) {
        return ((-B - 6 * C) * ax * ax * ax + (6 * B + 30 * C) * ax * ax + (-12 * B - 48 * C) * ax + (8 * B + 24 * C)) / 6;
    } else {
        return 0;
    }
}

float Luminance(vec3 source) {
	vec3 lum = vec3(0.2127, 0.7152, 0.0722);
	return dot(lum, source);
}

void main () {
	vec2 uvCurrent = gl_FragCoord.xy / textureSize(velocityTexture, 0).xy;
	vec3 result;

	vec3 sourceSampleTotal = vec3(0, 0, 0);
	float sourceSampleWeight = 0.0;
	vec3 neighborhoodMin = vec3(10000);
	vec3 neighborhoodMax = vec3(-10000);
	vec3 m1 = vec3(0, 0, 0);
	vec3 m2 = vec3(0, 0, 0);
	float closestDepth = 0.0;
	vec2 closestDepthPixelPosition = vec2(0, 0);

	// Sample neighbours using a 9-tap filter
	for (int x = -1; x <= 1; x++)
	{
	 for (int y = -1; y <= 1; y++)
		 {
			vec2 location = uvCurrent + vec2(x,y) * pixelSize;
			vec3 neighbour = max(vec3(0,0,0),texture(currentColorTexture, location).rgb);
			float subSampleDistance = length(vec2(x, y));
			float subSampleWeight = mitchell(subSampleDistance);

			sourceSampleTotal += neighbour * subSampleWeight;
			sourceSampleWeight += subSampleWeight;

			neighborhoodMin = min(neighborhoodMin, neighbour);
			neighborhoodMax = max(neighborhoodMax, neighbour);

			m1 += neighbour;
			m2 += neighbour * neighbour;

			float currentDepth = texture(currentDepthTexture, location).r;
			if (currentDepth < closestDepth)
			{
				closestDepth = currentDepth;
				closestDepthPixelPosition = vec2(x,y);
			}
		 }
	}

	vec2 motionVector = texture(velocityTexture, uvCurrent + closestDepthPixelPosition* pixelSize).rg;
	vec2 historyCood = uvCurrent + motionVector;
	vec3 sourceSample = sourceSampleTotal / sourceSampleWeight;
	vec3 historySample;

	if(historyCood.x < 0 || historyCood.x > 1 || historyCood.y < 0 || historyCood.y > 1)
	{
		FragColor = vec4(sourceSample,1);
		return;
	}
	else {
		historySample = texture(colorAntiAliased,historyCood).rgb;
		//result = mix(sourceSample, historySample, 0.75);
	}

	float oneDividedBySampleCount = 1.0 / 9.0;
	float gamma = 1.0;
	vec3 mu = m1 * oneDividedBySampleCount;
	vec3 sigma = sqrt(abs((m2 * oneDividedBySampleCount) - (mu * mu)));
	vec3 minc = mu - gamma * sigma;
	vec3 maxc = mu + gamma * sigma;

	historySample = clip_aabb(minc, maxc, clamp(historySample, neighborhoodMin, neighborhoodMax), historySample);
	float sourceWeight = 0.05;
	float historyWeight = 1.0 - sourceWeight;

	vec3 compressedSource = sourceSample * 1/(max(max(sourceSample.r, sourceSample.g), sourceSample.b) + 1.0);
	vec3 compressedHistory = historySample * 1/(max(max(historySample.r, historySample.g), historySample.b) + 1.0);
	float luminanceSource = Luminance(compressedSource);
	float luminanceHistory = Luminance(compressedHistory); 

	sourceWeight *= 1.0 / (1.0 + luminanceSource);
	historyWeight *= 1.0 / (1.0 + luminanceHistory);
 
	result = (compressedSource * sourceWeight + compressedHistory * historyWeight) / max(sourceWeight + historyWeight, 0.00001);
//
//	float belndWeight = 0.8;
//	result = mix(sourceSample, historySample, vec3(belndWeight));

	FragColor = vec4(result, 1);
}

//void main () {
//	vec2 texCoordOffset = pixelSize.xy;
//	vec2 uvCurrent = gl_FragCoord.xy / textureSize(velocityTexture, 0).xy;
//
//	float R_fxaaReduceMin = 1.0/128.0;
//	float R_fxaaReduceMul = 1.0/8.0;
//	
//	vec3 luma = vec3(0.299, 0.587, 0.114);	
//	float lumaTL = dot(luma, texture2D(currentColorTexture, uvCurrent + (vec2(-1.0, -1.0) * texCoordOffset)).xyz);
//	float lumaTR = dot(luma, texture2D(currentColorTexture, uvCurrent + (vec2(1.0, -1.0) * texCoordOffset)).xyz);
//	float lumaBL = dot(luma, texture2D(currentColorTexture, uvCurrent + (vec2(-1.0, 1.0) * texCoordOffset)).xyz);
//	float lumaBR = dot(luma, texture2D(currentColorTexture, uvCurrent + (vec2(1.0, 1.0) * texCoordOffset)).xyz);
//	float lumaM  = dot(luma, texture2D(currentColorTexture, uvCurrent).xyz);
//
//	vec2 dir;
//	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
//	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));
//	
//	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (R_fxaaReduceMul * 0.25), R_fxaaReduceMin);
//	float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
//	
//	dir = min(vec2(8.0f, 8.0f), 
//		max(vec2(-8.0f, -8.0f), dir * inverseDirAdjustment)) * texCoordOffset;
//
//	vec3 result1 = (1.0/2.0) * (
//		texture2D(currentColorTexture,uvCurrent + (dir * vec2(1.0/3.0 - 0.5))).xyz +
//		texture2D(currentColorTexture,uvCurrent + (dir * vec2(2.0/3.0 - 0.5))).xyz);
//
//	vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
//		texture2D(currentColorTexture,uvCurrent + (dir * vec2(0.0/3.0 - 0.5))).xyz +
//		texture2D(currentColorTexture,uvCurrent + (dir * vec2(3.0/3.0 - 0.5))).xyz);
//
//	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
//	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
//	float lumaResult2 = dot(luma, result2);
//	
//	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
//		FragColor = vec4(result1, 1.0);
//	else
//		FragColor = vec4(result2, 1.0);
//}

//void main () {
//	vec2 uvCurrent = gl_FragCoord.xy / textureSize(velocityTexture, 0).xy;
//	vec3 neighborhoodMin = vec3(100000);
//	vec3 neighborhoodMax = vec3(-100000);
//	vec3 current = texture(currentColorTexture, uvCurrent).rgb;
//	float bestDepth = 1;
//
//	vec2 bestPixel = vec2(0, 0);
//	for (int x = -1; x <= 1; ++x)
//	{
//		for (int y = -1; y <= 1; ++y)
//		{
//			vec2 curPixel = uvCurrent + vec2(x,y) * pixelSize;
//
//			vec3 neighbor = texture(currentColorTexture, curPixel - jitter).rgb;
//			neighborhoodMin = min(neighborhoodMin, neighbor);
//			neighborhoodMax = max(neighborhoodMax, neighbor);
//
//			float depth = texture(currentDepthTexture, curPixel).r;
//			if (depth < bestDepth)
//			{
//				bestDepth = depth;
//				bestPixel = curPixel;
//			}
//		}
//	}
//
//	vec2 velocity = texture(velocityTexture, bestPixel).rg;
//	vec2 prevUV = uvCurrent + velocity;
//	vec3 historySample = textureLod(colorAntiAliased,prevUV - jitter, 0).rgb;
//	historySample = clamp(historySample, neighborhoodMin, neighborhoodMax);
//
//	float subpixelCorrection = fract(max(abs(velocity.x) * 1920.0f, abs(velocity.y) * 1080.0f)) * 0.5f;
//	float bf = clamp(mix(0.05f, 0.8f, subpixelCorrection),0,1);
//
//	if(prevUV.x < 0 || prevUV.x > 1 || prevUV.y < 0 || prevUV.y > 1) {
//		bf = 1.0f;
//	}
//
//	current = tonemap(current);
//	historySample = tonemap(historySample);
//
//	vec3 result = (historySample + current) / 2;
////	result = inverseTonemap(result);
//
//	FragColor = vec4(result, 1.0f);
//}
