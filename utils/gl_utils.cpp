#define GL_GLEXT_PROTOTYPES
#include "gl_utils.h"
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
std::vector< float > loadobj(string objfile){
	
	//const char * path,
  std::vector < glm::vec3 > out_vertices;
  std::vector < glm::vec2 > out_uvs;
  std::vector < glm::vec3 > out_normals;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector<float> altVertices;

	FILE * file = fopen(objfile.c_str(), "r");
	if( file == NULL ){
		  printf("Impossible to open the file !\n");
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		    break; // EOF = End Of File. Quit the loop.
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				  printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				  //return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	// For each vertex of each triangle
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
    // else : parse lineHeader
			unsigned int vertexIndex = vertexIndices[i];
			glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
			out_vertices.push_back(vertex);
			altVertices.push_back(vertex.x);
			altVertices.push_back(vertex.y);
			altVertices.push_back(vertex.z);
	}

	return altVertices;
}

void glErrorCheck()
{
    int errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        printf("Failure in OpenGL %d \n", errCode);
        //exit(0);
    }
}

string readShaderFile(const char* filename){

	ifstream infile;
	infile.open(filename,  std::ifstream::binary);
	string contents;
	string line;
	int i = 0;
if(!infile)
    {
				cout<<"FAIL in filereading"<<endl;
        //return -1;
    } 
	while(getline(infile, line)){
		contents.append(line + "\n");
		i++;
	}
	return contents;

}

GLuint loadShaders(const char* vertexShader, const char* fragmentShader){
	
	//Get contents of shader files
	string vertSource = readShaderFile(vertexShader);
	string fragSource = readShaderFile(fragmentShader);
	//Create handles for the shaders
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Transform the string into something useful for the shadersource.
	const GLchar *source = (const GLchar *)vertSource.c_str();
	//Set source code and compile shaders
	glShaderSource(vertShader, 1, &source, NULL);
	glCompileShader(vertShader);
	//Test if the compilation was successfull
	GLint isCompiled = 0;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
	
	if(isCompiled == GL_FALSE){

		cout << " vert shader not compiled"<< endl;
		GLint maxLength = 0;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertShader, maxLength, &maxLength, &infoLog[0]);
		//We don't need the shader anymore.
		glDeleteShader(vertShader);

		return 0;
	}	

	//Same process for the fragment shader as for the vertex shader
	source = (const GLchar *)fragSource.c_str();
	glShaderSource(fragShader, 1, &source, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
	
	if(isCompiled == GL_FALSE){

		cout << " frag shader not compiled"<< endl;

		GLint maxLength = 0;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragShader, maxLength, &maxLength, &infoLog[0]);
	
		//We don't need the shader anymore.
		glDeleteShader(fragShader);

		return 0;
	}	
	
	//Finally create and compile the program object to be used for using the shaders.
	GLuint program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	//Link our program
	glLinkProgram(program);
	
	//Attach our shaders to our program
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);
	//Not needed anymore.
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	//return handle to our shaders
	return program; 

}



