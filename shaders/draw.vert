#version 330

in vec3 inPosition;
out vec3 passPosition;
uniform mat4 MVP;
//Translation*rotation*scale
void main(void){
		passPosition = inPosition;
		gl_Position = MVP * vec4(inPosition, 1.0);
}
