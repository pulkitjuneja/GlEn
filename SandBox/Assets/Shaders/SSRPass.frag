#version 330 core


uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;

uniform sampler2D finalImageBuffer;


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
	mat4 lightSpaceMatrix;
	DirectionalLight directionalLight;
	PointLight pointLights[10];
	vec4 cameraPosition;
	int pointLightCount;
};

out vec4 fragcolor;

void main () {
  float maxdistance = 150;
  float resolution  = 0.8;
  int   steps       = 5;
  float thickness   = 0.5;

   vec2 texsize  = textureSize(positionTexture, 0).xy;
   vec2 texcoord = gl_FragCoord.xy / texsize; // this works	
	
   vec4 uv = vec4(0.0);

   vec3 worldpos = texture(positionTexture, texcoord).xyz;
   vec3 worldnormal = texture(normalTexture, texcoord).xyz;

   // dirty hack to only make the flat surfaces metallic
   if(worldnormal.y < 0.8) {
	discard;
   }

   vec3 viewpos = (viewMatrix * vec4(worldpos, 1.0)).xyz;
   vec3 viewnormal = mat3(viewMatrix) * worldnormal;

   vec3 reflection = normalize(reflect(normalize(viewpos), normalize(viewnormal)));

   vec3 positionto = viewpos;

  vec4 startview = vec4(viewpos + (reflection * 0.0), 1.0);
  vec4 endview   = vec4(viewpos + (reflection * maxdistance), 1.0);

  vec4 startfrag = projectionMatrix * startview;
  startfrag.xyz /= startfrag.w;
  startfrag.xy   = startfrag.xy * 0.5 + 0.5; // converting to uv coordinates 
  startfrag.xy  *= texsize;

  vec4 endfrag = projectionMatrix * endview;
  endfrag.xyz /= endfrag.w;
  endfrag.xy = endfrag.xy * 0.5 + 0.5;
  endfrag.xy *= texsize;

  float deltax    = endfrag.x - startfrag.x;
  float deltay    = endfrag.y - startfrag.y;

  float usex      = abs(deltax) >= abs(deltay) ? 1.0 : 0.0;
  float delta     = mix(abs(deltay), abs(deltax), usex) * clamp(resolution, 0.0, 1.0);
  vec2  increment = vec2(deltax, deltay) / max(delta, 0.001);

  float search0 = 0;
  float search1 = 0;

  int hit0 = 0;
  int hit1 = 0;

  float viewdistance = startview.z;
  float depthdifference = thickness;

  vec2 frag  = startfrag.xy;

  for (float i = 0; i < int(delta); ++i) {
	frag      += increment;
    uv.xy      = frag / texsize;
	// todo: optimize this step, too many calculations
    positionto = (viewMatrix * vec4(texture(positionTexture, uv.xy).xyz, 1.0)).xyz;

	search1 =
      mix
        ( (frag.y - startfrag.y) / max(deltay,0.001)
        , (frag.x - startfrag.x) / max(deltax, 0.001)
        , usex
        );

	search1 = clamp(search1, 0.0, 1.0);

	// perspective corect interpolation for z depth https://www.comp.nus.edu.sg/~lowkl/publications/lowk_persp_interp_techrep.pdf
	viewdistance = (startview.z * endview.z) / mix(endview.z, startview.z, search1);
	depthdifference = positionto.z - viewdistance;

	if (depthdifference > 0 && depthdifference < thickness) {
      hit0 = 1;
      break;
    } else {
      search0 = search1;
    }
  }

  // proceed to the binary search
   steps *= hit0;
   search1 = search0 + ((search1 - search0) / 2.0);

  for (float i = 0; i < steps; ++i) {
	frag = mix(startfrag.xy, endfrag.xy, search1);
	uv.xy = frag / texsize;
	positionto = (viewMatrix * vec4(texture(positionTexture, uv.xy).xyz, 1.0)).xyz;

	viewdistance = (startview.z * endview.z) / mix(endview.z, startview.z, search1);
    depthdifference = viewdistance - positionto.z;

	if (depthdifference > 0 && depthdifference < thickness) {
      hit1 = 1;
      search1 = search0 + ((search1 - search0) / 2);
    } else {    
      float temp = search1;
      search1 = search1 + ((search1 - search0) / 2);
      search0 = temp;
    }

  }

   float visibility =
      hit1
    * ( 1
      - max
         ( dot(-normalize(viewpos), reflection)
         , 0
         )
      )
	* (uv.x < 0 || uv.x > 1 ? 0 : 1)
    * (uv.y < 0 || uv.y > 1 ? 0 : 1)    
    * ( 1 - clamp(length(positionto - viewpos) / maxdistance, 0, 1));

   if (visibility <= 0.0) { fragcolor = vec4(0.0); return;}

  vec4  color = texture(finalImageBuffer, uv.xy);
  float alpha = clamp(visibility, 0.0, 1.0);

  fragcolor = vec4(mix(vec3(0.0), color.rgb, alpha), alpha);
}


// View space raytracing approach

//uniform float ssr_minRayStep = 0.5;
//uniform float ssr_step = 0.005; // 0.025
//uniform int ssr_rayMarchCount = 200; // 60
//uniform int ssr_binarySearchCount = 10;
//
//vec2 binarySearch (inout vec3 dir, inout vec3 hitCoord, inout float dDepth) {
//	float depth;
//
//    vec4 projectedCoord;
//
//	for(int i = 0; i < ssr_binarySearchCount; i++) {
//	    projectedCoord = projectionMatrix * vec4(hitCoord, 1.0);
//        projectedCoord.xy /= projectedCoord.w;
//        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
// 
//        depth = (viewMatrix * vec4(texture(positionTexture, projectedCoord.xy).xyz, 1.0)).z;
// 
//        dDepth = hitCoord.z - depth;
//
//		dir *= 0.5;
//        if(dDepth > 0.0)
//            hitCoord += dir;
//        else
//            hitCoord -= dir;    
//	}
//
//	projectedCoord = projectionMatrix * vec4(hitCoord, 1.0);
//    projectedCoord.xy /= projectedCoord.w;
//    projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
// 
//    return vec2(projectedCoord.xy);
//}
//
//vec3 fresnelSchlick(float cosTheta, vec3 F0)
//{
//    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
//}
//
//vec2 rayCast(vec3 dir, inout vec3 hitCoord, out float dDepth) {
//	dir *= ssr_step;
//	for (int i = 0; i < ssr_rayMarchCount; i++) {
//		hitCoord += dir;
//		vec4 projectedCoord = projectionMatrix * vec4(hitCoord, 1.0);
//        projectedCoord.xy /= projectedCoord.w;
//        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5; 
//
//		float depth = (viewMatrix * vec4(texture(positionTexture, projectedCoord.xy).xyz, 1.0)).z;
//		dDepth = hitCoord.z - depth;
//
//		if((dir.z - dDepth) < 0.6 && dDepth <= 0.0) return binarySearch(dir, hitCoord, dDepth);
//	}
//	return vec2(-1.0);
//}
//
//void main () {
//	float reflectionStrength = 0.7f;
//
//	vec2 texSize  = textureSize(positionTexture, 0).xy;
//    vec2 texCoord = gl_FragCoord.xy / texSize; // this works	
//
//   vec3 worldPos = texture(positionTexture, texCoord).xyz;
//   vec3 worldNormal = texture(normalTexture, texCoord).xyz;
//       
//   // dirty hack to only make the flat surfaces metallic
//   if(worldNormal.y < 0.8) {
//	discard;
//   }
//
//   vec3 viewPos = (viewMatrix * vec4(worldPos, 1.0)).xyz;
//   vec3 viewNormal = mat3(viewMatrix) * worldNormal;
//
//    vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));
//
//	vec3 hitPos = viewPos;
//	float dDepth; 
//
//	// ray lenght is proportional to the distance of the fragment. why ?
//	float rayLength = max(-viewPos.z, ssr_minRayStep);
//	vec2 coords = rayCast(reflected * rayLength, hitPos, dDepth);
//
//	vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - coords.xy));
//
//	float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);
//	
//	vec3 F0 = texture(finalImageBuffer, texCoord).rgb;
//
//	vec3 Fresnel = fresnelSchlick(max(dot(normalize(viewNormal), normalize(viewPos)), 0.0), F0);
//
//	float hit = coords.x>0 ?1 : 0;
//
//	 float visibility =
//      hit
//    * ( 1
//      - max
//         ( dot(-normalize(viewPos), reflected)
//         , 0
//         )
//      );
//
//
//	if (visibility <= 0.0) { fragcolor = vec4(0.0); return;}
//	vec3  color = texture(finalImageBuffer, coords.xy).rgb * clamp(screenEdgefactor, 0.0, 0.9);
//
//	float alpha = clamp(visibility, 0.0, 1.0);
//
//	fragcolor = vec4(mix(vec3(0.0), color.rgb, alpha), alpha);
//}
//