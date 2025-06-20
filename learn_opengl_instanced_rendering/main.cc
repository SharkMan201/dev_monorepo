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
  glfwWindowHint(GLFW_SAMPLES, 4);

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

  Shader instance_shader(
      "_main/learn_opengl_instanced_rendering/shaders/instance_shader.vert",
      "_main/learn_opengl_instanced_rendering/shaders/shader.frag");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  float delta_time = 0.0f;
  float last_frame = 0.0f;

  Model planet_model(
      "_main/learn_opengl_instanced_rendering/models/planet/planet.obj");
  Model rock_model(
      "_main/learn_opengl_instanced_rendering/models/rock/rock.obj");

  unsigned int amount = 10000;
  std::vector<glm::mat4> modelMatrices(amount);

  // instanceBufferObject
  unsigned int ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ARRAY_BUFFER, ibo);
  glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4),
               modelMatrices.data(), GL_STATIC_DRAW);

  for (int i = 0; i < rock_model.meshes_.size(); i++) {
    auto vao = rock_model.meshes_[i]->vao;
    glBindVertexArray(vao);

    // vertex attributes
    auto vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          reinterpret_cast<void *>(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          reinterpret_cast<void *>(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          reinterpret_cast<void *>(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
  }

  bool first_render = true;

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
                                       800.0f / 600.0f, 0.1f, 200.0f);
    our_shader.use(); // need to activate the shader before setting the uniforms
    our_shader.setMat4("view", camera.GetViewMatrix());
    our_shader.setMat4("projection", projection);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    our_shader.setMat4("model", model);
    planet_model.Draw(our_shader);

    srand(0);
    float radius = 80.0f;
    float offset = 25.0f;

    for (auto i = 0; i < amount; i++) {
      glm::mat4 model = glm::mat4(1.0f);

      // 1. translation: displace along circle with 'radius' in range [-offset,
      // offset]
      float angle = static_cast<float>(i) / amount * 360.0f + glfwGetTime() / 10;
      float displacement =
          (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
      float x = sin(angle) * radius + displacement;
      displacement =
          (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
      float y = displacement *
                0.4f; // keep height of field smaller compared to width of x & z
      displacement =
          (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
      float z = cos(angle) * radius + displacement;
      model = glm::translate(model, glm::vec3(x, y, z));

      // 2. scale: scale between 0.05 and 09.25f
      float scale = (rand() % 20) / 100.0f + 0.05f;
      model = glm::scale(model, glm::vec3(scale));

      // 3. rotation: add random rotation around a semi-randomly picked rotation
      // axis vector
      float rotAngle = (rand() % 360) + glfwGetTime() / 10;
      model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

      // 4. add to list of matrices
      modelMatrices[i] = model;
    }

    glBindBuffer(GL_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4),
                 modelMatrices.data(), GL_STATIC_DRAW);

    instance_shader.use();
    instance_shader.setMat4("view", camera.GetViewMatrix());
    instance_shader.setMat4("projection", projection);
    for (int i = 0; i < rock_model.meshes_.size(); i++) {
      glBindVertexArray(rock_model.meshes_[i]->vao);
      glDrawElementsInstanced(GL_TRIANGLES,
                              rock_model.meshes_[i]->indices_.size(),
                              GL_UNSIGNED_INT, 0, amount);
    }

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