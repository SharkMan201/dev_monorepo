//
// Created by abdoe on 6/06/2025.
//
#include "learn_opengl_utils/camera.h"
#include "learn_opengl_utils/model.h"
#include "learn_opengl_utils/shader.h"
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
    camera.ProcessKeyboard(CAMERA_MOVEMENT::FORWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(CAMERA_MOVEMENT::BACKWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(CAMERA_MOVEMENT::LEFT, delta_time);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(CAMERA_MOVEMENT::RIGHT, delta_time);
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

  camera.ProcessMouseMovement(x_offest, y_offset);
}

void scrollCallback(GLFWwindow *window, double x_offset, double y_offset) {
  camera.ProcessMouseScroll(static_cast<float>(y_offset));
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

  // setup stbi to flip image y-axis by default
  stbi_set_flip_vertically_on_load(true);

  // create shaders
  Shader our_shader(
      "_main/learn_opengl_instanced_rendering/shaders/shader.vert",
      "_main/learn_opengl_instanced_rendering/shaders/shader.frag");

  glEnable(GL_DEPTH_TEST);

  float delta_time = 0.0f;
  float last_frame = 0.0f;

  Model planet_model(
      "_main/learn_opengl_instanced_rendering/models/planet/planet.obj");
  Model rock_model(
      "_main/learn_opengl_instanced_rendering/models/rock/rock.obj");

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

    auto projection = glm::perspective(glm::radians(camera.GetFov()),
                                       800.0f / 600.0f, 0.1f, 100.0f);
    our_shader.use(); // need to activate the shader before setting the uniforms
    our_shader.setMat4("view", camera.GetViewMatrix());
    our_shader.setMat4("projection", projection);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    our_shader.setMat4("model", model);
    planet_model.Draw(our_shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, 4.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    our_shader.setMat4("model", model);
    rock_model.Draw(our_shader);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  our_shader.destroy();

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