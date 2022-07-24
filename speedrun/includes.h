#include "GL/glew.h"
#include <iostream>
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include "IMGUI/imgui_impl_glfw.h"

#ifdef _DEBUG
#define falseBreak(x) if(!(x)) __debugbreak();
#else
#define falseBreak(x) if((x)) x;
#endif

#define report(x) ClearErrors(); x; falseBreak(DisplayError(#x, __FILE__, __LINE__))

#define _USE_MATH_DEFINES
#include <math.h>

#define toRadians M_PI/180.0
#define toDegrees 180.0/M_PI

#define rad(x) (x) * toRadians
#define deg(x) (x) * toDegrees

#define randomNumber(min, max) (min) + ( std::rand() % ( (max) - (min) + 1 ) )


inline void ClearErrors() {

    while (glGetError() != GL_NO_ERROR);
}

inline const bool& DisplayError(const char* function, const char* file, int line)
{

    bool continueCode = true;

    while (GLenum error = glGetError()) 
    {
        std::cout << "Error " << error << " has occured in function" << function << ", line " << line << " in file " << file << "\n";

        return false;
    }

    return true;
}

