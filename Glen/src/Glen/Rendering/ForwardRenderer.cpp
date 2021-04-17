#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer()
{
	csm = new Csm(0.3, 350.0f, 4, 4096);
	perFrameUbo = new UniformBuffer(sizeof(PerFrameUniforms), 0);
	CsmUbo = new UniformBuffer(sizeof(CSMUniforms), 1);
	debugDraw = new DebugDraw();
}

void ForwardRenderer::render(Scene* scene)
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);

	csm->update(scene->getMainCamera(), scene->getDirectionalLight()->direction);
	csm->updateUniforms(csmUniforms);

	//Update Global Uniform blocks
	sceneRenderer.setGlobalUniforms(perFrameUniforms, scene);

	perFrameUbo->bind();
	void* mem = perFrameUbo->mapToMemory(GL_WRITE_ONLY);

	if (mem) {
		memcpy(mem, &perFrameUniforms, sizeof(PerFrameUniforms));
		perFrameUbo->unmapFromMemroy();
	}

	CsmUbo->bind();
	auto siz = sizeof(int);
	void* mem2 = CsmUbo->mapToMemory(GL_WRITE_ONLY);
	if (mem2) {
		memcpy(mem2, &csmUniforms, sizeof(CSMUniforms));
		CsmUbo->unmapFromMemroy();
	}

	csm->render(scene);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneRenderer.renderScene(scene);

	// render debug Meshes
	debugDraw->render(scene);
}
