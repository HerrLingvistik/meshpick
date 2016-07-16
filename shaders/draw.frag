#version 330

in vec3 passPosition;
//in vec3 upper_Left;
//in vec3 lower_Right;
uniform vec2 upperLeft;
uniform vec2 lowerRight;
out vec4 outColor;

void main(void){
	if(passPosition.x < lowerRight.x){
		outColor = vec4(0.0,1.0,0.0,1.0);
	}
	else{
		outColor = vec4(1.0,0.0,0.0,1.0);
	}
}
