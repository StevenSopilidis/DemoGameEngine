#pragma once

#include "GL/glew.h"

#include <vector>

namespace engine
{
struct VertexElement
{
    GLuint   index;  // location
    GLuint   size;   // num of components
    GLuint   type;   // data type (e.g GL_FLOAT etc)
    uint32_t offset; // bytes offset from start of vertex
};

struct VertexLayout
{
    std::vector<VertexElement> elements;
    uint32_t                   stride{0}; // total size of single vertex
};
} // namespace engine