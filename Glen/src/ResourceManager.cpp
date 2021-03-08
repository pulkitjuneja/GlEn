#include "Globals.h"
#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

ResourceManager* ResourceManager::instance;

void ResourceManager::readFromFile(const std::string& fileName, char*& shaderContent)
{
	string fullPath = fileName;
	ifstream shaderFile(fullPath);
	stringstream buffer;
	buffer << shaderFile.rdbuf();
	shaderContent = new char[buffer.str().length()];
	strcpy(shaderContent, &buffer.str()[0]);
}

ResourceManager::ResourceManager() {

}

ResourceManager* ResourceManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ResourceManager();
	}
	return instance;
}

void ResourceManager::loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& shaderName)
{

	if (loadedShaders.find(shaderName) != loadedShaders.end())
	{
		cout << "Shader already loaded, to get the shader use the getShader function";
		return;
	}
	unsigned int vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	char* vertexShaderSource;
	readFromFile(vertexShaderPath, vertexShaderSource);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR " << shaderName << "::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	char* fragmentShaderSource;
	readFromFile(fragmentShaderPath, fragmentShaderSource);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR " << shaderName << "::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR" << shaderName << "::PROGRAM::LINKING_FAILED\n"
			<< infoLog << std::endl;
	}

	int uniformCount = 0;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	Shader* newShader = new Shader(shaderProgram, shaderName, uniformCount);
	newShader->setUniformBlockBinding("perFrameUniforms", 0);

	cout << "Shader loaded : " << shaderName << " id : " << std::to_string(newShader->getShaderID()) << "\n";
	loadedShaders.insert(make_pair(shaderName, newShader));
}


Shader* ResourceManager::getShader(const string& shaderName)
{
	if (loadedShaders.find(shaderName) != loadedShaders.end())
	{
		return loadedShaders.find(shaderName)->second;
	}
	else
	{
		cout << "Shader " << shaderName << " is not loaded";
		return nullptr;
	}
}

Texture* ResourceManager::loadTexture(const string& texturePath, const string& directory, TextureType type)
{

	string filename = string(texturePath);
	filename = directory + '/' + filename;

	if (textures.find(texturePath) != textures.end())
	{
		return textures.find(texturePath)->second;
	}

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	Texture* tex;

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		tex = new Texture(type, 1, width, height, format, GL_UNSIGNED_BYTE, format);
		tex->bind();
		tex->setData(data, 0);
		tex->generateMipMaps();
		tex->setWrapping(GL_REPEAT, GL_REPEAT);
		tex->setMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		tex->Unbind();
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	textures.emplace(make_pair(texturePath, tex));
	std::cout << "Texture Loaded: " << texturePath << std::endl;
	return textures.find(texturePath)->second;
}

Texture* ResourceManager::getTexture(const string& textureName)
{
	if (textures.find(textureName) != textures.end())
	{
		return textures.find(textureName)->second;
	}
	else
	{
		cout << "Shader " << textureName << " does not exist";
		return nullptr;
	}
}
