#pragma once

#include <cstdlib>
#include <glad/gl.h>

[[nodiscard]] constexpr bool is_integral_type(GLenum const data_type) {
    switch (data_type) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_INT:
        case GL_UNSIGNED_INT:
            return true;
        default:
            return false;
    }
}

[[nodiscard]] constexpr std::size_t get_size_of_gl_type(GLenum const data_type) {
    switch (data_type) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            return 1;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_HALF_FLOAT:
            return 2;
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_FLOAT:
            return 4;
        case GL_DOUBLE:
            return 8;
        default:
            return 0;
    }
}
