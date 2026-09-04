#include "glad/include/glad/glad.h"
#include "glfw-3.5.1/include/GLFW/glfw3.h"
#include <iostream>
#include <math.h>
#include "shader_program.h"

const char* vertex_shader_sourcecode = 
    "#version 330 core\n"

    "layout (location = 0) in vec3 aPos;\n" // define input as vec3, store in aPos
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 shaderColor;\n"

    "void main()\n"
    "{\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // output is bound to gl_Position
        "shaderColor = aColor;\n"
    "}";

const char* fragment_shader_sourcecode = 
    "#version 330 core\n"

    "in vec3 shaderColor;\n" // from the vertex shader
    "out vec4 FragColor;\n" // define vec4 FragColor as output 

    "void main()\n"
    "{\n"
        "FragColor = vec4(shaderColor, 1.0);\n"
    "}";
    

// callback function everytime the user resizes the window
// width and height are the new sizes of the os window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // set the rendering window size relative to the os window size
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    glfwInit();
    // set version to opengl 3.3 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // use opengl core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize glad before any opengl function calls
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // set the callback function for window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ShaderProgram shader_program{vertex_shader_sourcecode, fragment_shader_sourcecode};

    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    unsigned int indices[] = {
        0, 1, 2
    };


    unsigned int vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);

    unsigned int element_buffer_object;
    glGenBuffers(1, &element_buffer_object);

    unsigned int vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);


    glBindVertexArray(vertex_array_object); // bind the VAO

    // bind and send data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    // copy data into whatever's bound to GL_ARRAY_BUFFER, copies vertices into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind and send data to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    // feed the EBO vertex indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // snapshots whatever VBO is bound, allowing multiple bindings of VBO
    // VBO position attribute configuration
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // VBO color attribute configuration
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO now safe
    // unbind EBO NOT safe
    glBindVertexArray(0); // unbind VAO now safe

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // for wireframe mode

        shader_program.use_program();

        glBindVertexArray(vertex_array_object);

        // float time_value = glfwGetTime();
        // float green_value = (sin(time_value) / 2.0f) + 0.5f;
        // int uniformLocation = glGetUniformLocation(shader_program, "uniformColor");
        // glUniform4f(uniformLocation, 0.0f, green_value, 0.0f, 1.0f);

        // glDrawArrays(GL_TRIANGLES, 0, 3); // for drawing with only VBO, no EBO
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents(); // checks if events are triggered and calls callback functions if so
    }

    glfwTerminate();
    return 0;
}

