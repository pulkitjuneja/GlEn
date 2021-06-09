
#pragma once
#ifndef Shader_h
#define Shader_h

#include <GL/glew.h>
#include "Glen/Globals.h"
#include <unordered_map>

static int currentShaderProgramInUse;

class GLN_API Shader {
    GLuint shaderProgram;
    std::unordered_map<std::string, GLuint> uniformLocations;
public:
    std::string shaderName;
    Shader(const GLuint shaderProgram, const std::string& shaderName, int uniformCount);
    void use();
    GLuint getShaderID();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloat3(const std::string& name, float value1, float value2, float value3);
    void setMat4(const std::string& name, glm::mat4& matrix);
    void setUniformBlockBinding(const std::string& name, int index);
    GLuint getUniformLocation(const std::string& name) const;
};

class GLN_API ComputeShader : public Shader {
public:
    ComputeShader(const GLuint shaderProgram, const std::string& shaderName, int uniformCount) : 
        Shader(shaderProgram, shaderName, uniformCount) {}
    void dispatch(int numGroupsX, int numGroupsY = 1, int numGroupsZ = 1);
};


#endif /* Shader_h */
