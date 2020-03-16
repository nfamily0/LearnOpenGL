#ifndef SHADER_H
#define SHDAER_H

#include <glad/glad.h> // 필요한 모든 OpenGL의 헤더파일을 가져오기 위해 glad를 포함합니다.

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // program ID
    unsigned int ID;

    // 생성자는 shader를 읽고 생성합니다.
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // shader를 활성화하고 사용합니다.
    void use();
    // Uniform 유틸리티 함수들
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};



Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) 
{
    // 1. 파일 경로를 통해 vertex/fragment shader 소스 코드를 검색합니다.
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ifstream 객체들이 예외를 던질 수 있도록 합니다.
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // 파일 열기
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        // stream에 파일의 버퍼 내용을 읽기
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // 파일 핸들러 닫기
        vShaderFile.close();
        fShaderFile.close();

        // stream을 string으로 변환
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. shader 컴파일
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // 오류가 발생한다면 컴파일 오류를 출력
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Fragment shader도 마찬가지
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // 오류가 발생한다면 컴파일 오류를 출력
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

    // 오류가 발생한다면 링킹 오류를 출력
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // program 내부에서 shader들이 링크 완료되었다면 이제 필요 없으므로 shader들을 삭제
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