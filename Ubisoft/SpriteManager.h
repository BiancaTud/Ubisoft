#include <vector>
#include "Sprite.h"

class SpriteManager
{
private:
	GLuint shader_programme, *vao;
	vector<Sprite> sprites;
	GLFWwindow *window;
	int nr;

public:
	SpriteManager(GLuint shader_programme, int nr, GLFWwindow *window);
	virtual ~SpriteManager();

	void AddSprite(const char * filename, int type);
	void DelSprite(int index);
	//Sprite GetSprite(int nr);
	void Init();
	void Draw();
};