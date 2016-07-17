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
#include <math.h>
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


//susing namespace std;
using namespace glm;

const int sW=600, sH=600;
vector<float> model;
vector<float> triColors;
GLuint vertArray, vertBuffer, colorArray, colorBuffer, markerArray, markerBuffer, upperLeftArray, upperLeftBuffer, lowerRightArray, lowerRightBuffer;
glm::vec3 cameraPos = glm::vec3(0,10,20); 
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

/*GLfloat verts[] = {
	-1.0f, -1.0f, 0.0f, /*pos 1.0f, 0.0f, 0.0f, /*color*/
	/*1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f
};*/

GLfloat verts[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f,  1.0f, 0.0f
};

GLfloat colors[] = {
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f,  0.0f, 0.0f
};

GLuint drawShader, markerShader;
glm::mat4 Projection = glm::perspective(45.0f, 3.f / 3.f, 0.1f, 100.f);
//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) W / (float)H, 0.1f, 300.0f);
glm::mat4 View = glm::lookAt(
    cameraPos, // Camera is at (4,3,3), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
 
glm::mat4 Scale = glm::mat4(1.0f);

glm::mat4 MVP;

void fillColorVector(){
	srand(time(NULL));
	triColors.clear();
	for(uint i=0; i<model.size(); i+=9){
		float random = (float)rand()/RAND_MAX;
		triColors.push_back(random);
		triColors.push_back(0.0f);
		triColors.push_back(0.0f);
		triColors.push_back(random);
		triColors.push_back(0.0f);
		triColors.push_back(0.0f);
		triColors.push_back(random);
		triColors.push_back(0.0f);
		triColors.push_back(0.0f);
	}
}

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
	//glBindVertexArray(colorArray);
	glEnableVertexAttribArray(1);

	glUniformMatrix4fv(glGetUniformLocation(drawShader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	//glUniform2f(glGetUniformLocation(drawShader, "upperLeft"), strut.x, strut.y);
	//glUniform2f(glGetUniformLocation(drawShader, "lowerRight"), strut2.x, strut2.y);
	
	glDrawArrays(GL_TRIANGLES, 0, model.size()/3);
	//glDrawArrays(GL_TRIANGLES, 0, 15);
	//glDrawArrays(GL_TRIANGLES, 0, 3);	
	glDisableVertexAttribArray(1);
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
	glViewport(0, 0, sW, sH);
	Projection = glm::perspective(45.0f, 3.f / 3.f, 0.1f, 100.f);
}

// This function is called whenever the computer is idle
// As soon as the machine is idle, ask GLUT to trigger rendering of a new
// frame
void idle()
{
  glutPostRedisplay();
}

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
	//strut = MVP * glm::vec4(upperLeft[0], upperLeft[1], 0.0f, 1.0f);
	//strut2 = MVP * glm::vec4(lowerRight[0], lowerRight[1], 0.0f, 1.0f);
}

void mouseHoldFunction(int x, int y){
	
		lowerRight[0] = (float)x/sW*2.0f-1.0f;
		lowerRight[1] = -1.0f*((float)y/sH*2.0f-1.0f);
		//cout<<"x: "<<to_string(lowerRight[0])<<endl;
		//cout<<"y: "<<to_string(lowerRight[1])<<endl;
		changeMarker();
		
}

void changeColorBuffer(int startIndex){
	triColors[startIndex+2] = (triColors[startIndex+2] == 0.0f) ? 1.0f : 0.0f;
	triColors[startIndex+5] = (triColors[startIndex+5] == 0.0f) ? 1.0f : 0.0f;
	triColors[startIndex+8] = (triColors[startIndex+8] == 0.0f) ? 1.0f : 0.0f;
}
//ide:
//ta rektangel från markeringen och transformera till världskoordinater
//testa varje punkt i triangel och se om den ligger inuti
bool rectangleIntersection(vec3 v0, vec3 v1, vec3 v2){
	cout<<"Working with ("<<upperLeft[0] << "," <<upperLeft[1] <<")\n("<<lowerRight[0] << "," <<lowerRight[1] <<")\n";
	vec4 minCorner = vec4(upperLeft[0], upperLeft[1], 0.0f, 1.0f);
	vec4 maxCorner = vec4(lowerRight[0], lowerRight[1], 0.0f, 1.0f);
	vec4 minEye = inverse(Projection)*minCorner;
	vec4 maxEye = inverse(Projection)*maxCorner;
	return false;
} 

void testIntersection(float x, float y){
	
	bool intersection = true;
	float EPSILON = 0.00001f, u, v, det, inv_det;
	//glm::vec3 dir = glm::vehttps://github.com/HerrLingvistik/meshpickc3(0.0f,0.0f,1.0f);
	//glm::vec3 origin = glm::vec3(x,y,10); 

	//glm::vec3 dir_clip = glm::vec3(glm::inverse(MVP)*glm::vec4(x,y,-1.0f,1.0f));
	glm::vec4 dir_clip = glm::vec4(x,y,-1.0f,1.0f);	
	glm::vec4 dir_eye = glm::inverse(Projection)*dir_clip;
	//Detta är en vektor då ska den ha 0.0	
	dir_eye = glm::vec4(glm::vec2(dir_eye), -1.0, 0.0);
	glm::vec3 dir_world = glm::vec3(glm::inverse(View)*dir_eye);	
	//glm::vec3 origin = glm::vec3(glm::inverse(MVP)*glm::vec4(x,y,10,1.0f));
	glm::vec3 origin =  cameraPos;//glm::vec3(0,0,10);

	bool intersect = rectangleIntersection(vec3(0,0,0), vec3(0.1f,0,0), vec3(0,0.1f,0));
	//Gör nedanstående för alla trianglar och ändra färg vid intersection
	for(uint i=0; i<model.size(); i+=9){
		intersection = true;
		/*glm::vec3 v0 = glm::vec3(verts[0], verts[1], verts[2]);
		glm::vec3 v1 = glm::vec3(verts[3], verts[4], verts[5]);
		glm::vec3 v2 = glm::vec3(verts[6], verts[7], verts[8]);*/
		
		glm::vec3 v0 = glm::vec3(model[i], model[i+1], model[i+2]);
		glm::vec3 v1 = glm::vec3(model[i+3], model[i+4], model[i+5]);
		glm::vec3 v2 = glm::vec3(model[i+6], model[i+7], model[i+8]);
		//glm::vec3 V1 = glm::vec3(verts[3]-verts[0], verts[4]-verts[1], verts[5]-verts[2]);
		glm::vec3 V1 = v1-v0;
		glm::vec3 V2 = v2-v0;
		//cout<<glm::to_string(V1)<<endl;
		//cout<<glm::to_string(V2)<<endl;
		glm::vec3 P = glm::cross(dir_world,V2);
		det = glm::dot(V1,P);

		if(det > -EPSILON && det < EPSILON){ 
			intersection = false;
			continue;
		}
		inv_det = 1.0f/det;

		glm::vec3 T = origin - v0;//glm::vec3(origin.x-verts[0], origin.y-verts[1], origin.z-verts[2]);
		u = glm::dot(T,P)*inv_det;

		if(u < 0.0f || u>1.0f){ 
			intersection = false;
			continue;
		}
		//Prepare to test v parameter
		glm::vec3 Q = glm::cross(T, V1);

		//Calculate V parameter and test bound
		v = glm::dot(dir_world, Q) * inv_det;
		//The intersection lies outside of the triangle
		if(v < 0.f || (u + v)  > 1.f){ 
			intersection = false;
			continue;
		}

		float t = glm::dot(V2, Q) * inv_det;

		if(t > EPSILON && intersection != false) { //ray intersection
			changeColorBuffer(i);
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*triColors.size(), &triColors[0], GL_STATIC_DRAW);
			glutSetWindowTitle("YEAS!");
		  intersection = true;
		}else{
			//changeColorBuffer(0);
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*triColors.size(), &triColors[0], GL_STATIC_DRAW);
		}
	//glm::vec3 inte = glm::vec3(origin.x+dir_world.x*t, origin.y+dir_world.y*t, origin.z + dir_world.z*t);
	}
}

void mouseFunction(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		upperLeft[0] = (float)x/sW*2.0f-1.0f;
		upperLeft[1] = -1.0f*((float)y/sH*2.0f-1.0f);
		//testIntersection( (float)x/sW*2.0f-1.0f, -1.0f*((float)y/sH*2.0f-1.0f));
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		testIntersection( (float)x/sW*2.0f-1.0f, -1.0f*((float)y/sH*2.0f-1.0f));
	}
}

void init(){
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(sW,sH);
	glutInitContextVersion(3, 3);	
	glutCreateWindow("Cruising down the street in my 64!");
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMotionFunc(mouseHoldFunction);	
	glutMouseFunc(mouseFunction);	
	
	drawShader = loadShaders("shaders/draw.vert", "shaders/draw.frag");
	markerShader = loadShaders("shaders/marker.vert", "shaders/marker.frag");

	//model = loadobj("./test.obj");
	model = loadobj("./teapot.obj");
	fillColorVector();
	printf("%zu\n",model.size()/3);	
	/*glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);
	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.size(), &model[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(glGetAttribLocation(drawShader, "inPosition"),3, GL_FLOAT,GL_FALSE,0,0);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "inPosition"),3, GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),0);
	glDisableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "inColor"),3, GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/


	glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);
	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.size(), &model[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "inPosition"),3, GL_FLOAT,GL_FALSE,0,0);
	glDisableVertexAttribArray(0);
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*triColors.size(), &triColors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "inColor"),3, GL_FLOAT,GL_FALSE,0,0);
	glDisableVertexAttribArray(1);
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

	MVP = Projection*View*Scale;
	std::cout<<glm::to_string(MVP)<<std::endl;

}

int main(int argc, char **argv){
	std::cout << "Cruising down the street in my 64!"<<std::endl;
	glutInit(&argc, argv);
	init();
	glutMainLoop();
	exit(0);
}
