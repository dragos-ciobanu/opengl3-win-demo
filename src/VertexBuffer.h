//
// Created by dragos on 28.04.21.
//
#pragma once
#ifndef HAND_VERTEXBUFFER_H
#define HAND_VERTEXBUFFER_H
class VertexBuffer
{
private:
    unsigned int m_RendererId;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};
#endif //HAND_VERTEXBUFFER_H
