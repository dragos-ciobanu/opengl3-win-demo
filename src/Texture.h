//
// Created by dragos on 01.05.21.
//

#ifndef HAND_TEXTURE_H
#define HAND_TEXTURE_H

#include "Renderer.h"

class Texture
{
private:
    unsigned int m_RendererId;
    std::string m_filePath;
    unsigned char* m_localBuffer;
    int m_width, m_height, m_BPP;
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int getWidth() const { return m_width;};
    inline int getHeight() const { return m_height;};
};
#endif //HAND_TEXTURE_H
