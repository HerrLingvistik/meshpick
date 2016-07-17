#version 330

in vec3 inPosition;
in vec3 inColor;
out vec3 passColor;
out vec3 passPosition;
uniform mat4 MVP;
//Translation*rotation*scale
void main(void){
		passPosition = inPosition;
		passColor = inColor;
		gl_Position = MVP * vec4(inPosition, 1.0);
}
