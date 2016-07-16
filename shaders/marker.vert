#version 330

in vec3 inPosition;
//Translation*rotation*scale
void main(void){
		gl_Position = vec4(inPosition, 1.0);
}
