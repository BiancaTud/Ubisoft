#include "stdafx.h"
#include "SpriteManager.h"


SpriteManager::SpriteManager(GLuint shader_programme, int nr, GLFWwindow* window)
{
	this->nr=nr;
	this->window=window;
	this->shader_programme = shader_programme;
	this->vao = (GLuint*)malloc(nr * sizeof(GLuint));
	glGenVertexArrays(nr, &vao[0]);

}


SpriteManager::~SpriteManager(){
	free(vao); //163
	glDeleteProgram(shader_programme);
}


void SpriteManager::AddSprite(const char *filename, int type)
{
	Sprite *sprite = new Sprite(type, filename,3);
	sprite->setPosition(type);
	sprites.push_back(*sprite);
	delete sprite;
}

void SpriteManager::DelSprite(int index)
{
	sprites[index].setLife(0);
}



void SpriteManager::Init()
{
	for (int i = 0; i < nr; i++) {
		if(sprites[i].getLife()!=0){
			glBindVertexArray(vao[i]);
			sprites[i].LoadTexture();
			sprites[i].Init(shader_programme);
		}

	}
}

void SpriteManager::Draw()
{
	for (int i = 0; i <nr; i++) {
		if(sprites[i].getLife()!=0){
			glBindVertexArray(vao[i]);
			if(sprites[i].getType()==0)
				sprites[i].onKey(window);
			sprites[i].Draw();
		}
	}
}