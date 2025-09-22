#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Trying OpenGL"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
bool test_shader(unsigned int shader);

int main(void) {
    printf("Hello OpenGL\n");

    GLFWwindow *window;

    if (!glfwInit()) {
        fprintf(stderr, "failed to initialize GLFW!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT,
        WINDOW_TITLE,
        NULL, NULL
        );

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "failed to initialize GLAD\n");
        return -1;
    }

    //~begin SHADERS
    const char *vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec3 a_pos;\n"
        "void main() {\n"
        "    gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
        "}\n"
        ;

    const char *fragment_shader_source =
        "#version 330 core\n"
        "out vec4 frag_color;\n"
        "uniform vec4 our_color;\n"
        "void main() {\n"
        "    frag_color = our_color;\n"
        "}\n"
        ;

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    if (!test_shader(vertex_shader)) {
        return -1;
    }

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    if (!test_shader(fragment_shader)) {
        return -1;
    }

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    {
        int success;
        char infoLog[512];
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
            fprintf(stderr, "ERROR::SHADER_PROGRAM::LINKING_FAILED:\n%s\n", infoLog);
            return -1;
        }
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int vertex_color_location = glGetUniformLocation(shader_program, "our_color");
    //~end SHADERS
    
    // elements
    float vertices[] = {
         0.5f,  0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f,
        -0.5f,  0.5f,  0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int vao; // vertex array object
    glGenVertexArrays(1, &vao);

    unsigned int vbo; // vertex buffer object
    glGenBuffers(1, &vbo);

    unsigned int ebo; // element buffer object
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time_value = glfwGetTime();
        float green_value = (sin(time_value) / 2.0f) + 0.5f;

        glUseProgram(shader_program);
        glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0); // no need to unbind everytime

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    printf("window resized: %dx%d\n", width, height);
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}


bool test_shader(unsigned int shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED:\n%s\n", infoLog);
        return false;
    }

    return true;
}
