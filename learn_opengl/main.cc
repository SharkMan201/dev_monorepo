//
// Created by abdoe on 6/06/2025.
//
#include "learn_opengl/camera.h"
#include "learn_opengl/shaders/shader.h"
#include "third_party/glad/include/glad/glad.h"
#include "third_party/stb_image/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <iostream>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  // Update view port width & height with resize
  glViewport(0, 0, width, height);
}

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void processInput(GLFWwindow *window, float delta_time) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.processKeyboard(CAMERA_MOVEMENT::FORWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboard(CAMERA_MOVEMENT::BACKWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboard(CAMERA_MOVEMENT::LEFT, delta_time);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboard(CAMERA_MOVEMENT::RIGHT, delta_time);
}
// store mouse data
float last_x = 400; // middle of the screen
float last_y = 300;
bool first_mouse = true;

void mouseCallback(GLFWwindow *window, double x_pos, double y_pos) {
  if (first_mouse) {
    first_mouse = false;
    last_x = x_pos;
    last_y = y_pos;
  }
  float x_offest = x_pos - last_x;
  float y_offset = last_y - y_pos;
  last_x = x_pos;
  last_y = y_pos;

  camera.processMouseMovement(x_offest, y_offset);
}

void scrollCallback(GLFWwindow *window, double x_offset, double y_offset) {
  camera.processMouseScroll(static_cast<float>(y_offset));
}

unsigned int loadTexture(const std::string &path) {
  unsigned int texture_id;
  glGenTextures(1, &texture_id);

  int width, height, nr_components;
  unsigned char *data = stbi_load(LocalPaths::getLocalPath(path).c_str(),
                                  &width, &height, &nr_components, 0);
  if (data) {
    GLenum format = GL_RED;
    if (nr_components == 1)
      format = GL_RED;
    else if (nr_components == 3)
      format = GL_RGB;
    else if (nr_components == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    const std::string err_message =
        "Texture failed to load at path " + path + "\n";
    stbi_image_free(data);
    throw std::runtime_error(err_message);
  }

  return texture_id;
}

int solve() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS
#endif

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // capture the mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
      0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
      0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
      -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
      0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
      -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
      0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
      -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
      1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
      -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};
  glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // setup stbi to flip image y-axis by default
  stbi_set_flip_vertically_on_load(true);

  // light setup
  unsigned int light_vao;
  glGenVertexArrays(1, &light_vao);
  glBindVertexArray(light_vao);
  // bind to the same vbo, the vbo contains the same data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // light maps setup
  auto diffuse_map = loadTexture("_main/learn_opengl/textures/container2.png");
  auto specular_map =
      loadTexture("_main/learn_opengl/textures/container2_specular.png");

  // create shaders
  Shader our_shader("_main/learn_opengl/shaders/shader.vert",
                    "_main/learn_opengl/shaders/shader.frag");

  Shader lighting_cube_shader("_main/learn_opengl/shaders/light_shader.vert",
                              "_main/learn_opengl/shaders/light_shader.frag");

  glEnable(GL_DEPTH_TEST);

  float delta_time = 0.0f;
  float last_frame = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    // calculate delta
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    // input
    processInput(window, delta_time);

    // rendering commands
    // ------
    // clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw the light
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, light_pos);
    model = glm::scale(model, glm::vec3(0.2f));
    auto projection = glm::perspective(glm::radians(camera.getFov()),
                                       800.0f / 600.0f, 0.1f, 100.0f);

    lighting_cube_shader.use();
    lighting_cube_shader.setMatrix("model", model);
    lighting_cube_shader.setMatrix("view", camera.getViewMatrix());
    lighting_cube_shader.setMatrix("projection", projection);

    glBindVertexArray(light_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw the cube
    model = glm::mat4(1.0f);
    // model = glm::rotate(model, 20 * float(glm::radians(glfwGetTime())),
    //                     glm::vec3(0.0f, 1.0f, 0.0f));
    our_shader.use(); // need to activate the shader before setting the uniforms
    our_shader.setInt("material.diffuse", 0);
    our_shader.setInt("material.specular", 1);
    our_shader.setFloat("material.shininess", 32.0f);

    our_shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    our_shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    our_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    our_shader.setVec3("light.position", light_pos);

    our_shader.setVec3("viewPos", camera.getPosition());

    our_shader.setMatrix("model", model);
    our_shader.setMatrix("view", camera.getViewMatrix());
    our_shader.setMatrix("projection", projection);

    // set up the diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_map);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_map);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // de-allocate all resources
  glDeleteVertexArrays(1, &vao);
  glDeleteVertexArrays(1, &light_vao);
  glDeleteBuffers(1, &vbo);
  our_shader.destroy();
  lighting_cube_shader.destroy();

  glfwTerminate();
  return 0;
}

int main() {
  try {
    return solve();
  } catch (std::exception &ex) {
    std::cerr << ex.what() << std::endl << std::flush;
    return -1;
  }
}