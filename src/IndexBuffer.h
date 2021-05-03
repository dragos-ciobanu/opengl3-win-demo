//
// Created by dragos on 28.04.21.
//
#pragma once
#ifndef HAND_INDEXBUFFER_H
#define HAND_INDEXBUFFER_H
class IndexBuffer
{
private:
    unsigned int m_RendererId;
    int m_Count;
public:
    IndexBuffer(const unsigned int* data, int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    int getCount() const { return m_Count;}
};
#endif //HAND_IndexBUFFER_H
