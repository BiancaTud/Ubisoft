#version 400 // 150 daca nu e suportat opengl 4.x dar e suportat 3.3
out vec4 color;
in vec2 Texcoord;

uniform sampler2D tex;

void main(){
	color = texture(tex, Texcoord);
}