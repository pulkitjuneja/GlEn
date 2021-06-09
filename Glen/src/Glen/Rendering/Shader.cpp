#include "Shader.h"

Shader::Shader(const GLuint shaderProgram, const std::string& shaderName, int uniformCount) {
	this->shaderProgram = shaderProgram;
	this->shaderName = shaderName;

	GLint        size;
	GLenum       type;
	GLsizei      length;
	const GLuint bufSize = 64;
	GLchar       name[bufSize];

	for (int i = 0; i < uniformCount; i++)
	{
		glGetActiveUniform(shaderProgram, i, bufSize, &length, &size, &type, name);
		GLuint loc = glGetUniformLocation(shaderProgram, name);

		if (loc != GL_INVALID_INDEX)
			uniformLocations.insert(std::make_pair(name, loc));
	}

}

//void Shader::readFromFile(const std::string& fileName, char*& shaderContent) {
//	std::ifstream shaderFile(fileName);
//	std::stringstream buffer;
//	buffer << shaderFile.rdbuf();
//	shaderContent = new char[buffer.str().length()];
//	strcpy(shaderContent, &buffer.str()[0]);
//}

void Shader::use() {
	if (getShaderID() != currentShaderProgramInUse) {
		currentShaderProgramInUse = getShaderID();
		glUseProgram(shaderProgram);
	}
}

GLuint Shader::getShaderID()
{
	return shaderProgram;
}

void Shader::setBool(const std::string& name, bool value) const {
	GLuint loc = getUniformLocation(name);
	if (loc != -1) {
		glProgramUniform1i(shaderProgram, getUniformLocation(name), (int)value);
	}
}

void Shader::setInt(const std::string& name, int value) const {
	GLuint loc = getUniformLocation(name);
	if (loc != -1) {
		glProgramUniform1i(shaderProgram, getUniformLocation(name), value);
	}
}

void Shader::setFloat(const std::string& name, float value) const {
	GLuint loc = getUniformLocation(name);
	if (loc != -1) {
		glProgramUniform1f(shaderProgram, getUniformLocation(name), value);
	}
}

void Shader::setFloat3(const std::string& name, float value1, float value2, float value3) {
	GLuint loc = getUniformLocation(name);
	if (loc != -1) {
		glProgramUniform3f(shaderProgram, getUniformLocation(name), value1, value2, value3);
	}
}

void Shader::setMat4(const std::string& name, glm::mat4& matrix) {
	GLuint loc = getUniformLocation(name);
	if (loc != -1) {
		glProgramUniformMatrix4fv(shaderProgram, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}
}

void Shader::setUniformBlockBinding(const std::string& name, int index)
{
	unsigned int uniformBlockIndex = glGetUniformBlockIndex(shaderProgram, name.c_str());
	if (uniformBlockIndex != GL_INVALID_INDEX) {
		glUniformBlockBinding(shaderProgram, uniformBlockIndex, index);
	}
}

GLuint Shader::getUniformLocation(const std::string& name) const
{
	if (uniformLocations.find(name) == uniformLocations.end())
		return -1;
	else
		return uniformLocations.find(name)->second;
}

void ComputeShader::dispatch(int numGroupsX, int numGroupsY, int numGroupsZ)
{
	use();
	glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
}
