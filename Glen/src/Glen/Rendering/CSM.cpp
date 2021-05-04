#include "Csm.h"

void Csm::updateSplits(Camera* camera)
{
	float nd = camera->nearPlane;
	float fd = 400.0f;

	float ratio = fd / nd;
	splits[0].nearPlane = nd;
	splits[0].fov = camera->fov;
	splits[0].aspectRatio = camera->aspect;

	for (int i = 1; i < splitCount; i++)
	{
		splits[i].fov = camera->fov;
		splits[i].aspectRatio = camera->aspect;
		float si = i / (float)splitCount;

		// practical split scheme: https://developer.nvidia.com/gpugems/gpugems3/gpugems3_ch10.html
		float t_near = lambda * (nd * powf(ratio, si)) + (1 - lambda) * (nd + (fd - nd) * si);
		float t_far = t_near * 1.005f;
		splits[i].nearPlane = t_near;
		splits[i - 1].farPlane = t_far;
	}
	splits[splitCount - 1].farPlane = fd;

}

void Csm::updateFrustumCorners(Camera* camera)
{
	glm::vec3 center = camera->transform.getPosition();
	glm::vec3 view_dir = camera->front;

	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::cross(view_dir, up); // change this to use the camera right variable if this works

	for (int i = 0; i < splitCount; i++)
	{
		FrustumSplit& frustum = splits[i];

		glm::vec3 fc = center + view_dir * frustum.farPlane;
		glm::vec3 nc = center + view_dir * frustum.nearPlane;

		right = glm::normalize(right); // again this will be replaced 
		up = glm::normalize(glm::cross(right, view_dir));

		// these heights and widths are half the heights and widths of
		// the near and far plane rectangles

		float nearHeight = tan(frustum.fov / 2.0f) * frustum.nearPlane;
		float nearWidth = nearHeight * frustum.aspectRatio;
		float farHeight = tan(frustum.fov / 2.0f) * frustum.farPlane;
		float farWidth = farHeight * frustum.aspectRatio;

		// todo: remove duplicate frustum calculation from camera class
		frustum.corners[0] = nc - (up * nearHeight) - (right * nearWidth); // near-bottom-left
		frustum.corners[1] = nc + (up * nearHeight) - (right * nearWidth); // near-top-left
		frustum.corners[2] = nc + (up * nearHeight) + (right * nearWidth); // near-top-right
		frustum.corners[3] = nc - (up * nearHeight) + (right * nearWidth); // near-bottom-right

		frustum.corners[4] = fc - (up * farHeight) - (right * farWidth); // far-bottom-left
		frustum.corners[5] = fc + (up * farHeight) - (right * farWidth); // far-top-left
		frustum.corners[6] = fc + (up * farHeight) + (right * farWidth); // far-top-right
		frustum.corners[7] = fc - (up * farHeight) + (right * farWidth); // far-bottom-right
	}
}

void Csm::updateTextureMatrix(Camera* camera)
{
	for (int i = 0; i < splitCount; i++) {
		textureMatrices[i] = biasMatrix * cropMatrices[i];
	}
}

void Csm::updateCropMatrices(glm::mat4 lightModelView, Camera* camera, glm::vec3 lightDir)
{
	glm::mat4 projectionMat;
	for (int i = 0; i < splitCount; i++)
	{
		FrustumSplit& frustum = splits[i];

		glm::vec3 tmax(-INFINITY, -INFINITY, -INFINITY);
		glm::vec3 tmin(INFINITY, INFINITY, INFINITY);

		// find the z-range of the current frustum as seen from the light
		// in order to increase precision
		glm::vec4 transformedCorner = lightModelView * glm::vec4(frustum.corners[0], 1.0f);

		tmin.z = transformedCorner.z;
		tmax.z = transformedCorner.z;
		for (int j = 1; j < 8; j++)
		{
			transformedCorner = lightModelView * glm::vec4(frustum.corners[j], 1.0f);
			if (transformedCorner.z > tmax.z) { tmax.z = transformedCorner.z; }
			if (transformedCorner.z < tmin.z) { tmin.z = transformedCorner.z; }
		}


		// calculate frustum split center
		frustum.center = glm::vec3(0.0f, 0.0f, 0.0f);

		for (int j = 0; j < 8; j++)
			frustum.center += frustum.corners[j];

		frustum.center /= 8.0f;

		//implementing only unstable pssm for now 
		float radius = 0.0f;

		for (int j = 0; j < 8; j++)
		{
			float length = glm::length(frustum.corners[j] - frustum.center);
			radius = glm::max(radius, length);
		}

		radius = ceil(radius * 16.0f) / 16.0f;

		// Find bounding box that fits the sphere
		glm::vec3 radius3(radius, radius, radius);

		glm::vec3 max = radius3;
		glm::vec3 min = -radius3;

		glm::vec3 cascade_extents = max - min;

		// Push the light position back along the light direction by the near offset.
		glm::vec3 shadow_camera_pos = frustum.center - lightDir * -min.z;

		// Add the near offset to the Z value of the cascade extents to make sure the orthographic frustum captures the entire frustum split (else it will exhibit cut-off issues).
		glm::mat4 ortho = glm::ortho(min.x, max.x, min.y, max.y, -nearOffset, cascade_extents.z);
		glm::mat4 view = glm::lookAt(shadow_camera_pos, frustum.center, camera->cameraUP);

		projectionMatrices[i] = ortho;
		cropMatrices[i] = ortho * view;

		glm::vec4 shadow_origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shadow_origin = cropMatrices[i] * shadow_origin;
		shadow_origin = shadow_origin * (shadowMapSize / 2.0f);

		glm::vec4 rounded_origin = glm::round(shadow_origin);
		glm::vec4 round_offset = rounded_origin - shadow_origin;
		round_offset = round_offset * (2.0f / shadowMapSize);
		round_offset.z = 0.0f;
		round_offset.w = 0.0f;

		glm::mat4& shadow_proj = projectionMatrices[i];

		shadow_proj[3][0] += round_offset.x;
		shadow_proj[3][1] += round_offset.y;
		shadow_proj[3][2] += round_offset.z;
		shadow_proj[3][3] += round_offset.w;

		cropMatrices[i] = shadow_proj * view;
		//glm::mat4 ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -nearOffset, -tmin.z);
		//glm::mat4 shadowMvp = ortho * lightModelView;

		//// TODO: see if you can combine this with the above loop (this is in clip space)
		//for (int j = 1; j < 8; j++)
		//{
		   // transformedCorner = shadowMvp * glm::vec4(frustum.corners[j], 1.0f);
		   // transformedCorner.x /= transformedCorner.w;
		   // transformedCorner.y /= transformedCorner.w;
		//    if (transformedCorner.x > tmax.x) { tmax.x = transformedCorner.x; }
		   // if (transformedCorner.x < tmin.x) { tmin.x = transformedCorner.x; }
		   // if (transformedCorner.y > tmax.y) { tmax.y = transformedCorner.y; }
		   // if (transformedCorner.y < tmin.y) { tmin.y = transformedCorner.y; }
		//}

		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		   // cout << tmax.x << " , " << tmax.y << " , " << tmax.z << "\n";
		   // cout << tmin.x << " , " << tmin.y << " , " << tmin.z << "\n";
		   // cout << "\n\n\n\n\n";
		//}

		//glm::vec2 tscale(2.0f / (tmax.x - tmin.x), 2.0f / (tmax.y - tmin.y));
		//glm::vec2 toffset(-0.5f * (tmax.x + tmin.x) * tscale.x, -0.5f * (tmax.y + tmin.y) * tscale.y);

		//glm::mat4 shadowCropMatrix = glm::mat4(1.0f);
		//shadowCropMatrix[0][0] = tscale.x;
		//shadowCropMatrix[1][1] = tscale.y;
		//shadowCropMatrix[0][3] = toffset.x;
		//shadowCropMatrix[1][3] = toffset.y;
		//shadowCropMatrix = glm::transpose(shadowCropMatrix);

		//glm::mat4 finalProjectionMatrix = shadowCropMatrix * ortho;
		//// Store the projection matrix
		//projectionMatrices[i] = finalProjectionMatrix;
		//cropMatrices[i] = finalProjectionMatrix * lightModelView;
	}
}

void Csm::updateFarBounds(Camera* camera)
{
	for (int i = 0; i < splitCount; i++) {
		auto split = splits[i];
		glm::vec4 pos = camera->getProjectionMatrix() * glm::vec4(0.0f, 0.0f, -split.farPlane, 1.0f);
		glm::vec4 ndcPos = pos / pos.w;
		farBounds[i] = ndcPos.z * 0.5f + 0.5f;
	}
}

void Csm::updateUniforms(CSMUniforms& csmUniforms)
{
	for (int i = 0; i < splitCount; i++) {
		csmUniforms.textureMatrices[i] = textureMatrices[i];
		csmUniforms.farBounds[i].farBound = farBounds[i];
	}
	csmUniforms.splitCount = splitCount;
}

void Csm::render(SceneManager* scene)
{
	for (int i = 0; i < splitCount; i++) {
		depthMapMaterial.getShader()->setMat4("lightSpaceMatrix", cropMatrices[i]);
		shadowFbos[i].bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, shadowMapSize, shadowMapSize);
		sceneRenderer.renderScene(scene, &depthMapMaterial);
	}
	shadowFbos[splitCount - 1].unBind();
}


Csm::Csm(float lambda, float nearOffset, int splitCount, int shadowMapSize) : lambda(lambda), nearOffset(nearOffset), splitCount(splitCount), shadowMapSize(shadowMapSize)
{
	shadowMaps = EngineContext::get()->resourceManager->generateTexture(CSM_SHADOW_MAPS, TextureType::DEPTH, shadowMapSize, shadowMapSize,
		GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, splitCount);
	shadowMaps->bind();
	shadowMaps->bind(GL_TEXTURE0 + 10);
	shadowMaps->setMinMagFilter(GL_LINEAR, GL_LINEAR);
	shadowMaps->setWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);

	for (int i = 0; i < splitCount; i++) {
		shadowFbos[i].bind();
		shadowFbos[i].attachDepthTarget(shadowMaps, 0, i, true);
	}
	biasMatrix = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);
	depthMapMaterial.setShader(EngineContext::get()->resourceManager->getShader("depthMap"));
}

void Csm::update(Camera* camera, glm::vec3 lightDir)
{
	lightDir = glm::normalize(lightDir);
	// todo: try computing this for each frustum seperately if things dont work out 
	glm::vec3 center = camera->transform.getPosition() + (camera->front * 10.0f);
	glm::vec3 lightPosition = center - lightDir * ((camera->farPlane - camera->nearPlane) / 2.0f);
	glm::mat4 lightView = glm::lookAt(lightPosition, center, camera->right);
	updateSplits(camera);
	updateFrustumCorners(camera);
	updateCropMatrices(lightView, camera, lightDir);
	updateTextureMatrix(camera);
	updateFarBounds(camera);
}	