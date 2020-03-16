#ifndef SHADER_H
#define SHDAER_H

#include <glad/glad.h> // �ʿ��� ��� OpenGL�� ��������� �������� ���� glad�� �����մϴ�.

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // program ID
    unsigned int ID;

    // �����ڴ� shader�� �а� �����մϴ�.
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // shader�� Ȱ��ȭ�ϰ� ����մϴ�.
    void use();
    // Uniform ��ƿ��Ƽ �Լ���
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};



Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) 
{
    // 1. ���� ��θ� ���� vertex/fragment shader �ҽ� �ڵ带 �˻��մϴ�.
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ifstream ��ü���� ���ܸ� ���� �� �ֵ��� �մϴ�.
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // ���� ����
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        // stream�� ������ ���� ������ �б�
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // ���� �ڵ鷯 �ݱ�
        vShaderFile.close();
        fShaderFile.close();

        // stream�� string���� ��ȯ
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. shader ������
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // ������ �߻��Ѵٸ� ������ ������ ���
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Fragment shader�� ��������
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // ������ �߻��Ѵٸ� ������ ������ ���
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // ������ �߻��Ѵٸ� ��ŷ ������ ���
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // program ���ο��� shader���� ��ũ �Ϸ�Ǿ��ٸ� ���� �ʿ� �����Ƿ� shader���� ����
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() 
{
    glUseProgram(ID);
}


void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

#endif