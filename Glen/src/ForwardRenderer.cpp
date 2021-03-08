#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer()
{
	perFrameUbo = new UniformBuffer(sizeof(PerFrameUniforms), 0);
}

void ForwardRenderer::render(Scene* scene)
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);

	//Update Global Uniform blocks
	sceneRenderer.setGlobalUniforms(perFrameUniforms, scene);

	perFrameUbo->bind();
	void* mem = perFrameUbo->mapToMemory(GL_WRITE_ONLY);

	if (mem) {
		memcpy(mem, &perFrameUniforms, sizeof(PerFrameUniforms));
		perFrameUbo->unmapFromMemroy();
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneRenderer.renderScene(scene);
}
