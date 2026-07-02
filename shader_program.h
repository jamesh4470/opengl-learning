#ifndef SHADER_H
#define SHADER_H

#include "glad/include/glad/glad.h"
#include <string>

class ShaderProgram {
    public:
        unsigned int shader_program; // shader program ID

        ShaderProgram(const char* vertex_shader_sourcecode, const char* fragment_shader_sourcecode);

        void use_program();

        void set_uniform_bool(const char* uniform_name, bool value);
        void set_uniform_int(const char* uniform_name, int value);
        void set_uniform_float(const char* uniform_name, float value);
};

#endif
