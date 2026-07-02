#include "glad/include/glad/glad.h"
#include "glfw-3.4/include/GLFW/glfw3.h"
#include <iostream>
#include <math.h>

const char* vertex_shader_sourcecode = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n" // define input as vec3, store in aPos
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // output is bound to gl_Position
    "}";

const char* fragment_shader_sourcecode = 
    "#version 330 core\n"
    "out vec4 FragColor;\n" // define vec4 FragColor as output 
    "uniform vec4 uniformColor;\n"
    "void main()\n"
    "{\n"
        // "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);" // assign color
        "FragColor = uniformColor;\n"
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


    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // 1 for 1 string passed
    glShaderSource(vertex_shader, 1, &vertex_shader_sourcecode, nullptr);
    glCompileShader(vertex_shader);
    
    /**  for shader compilation error checking
     * int success;
     * char infoLog[512];
     * glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
     * if (!success) {
     *     glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
     *     std::cout << "Vertex shader compilation failed\n" << infoLog << std::endl;
     * }
     */


    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_sourcecode, nullptr);
    glCompileShader(fragment_shader);


    // object that links individual shaders together
    unsigned int shader_program; 
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    /** for shader linking error checking
     * int success;
     * char infoLog[512];
     * glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
     * if (!success) {
     *     glGetProgramInfoLog(shader_program, 512, nullptr, infoLog);
     *     std::cout << "Vertex shader linking failed\n" << infoLog << std::endl;
     * }
     */
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    // in normalized device coordinates, between -1 and 1
    float triangle_vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    float square_vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left 
    };
    unsigned int square_indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);

    // bind and send data to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    // feed the EBO vertex indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_indices), square_indices, GL_STATIC_DRAW);

    // position attribute is at location 0. each position has 3 float components.
    // snapshots whatever VBO is bound, allowing multiple bindings of VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    // enable the position attribute at location 0
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO now safe
    // unbind EBO NOT safe
    glBindVertexArray(0); // unbind VAO now safe

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // for wireframe mode

        glUseProgram(shader_program);   
        glBindVertexArray(vertex_array_object);

        float time_value = glfwGetTime();
        float green_value = (sin(time_value) / 2.0f) + 0.5f;
        int uniformLocation = glGetUniformLocation(shader_program, "uniformColor");
        glUniform4f(uniformLocation, 0.0f, green_value, 0.0f, 1.0f);

        // glDrawArrays(GL_TRIANGLES, 0, 3); // for drawing with only VBO, no EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents(); // checks if events are triggered and calls callback functions if so
    }

    glfwTerminate();
    return 0;
}

