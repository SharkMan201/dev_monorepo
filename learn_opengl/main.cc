//
// Created by abdoe on 6/06/2025.
//
#include "third_party/glad/include/glad/glad.h"

#include <GLFW/glfw3.h>
#include <iostream>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  // Update view port width & height with resize
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
}

const char *vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "  ourColor = aColor;\n"
    "}\0";

const char *fragment_shader_source = "#version 330 core\n"
                                     "in vec3 ourColor;"
                                     "out vec4 FragColor;\n"
                                     "void main() {\n"
                                     "  FragColor =  vec4(ourColor, 1.0);\n"
                                     "}\0";

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for MacOS
#endif

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  float vertices[] = {
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.0f,
      0.0f, 1.0f,  0.0f, 0.0f, 0.5f, 0.0f, 0.0f,  0.0f,  1.0f,
  };
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  // 1. bind vertex array object
  glBindVertexArray(vao);

  // 2. copy our vertices array in a buffer for OpenGL to use
  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // tell openGL how to read the vertex buffer data
  // setup vertices (location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // setup colors (location = 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // setup element buffer object
  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // unbinding buffer & array objects
  glBindVertexArray(NULL); // needs to be unbound first to not record unbinding
                           // the ebos
  glBindBuffer(GL_ARRAY_BUFFER, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

  // create shaders
  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
  glCompileShader(vertex_shader);
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << info_log << std::endl;
    return -1;
  }

  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << info_log << std::endl;
    return -1;
  }

  // link shaders to a program
  unsigned int shader_program;
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
    std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    return -1;
  }

  // free up shaders after they have been linked
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering commands
    // ------
    // clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // set openGL to use the program
    glUseProgram(shader_program);

    float time_value = glfwGetTime();
    float green_value = (sin(time_value / 2.0f)) + 0.5f;
    // retrieve the uniform variable
    int vertex_color_location =
        glGetUniformLocation(shader_program, "ourColor");
    // set the value of the uniform variable to change over time
    glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);

    // draw the triangle
    glBindVertexArray(vao);
    // no need to bind vbo & ebo because the vao keeps track of these bindings
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //  glBindVertexArray(0); // no need to unbind everytime

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // de-allocate all resources
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return 0;
}