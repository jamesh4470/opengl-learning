#include "shader_program.h"
#include <iostream>

ShaderProgram::ShaderProgram(const char* vertex_shader_sourcecode, const char* fragment_shader_sourcecode) {
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // 1 for 1 string passed
    glShaderSource(vertex_shader, 1, &vertex_shader_sourcecode, nullptr);
    glCompileShader(vertex_shader);

    // vertex shader error check
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
        std::cout << "Vertex shader compilation failed\n" << infoLog << std::endl;
    }


    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_sourcecode, nullptr);
    glCompileShader(fragment_shader);

    // fragment shader error check
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
        std::cout << "Fragment shader compilation failed\n" << infoLog << std::endl;
    }


    // object that links individual shaders together
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // shader program linking error check
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, infoLog);
        std::cout << "Vertex shader linking failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}


void ShaderProgram::use_program() {
    glUseProgram(shader_program);
}


// uniforms are uniform for all verticies inputted through the shader program
void ShaderProgram::set_uniform_bool(const char* uniform_name, bool value) {
    int uniformLocation = glGetUniformLocation(shader_program, uniform_name);
    glUniform1i(uniformLocation, (int) value);
}


void ShaderProgram::set_uniform_int(const char* uniform_name, int value) {
    int uniformLocation = glGetUniformLocation(shader_program, uniform_name);
    glUniform1i(uniformLocation, (int) value);
}


void ShaderProgram::set_uniform_float(const char* uniform_name, float value) {
    int uniformLocation = glGetUniformLocation(shader_program, uniform_name);
    glUniform1f(uniformLocation, value);
}
