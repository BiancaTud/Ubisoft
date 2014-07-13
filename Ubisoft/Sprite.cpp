#include "stdafx.h"
#include "Sprite.h"
#include <stb_image\stb_image.cpp>




Sprite::Sprite(int type,const char *filename, int life){

	this->filename=filename;
	this->type=type;
	this->life=life;
	view_mat = proj_mat = model_mat = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

}


// flip image
void FlipTexture(unsigned char* image_data,int x,int y , int n)
{
	//flip texture
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
}


void Sprite::LoadTexture(){

	int x, y, n, force_channels = 4;
	unsigned char* image_data;
	tex = 0;
	glGenTextures(1, &tex);
	cout<<filename<<" ";
	image_data = stbi_load(filename, &x, &y, &n, force_channels);
	FlipTexture(image_data, x, y, n);

	glBindTexture(GL_TEXTURE_2D, tex);
	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			x,
			y,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image_data
		);

	// setam parametri de sampling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //ce se intampla cand coordonata nu se inscrie in limite
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //ce se intampla cand coordonata nu se inscrie in limite
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // setam samplare cu interpolare liniara
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // setam samplare cu interpolare liniara

	//activare transparenta
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	delete image_data;

}


void Sprite::Init(GLuint shader_programme){

	this->shader_programme=shader_programme;
	float vertex_square[] = {
			0.0f, 0.0f, 0.0f,
			-0.3f, 0.0f, 0.0f, 
			-0.3f, 0.3f, 0.0f, 
			0.0f, 0.3f, 0.0f
	  };

	float uv[] ={
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
  };

	//array of indices for square
	unsigned int indices [] = { 0, 1, 2, 0, 2, 3};

	vbo=0;
	glGenBuffers(1, &vbo); // generam un buffer 
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // setam bufferul generat ca bufferul curent 
	glBufferData(GL_ARRAY_BUFFER,  sizeof(vertex_square), vertex_square, GL_STATIC_DRAW);

	tex_buff=0;
    glGenBuffers(1, &tex_buff);
	glBindBuffer(GL_ARRAY_BUFFER, tex_buff);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), uv, GL_STATIC_DRAW);

	elementbuffer=0;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tex_buff);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

}

void Sprite::Draw(){

	glUniformMatrix4fv(glGetUniformLocation(shader_programme,"u_model_matrix"),1,false, glm::value_ptr(model_mat));
	glUniformMatrix4fv(glGetUniformLocation(shader_programme,"u_transfMatrix"),1,false, glm::value_ptr(view_mat));
	glUniformMatrix4fv(glGetUniformLocation(shader_programme,"u_proj_matrix"),1,false,glm::value_ptr(proj_mat));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUseProgram(shader_programme);
	//glBindVertexArray(vbo);
	// Index buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//glEnableVertexAttribArray(0);
	glDrawElements(
		GL_TRIANGLES,      // mode
		6,    // count
		GL_UNSIGNED_INT,   // type
		NULL         // element array buffer offset
		);

	if(type==1 ||  type==2){
		float angle=0.12;
		model_mat = glm::rotate(model_mat ,angle, glm::vec3(0 , 0 ,0.1));
	}
	else if(type==3)
		model_mat = glm::translate(model_mat , glm::vec3(0.0001 , 0 , 0));

}

void Sprite::setLife(int life){
	this->life=life;
}

int Sprite::getLife(){
	return life;
}

int Sprite::getType(){
	return type;
}

void Sprite::setPosition(int type){

	if (type==0)
		model_mat = glm::translate(model_mat , glm::vec3(0.1 , -0.9 , 0));
	else if(type==1){
		model_mat = glm::translate(model_mat , glm::vec3(-0.5 , 0 , 0));
	}
	else if(type==2)
		model_mat = glm::translate(model_mat , glm::vec3(0.5 , 0 , 0));
	else
		model_mat = glm::translate(model_mat , glm::vec3(0 , 0.5 , 0));

}

void Sprite::movePlayer(float tx, float ty){

	model_mat = glm::translate(model_mat, glm::vec3(tx, ty , 0.0));

}


void Sprite::onKey(GLFWwindow* window){

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
				movePlayer(-0.0007f, 0.0f);
		}

		//deplasare dreapta
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
				movePlayer(0.0007f, 0.0f);
		}

		//deplasare sus
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
				movePlayer(0.0f, 0.0007f);
		}

		//deplasare jos
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
				movePlayer(0.0f, -0.0007f);
		}

}


Sprite::~Sprite()
{
	glDeleteBuffers(1, &vbo);
}