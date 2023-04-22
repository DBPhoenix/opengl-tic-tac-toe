#include <glad/glad.h>

#include <iostream>

class Shader
{
public:
    void    Compile ();
    void    Use     ();
private:
    unsigned int id;

    const char *vertexShaderSource;
    const char *fragmentShaderSource;

    unsigned int    compileVertex   ();
    unsigned int    compileFragment ();
};
