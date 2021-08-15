
#include "Texture.h"
#include "Glen/Core/EngineContext.h"
#include "Glen/ResourceManagement/ResourceManager.h"
#include "Glen/Rendering/FrameBuffer.h"

// TODO: Add support for mip map levels 
Texture2D::Texture2D(TextureType type, uint32_t w, uint32_t h, GLenum format, GLenum dataType, GLenum internalFormat) : Texture (type, w, h) {

	this->format = format;
	this->internalFormat = internalFormat;
	this->dataType = dataType;

	target = GL_TEXTURE_2D;
	glBindTexture(target, textureId);
	glTexImage2D(target, 0, internalFormat, w, h, 0, format, dataType, NULL);
	glBindTexture(target, 0);
}

//Texture::~Texture() {
//	std::cout << "Destructor called";
//	glDeleteTextures(1, &textureId);
//}

void Texture2D::setData(void* data, bool bind) {
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexImage2D(target, 0, internalFormat, w, h, 0, format, dataType, data);

	if (bind) {
		Unbind();
	}
}



void Texture2D::setWrapping(GLenum s, GLenum t, GLenum r, bool bind) {
	if (bind) {
		glBindTexture(target, textureId);
	}
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	if (bind) {
		Unbind();
	}
}

CubeMap::CubeMap(uint32_t w, uint32_t h) : Texture(TextureType::CUBEMAP, w, h){
	target = GL_TEXTURE_CUBE_MAP;
}

void CubeMap::setFaceData(int faceIndex, void* data, GLenum format,
	GLenum internalFormat, GLenum dataType, bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, 0, internalFormat, w, h, 0, format, dataType, data);

	if (bind) {
		Unbind();
	}
}

void CubeMap::setWrapping(GLenum s, GLenum t, GLenum r, bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, r);

	if (bind) {
		Unbind();
	}
}

void CubeMap::setDataFromHDRIMap(Texture2D* hdriMap)
{
	bind();

	for (int i = 0; i < 6; i++) {
		setFaceData(i, nullptr, GL_RGB, GL_RGB16F, GL_FLOAT);
	}
	setWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	setMinMagFilter(GL_LINEAR, GL_LINEAR);

	Shader* EqToCm = EngineContext::get()->resourceManager->getShader("EqToCm");
	EqToCm->setInt("equirectangularMap", 0);
	hdriMap->bind();
	renderCubeToFrameBuffer(EqToCm, w, h);
	hdriMap->Unbind();

	Unbind();
}

CubeMap* CubeMap::createConvolutionMap(const std::string& identifier)
{
	CubeMap* convolutionMap = EngineContext::get()->resourceManager->generateCubeMap(identifier, 32, 32);
	convolutionMap->bind();
	for (int i = 0; i < 6; i++) {
		convolutionMap->setFaceData(i, nullptr, GL_RGB, GL_RGB16F, GL_FLOAT);
	}
	convolutionMap->setWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	convolutionMap->setMinMagFilter(GL_LINEAR, GL_LINEAR);

	Shader* irradianceGenerator = EngineContext::get()->resourceManager->getShader("IrradianceGenerator");
	irradianceGenerator->setInt("environmentMap", 0);
	bind(GL_TEXTURE0);
	convolutionMap->renderCubeToFrameBuffer(irradianceGenerator, 32, 32);
	Unbind();

	return convolutionMap;
}

void CubeMap::renderCubeToFrameBuffer(Shader* shader, uint32_t w, uint32_t h)
{
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	shader->setMat4("projection", captureProjection);

	FrameBuffer captureFBO;
	captureFBO.bind();
	captureFBO.attachRenderBuffer(GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT, w, h);
	captureFBO.unBind();

	shader->use();

	glViewport(0, 0, w, h);
	captureFBO.bind();
	Mesh* cubeMesh = EngineContext::get()->resourceManager->getMesh("CUBE");

	for (int i = 0; i < 6; i++) {
		shader->setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureId, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(cubeMesh->VAO);
		SubMesh submesh = cubeMesh->subMeshes[0];
		glDrawElementsBaseVertex(GL_TRIANGLES, submesh.indexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * submesh.baseIndex), submesh.baseVertex);
	}

	std::cout << glGetError() << std::endl;
	captureFBO.unBind();
}

Texture::Texture(TextureType type, uint32_t w, uint32_t h)
{
	glGenTextures(1, &textureId);

	this->type = type;
	this->w = w;
	this->h = h;
}

void Texture::bind(int textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(target, textureId);
}

void Texture::Unbind()
{
	glBindTexture(target, 0);
}

void Texture::generateMipMaps(bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glGenerateMipmap(target);

	if (bind) {
		Unbind();
	}
}

void Texture::setMinMagFilter(GLenum minFilter, GLenum magFilter, bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

	if (bind) {
		Unbind();
	}
}

Texture3D::Texture3D(TextureType type, int arraySize, uint32_t w, uint32_t h, GLenum format, GLenum dataType, GLenum internalFormat) : Texture(type, w, h)
{
	this->format = format;
	this->internalFormat = internalFormat;
	this->dataType = dataType;
	this->arraySize = arraySize;

	target = GL_TEXTURE_2D_ARRAY;
	glBindTexture(target, textureId);
	glTexImage3D(target, 0, internalFormat, w, h, arraySize, 0, format, dataType, NULL);
}

void Texture3D::setData(void* data, int arrayIndex, bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexImage3D(target, 0, internalFormat, w, h, arrayIndex, 0, format, dataType, data);

	if (bind) {
		Unbind();
	}
}

void Texture3D::setWrapping(GLenum s, GLenum t, GLenum r, bool bind)
{
	if (bind) {
		glBindTexture(target, textureId);
	}

	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	if (bind) {
		Unbind();
	}
}
