#ifndef SHADERREADER_H	//header guard to prevent multiple inclusion problems
#define SHADERREADER_H
 
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace std;
std::vector< float > loadobj(std::string objfile);
void glErrorCheck();
string readFile(const char* filename);
GLuint loadShaders(const char* vertexShader, const char* fragmentShader);

#endif
