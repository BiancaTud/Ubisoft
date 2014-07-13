#include "stdafx.h"


class Sprite
{

private:
		GLuint vbo, tex, tex_buff, shader_programme, elementbuffer;
		const char * filename;
		int type, life;
		float cont, pas;
		glm::mat4 view_mat, proj_mat, model_mat;

public:
		Sprite(int type, const char *filename, int life);
		virtual ~Sprite();

		void LoadTexture();
		void Init(GLuint shader_programme);
		void setPosition(int type);
		void setLife(int nr);
		int getLife();
		int getType();
		void movePlayer(float tx, float ty);
		void onKey(GLFWwindow * window);
		void Draw();

};