//
// Created by abdoe on 23/06/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include "third_party/glad/include/glad/glad.h"

#include <vector>

class Texture2D {
public:
  Texture2D()
      : Width(0), Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB),
        WrapS(GL_REPEAT), WrapT(GL_REPEAT), FilterMin(GL_LINEAR),
        FilterMax(GL_LINEAR) {
    glGenTextures(1, &Id);
  }
  void Generate(unsigned int width, unsigned int height,
                const unsigned char *data) {
    Width = width;
    Height = height;

    // create Texture
    glBindTexture(GL_TEXTURE_2D, Id);
    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0,
                 ImageFormat, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterMax);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  void Bind() const { glBindTexture(GL_TEXTURE_2D, Id); }

  unsigned int Id;
  unsigned int Width, Height;
  unsigned int InternalFormat;
  unsigned int ImageFormat;
  unsigned int WrapS;
  unsigned int WrapT;
  unsigned int FilterMin;
  unsigned int FilterMax;
};

#endif // TEXTURE_H
