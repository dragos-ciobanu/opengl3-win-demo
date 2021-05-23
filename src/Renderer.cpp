//
// Created by dragos on 28.04.21.
//
#include "Renderer.h"
#include <iostream>
void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "):" << function << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}


void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::Clear() const {
    glClearColor(0.05f, 0.2f, 0.2f, 1.0f);
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
