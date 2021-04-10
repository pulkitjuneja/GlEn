
#pragma once
#ifndef Shader_h
#define Shader_h

#include "Glen/Globals.h"
#include <unordered_map>

using namespace std;

static int currentShaderProgramInUse;

class GLN_API Shader {
    void readFromFile(const string& fileName, char*& shaderContent);
    GLuint shaderProgram;
    unordered_map<string, GLuint> uniformLocations;
public:
    string shaderName;
    Shader(const GLuint shaderProgram, const string& shaderName, int uniformCount);
    void use();
    GLuint getShaderID();
    void setBool(const string& name, bool value) const;
    void setInt(const string& name, int value) const;
    void setFloat(const string& name, float value) const;
    void setFloat3(const string& name, float value1, float value2, float value3);
    void setMat4(const string& name, glm::mat4& matrix);
    void setUniformBlockBinding(const string& name, int index);
    GLuint getUniformLocation(const string& name) const;
};


#endif /* Shader_h */
