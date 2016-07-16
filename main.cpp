#define GL_GLEXT_PROTOTYPES	
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "utils/gl_utils.h"
#include "utils/loadobj.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include "glm/ext.hpp"
#include <glm/gtx/transform.hpp> 
//#include <glm/gtc/matrix_transform.hpp> 
//#include <glm/gtx/transform.hpp>
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

//Håll koden clean!
//FIXA SKALNING!!
//KANSKE SHADA!
//KUBEN OCH RUTAN HAR INTE SAMMA UTGÅNGSPOSITION, DET ÄR NOG ETT PROBLEM
//Kan läsa teapot men inte stanford bunny av någon anledning. Storlek?
//läs in modell och triangulisera
//måla ut rektanglar med interaktionsfunktioner
//kolla hur man kan måla trianglar hellre än pixlar
//Finns det annat sätt än att kolla xy?
//skriv ut text


using namespace std;

const int W=600, H=600;
vector<float> model;
GLuint vertArray, vertBuffer, markerArray, markerBuffer, upperLeftArray, upperLeftBuffer, lowerRightArray, lowerRightBuffer;
glm::vec4 strut, strut2;
GLfloat upperLeft[] = {
	-0.9f, 0.9f, 0.0f,
};
GLfloat lowerRight[] = {
	0.9f, -0.9f, 0.0f,
};

GLfloat marker[] = {
	-0.9f, -0.9f, 0.0f,
	0.9f, -0.9f, 0.0f,
	0.9f,  0.9f, 0.0f,
	-0.9f,  0.9f, 0.0f,
};

GLuint drawShader, markerShader;
glm::mat4 Projection = glm::perspective(45.0f, 3.f / 3.f, 0.1f, 100.f);
//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) W / (float)H, 0.1f, 300.0f);
glm::mat4 View = glm::lookAt(
    glm::vec3(0,0,10), // Camera is at (4,3,3), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
 
glm::mat4 Scale = glm::mat4(1.0f);

glm::mat4 MVP;

void draw(){
	//MVP = Projection*View*Scale;
	
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glUseProgram(drawShader);
	glBindVertexArray(vertArray);
	glEnableVertexAttribArray(0);
	glUniformMatrix4fv(glGetUniformLocation(drawShader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniform2f(glGetUniformLocation(drawShader, "upperLeft"), strut.x, strut.y);
	glUniform2f(glGetUniformLocation(drawShader, "lowerRight"), strut2.x, strut2.y);
	glDrawArrays(GL_TRIANGLES, 0, model.size()/3);	
	glDisableVertexAttribArray(0);	
	glBindVertexArray(0);
	glUseProgram(0);

	glUseProgram(markerShader);
	glBindVertexArray(markerArray);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_LOOP, 0, 4);	
	glDisableVertexAttribArray(0);	
	glBindVertexArray(0);
	glUseProgram(0);

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, W, H);
	Projection = glm::perspective(45.0f, 3.f / 3.f, 0.1f, 100.f);
}

// This function is called whenever the computer is idle
// As soon as the machine is idle, ask GLUT to trigger rendering of a new
// frame
void idle()
{
  glutPostRedisplay();
}
/*
	GLfloat marker[] = {
	-0.9f, -0.9f, 0.0f,
	0.9f, -0.9f, 0.0f,
	0.9f,  0.9f, 0.0f,
	-0.9f,  0.9f, 0.0f,
};
*/
void changeMarker(){
	marker[0] = upperLeft[0];
	marker[1] = upperLeft[1];
	marker[3] = upperLeft[0];
	marker[4] = lowerRight[1];
	marker[6] = lowerRight[0];
	marker[7] = lowerRight[1];
	marker[9] = lowerRight[0];
	marker[10] = upperLeft[1];
	glBindBuffer(GL_ARRAY_BUFFER, markerBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(marker), marker, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	strut = MVP * glm::vec4(upperLeft[0], upperLeft[1], 0.0f, 1.0f);
	strut2 = MVP * glm::vec4(lowerRight[0], lowerRight[1], 0.0f, 1.0f);
	
}

void mouseHoldFunction(int x, int y){
	
	//if(button == GLUT_LEFT_BUTTON){
		lowerRight[0] = (float)x/W*2.0f-1.0f;
		lowerRight[1] = -1.0f*((float)y/H*2.0f-1.0f);
		cout<<"x: "<<to_string(lowerRight[0])<<endl;
		cout<<"y: "<<to_string(lowerRight[1])<<endl;
		changeMarker();
	//}
	
}

void mouseFunction(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		upperLeft[0] = (float)x/W*2.0f-1.0f;
		upperLeft[1] = -1.0f*((float)y/H*2.0f-1.0f);
		cout<<"x: "<<to_string(upperLeft[0])<<endl;
		cout<<"y: "<<to_string(upperLeft[1])<<endl;
	}
	/*if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		lowerRight[0] = (float)x/W*2.0f-1.0f;
		lowerRight[1] = -1.0f*((float)y/H*2.0f-1.0f);
		cout<<"x: "<<to_string(lowerRight[0])<<endl;
		cout<<"y: "<<to_string(lowerRight[1])<<endl;
		changeMarker();
	}*/
}

void init(){
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(W,H);
	glutInitContextVersion(3, 3);	
	glutCreateWindow("Working Title");
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMotionFunc(mouseHoldFunction);	
	glutMouseFunc(mouseFunction);	
	
	drawShader = loadShaders("shaders/draw.vert", "shaders/draw.frag");
	markerShader = loadShaders("shaders/marker.vert", "shaders/marker.frag");

	model = loadobj("./test.obj");
	glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);
	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.size(), &model[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "inPosition"),3, GL_FLOAT,GL_FALSE,0,0);
	//glVertexAttribPointer(glGetAttribLocation(drawShader, "in_Position"),3, GL_FLOAT,GL_FALSE,0,0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &markerArray);
	glBindVertexArray(markerArray);
	glGenBuffers(1, &markerBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, markerBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(marker), marker, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(markerShader, "inPosition"),3, GL_FLOAT,GL_FALSE,0,0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &upperLeftArray);
	glBindVertexArray(upperLeftArray);
	glGenBuffers(1, &upperLeftBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, upperLeftBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(upperLeft), upperLeft, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "upperLeft"),3, GL_FLOAT,GL_FALSE,0,0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	MVP = Projection*View*Scale;
	std::cout<<glm::to_string(MVP)<<std::endl;

}

int main(int argc, char **argv){
	std::cout << "Hello Worldish!!!"<<std::endl;
	glutInit(&argc, argv);
	init();
	glutMainLoop();
	exit(0);
}
