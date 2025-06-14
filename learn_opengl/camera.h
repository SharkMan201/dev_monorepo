//
// Created by abdoe on 14/06/2025.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "third_party/glad/include/glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

enum CAMERA_MOVEMENT { FORWARD, BACKWARD, LEFT, RIGHT };

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.2f;
const float FOV = 45.0f;

class Camera {
public:
  // constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH)
      : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(SPEED),
        mouse_sensitivity_(SENSITIVITY), fov_(FOV) {
    position_ = position;
    world_up_ = up;
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
  }
  // constructor with scalar values
  Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y,
         float up_z, float yaw, float pitch)
      : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(SPEED),
        mouse_sensitivity_(SENSITIVITY), fov_(FOV) {
    position_ = glm::vec3(pos_x, pos_y, pos_z);
    world_up_ = glm::vec3(up_x, up_y, up_z);
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
  }

  glm::mat4 getViewMatrix() {
    return glm::lookAt(position_, position_ + front_, up_);
  }

  float getFov() { return fov_; }

  void processKeyboard(CAMERA_MOVEMENT direction, float delta_time) {
    float velocity = delta_time * movement_speed_;
    switch (direction) {
    case FORWARD:
      position_ += front_ * velocity;
      break;
    case BACKWARD:
      position_ -= front_ * velocity;
      break;
    case LEFT:
      position_ -= right_ * velocity;
      break;
    case RIGHT:
      position_ += right_ * velocity;
      break;
    }
  }

  void processMouseMovement(float x_offset, float y_offset,
                            GLboolean constrainPitch = true) {
    x_offset *= mouse_sensitivity_;
    y_offset *= mouse_sensitivity_;

    yaw_ += x_offset;
    pitch_ += y_offset;

    if (constrainPitch) {
      if (pitch_ > 89.0f)
        pitch_ = 89.0f;
      if (pitch_ < -89.0f)
        pitch_ = -89.0f;
    }

    updateCameraVectors();
  }

  void processMouseScroll(float y_offset) {
    fov_ -= y_offset;
    if (fov_ < 1.0f)
      fov_ = 1.0f;
    if (fov_ > 45.0f)
      fov_ = 45.0f;
  }

private:
  void updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }

  // camera attributes
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;
  // euler angles
  float yaw_;
  float pitch_;
  // camera options
  float movement_speed_;
  float mouse_sensitivity_;
  float fov_;
};

#endif // CAMERA_H
