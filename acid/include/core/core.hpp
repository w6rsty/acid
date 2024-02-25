#pragma once

#include <memory>

#ifdef ACID_DEBUG
    #include "glm/glm.hpp"
    #include <iostream>
#endif

namespace acid
{

#ifdef ACID_DEBUG
    #define ACID_ENABLE_LOGGING
    #define ACID_ENABLE_ASSERTS


    static std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
    {
        os << "mat4:\n";
        os << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", " << mat[0][3] << "\n";
        os << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", " << mat[1][3] << "\n";
        os << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ", " << mat[2][3] << "\n";
        os << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2] << ", " << mat[3][3] << "\n";
        return os;
    }
#endif


} // namespace acid
