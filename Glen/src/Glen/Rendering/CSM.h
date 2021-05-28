#pragma once

#ifndef CSM_H
#define CSM_H

#include <GL/glew.h>

#include "Texture.h"
#include "FrameBuffer.h"
#include "Glen/SCene/Camera.h"
#include "Glen/ResourceManagement/ResourceManager.h"
#include "SceneRenderer.h"
#include "Glen/Scene/Scene.h"
#include "Glen/Core/EngineContext.h"

struct FrustumSplit
{
    float nearPlane;
    float farPlane;
    float aspectRatio;
    float fov;
    float lambda;
    glm::vec3 center;
    glm::vec3 corners[8];
};

class Csm {
    Texture3D* shadowMaps;
    FrameBuffer shadowFbos[CSM_MAX_SPLIT_COUNT];
    float lambda;
    float nearOffset;
    int   splitCount;
    int   shadowMapSize;
    SceneRenderer sceneRenderer;
    Material depthMapMaterial;
    glm::mat4 biasMatrix;
    float farBounds[CSM_MAX_SPLIT_COUNT];
    FrustumSplit splits[CSM_MAX_SPLIT_COUNT];
    glm::mat4 cropMatrices[CSM_MAX_SPLIT_COUNT]; // crop * proj * view
    glm::mat4 textureMatrices[CSM_MAX_SPLIT_COUNT];
    glm::mat4 projectionMatrices[CSM_MAX_SPLIT_COUNT]; // crop * proj * light_view * inv_view

public:
    Csm(float lambda, float nearOffset, int splitCount, int shadowMapSize);
    void update(Camera* camera, glm::vec3 lightDir);
    void updateSplits(Camera* camera);
    void updateFrustumCorners(Camera* camera);
    void updateTextureMatrix(Camera* camera);
    void updateCropMatrices(glm::mat4 lightModelView, Camera* camera, glm::vec3 lightDir);
    void updateFarBounds(Camera* camera);

    void updateUniforms(CSMUniforms& csmUniforms);
    void render(SceneManager* scene);
};


#endif