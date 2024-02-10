#pragma once

#include <glad/gl.h>

struct VertexAttributeDefinition {
    VertexAttributeDefinition(GLint const count, GLenum const type, GLboolean const normalized) noexcept
        : count{ count },
          type{ type },
          normalized{ normalized } { }

    GLint count; // e.g. 3 for a vec3 position vector
    GLenum type; // e.g. GL_FLOAT
    GLboolean normalized;
};
