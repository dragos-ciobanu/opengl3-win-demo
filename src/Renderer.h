//
// Created by dragos on 28.04.21.
//

#include <iostream>
#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"

#ifndef HAND_RENDERER_H
#define HAND_RENDERER_H

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
    x;                            \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
private:

public:
    void Clear() const;
    void Draw(const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader) const;
};
#endif //HAND_RENDERER_H
