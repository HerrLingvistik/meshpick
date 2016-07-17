#version 330

in vec3 passPosition;

in vec3 passColor;
uniform vec2 upperLeft;
uniform vec2 lowerRight;
out vec4 outColor;

void main(void){
	
	outColor = vec4(passColor,1.0);
	//outColor = vec4(1.0,0.0,0.0,1.0);
}
