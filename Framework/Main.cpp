#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#include "common.h"
#include "Utils.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "Collision.h"
#include<iostream>
#include<vector>

// variabila in care tinem fereastra
GLFWwindow* window = NULL;


// WIDTH-ul si HEIGHT-ul initial
#define WWIDTH 1024
#define WHEIGHT 768

// width-ul si height-ul ferestrei  (chiar si dupa ce si-a primit resize)
int g_gl_width	= WWIDTH;
int g_gl_height = WHEIGHT;


bool semafor = true;



void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
	g_gl_width = width;
	g_gl_height = height;

}

void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, void* userParam)
{
	printf("%s", message);
	__debugbreak();
}

//draft pentru inamic
struct EnemyTest
{
	CSprite*				m_Sprite;

	// Constructor
	EnemyTest() :m_Sprite(NULL){}

	// Initializare
	void Init(CSprite *spr)
	{
		m_Sprite = spr;

		glm::vec3 pz = m_Sprite->GetPosition();
		m_Sprite->PlayAnimation("BugEye_Idle");
	}


	void Update(float dt)
	{
		//m_Sprite->SetPosition(glm::vec3(m_Body->position.x, m_Body->position.y, -10.0f));

	}

	~EnemyTest()
	{

	}

};






// Clasa cu care testam functionalitatea introdusa (draft de Player)
struct PlayerTest
{
	// viteza aleasa empiric
	#define PLAYER_MOVE_SPEED 2.0f
	
	// Enum cu toate tipurile de animatii ale player-ului
	enum EAnimationTypePlayer
	{
		NONE,
		IDLE,
		RIGHT,
		LEFT
	};

	// Sprite-ul cu care se deseneaza player-ul
	CSprite*				m_Sprite;
	// tipul curent de animatie (animatia 
	EAnimationTypePlayer	m_CurrentAnimType;

	// Constructor
	PlayerTest() :m_Sprite(NULL), m_CurrentAnimType(NONE){}
	
	// Initializare
	void Init(CSprite *spr)
	{
		m_Sprite = spr;
	}
	
	// In functia de update:
	// 1.Detectam directia de deplasare a player-ului in functie de tastele apasate
	// 2.Determinam animatia ce trebuie afisata
	// 3.Limitam din topor spatiul in care se poate deplasa player-ul
	void Update(float dt)
	{

		glm::vec3 pos;
		glm::vec3 dir(0.0f, 0.0f, 0.0f);
		bool recalculate_position = false;

		// Determinarea animatiilor si a directiei de deplasare
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT))
		{
			if (m_CurrentAnimType != LEFT)
			{
				m_Sprite->PlayAnimation("Left");
				m_CurrentAnimType = LEFT;
			}

			dir.x = -1.0f;
			recalculate_position = true;

		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			if (m_CurrentAnimType != RIGHT)
			{
				m_Sprite->PlayAnimation("Right");
				m_CurrentAnimType = RIGHT;
			}

			dir.x = 1.0f;
			recalculate_position = true;
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP))
		{
			if (m_CurrentAnimType != IDLE)
			{
				m_Sprite->PlayAnimation("Idle");
				m_CurrentAnimType = IDLE;
			}

			dir.y = 1.0f;
			recalculate_position = true;

		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN))
		{
			if (m_CurrentAnimType != IDLE)
			{
				m_Sprite->PlayAnimation("Idle");
				m_CurrentAnimType = IDLE;
			}

			dir.y = -1.0f;
			recalculate_position = true;
		}

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE) && semafor) {
			CSpriteManager::Get()->projectiles.push_back(CSpriteManager::Get()->AddSprite("projectile.png", -1));
			CSpriteManager::Get()->projectiles[CSpriteManager::Get()->projectiles.size() - 1]->
				SetPosition(glm::vec3(m_Sprite->GetPosition().x, m_Sprite->GetPosition().y+0.8f, -14));
			semafor = false;
		}

		if (GLFW_RELEASE == glfwGetKey(window, GLFW_KEY_SPACE) && !semafor) {
			semafor = true;
		}



		pos = m_Sprite->GetPosition();

		if (recalculate_position)
		{
		//	cout <<"initial "<< dir.x << " " << dir.y << " " << dir.z << endl;
			dir = glm::normalize(dir);
			//cout << dir.x << " " << dir.y << " " << dir.z<<endl;
			pos = pos + dir * PLAYER_MOVE_SPEED * dt;
		}
		else
		{
			if (m_CurrentAnimType != IDLE)
			{
				m_Sprite->PlayAnimation("Idle");
				m_CurrentAnimType = IDLE;
			}

		}
		// END



		// Limitarea playerului 
		if (pos.x < SCREEN_LEFT)
		{
			pos.x = SCREEN_LEFT + 0.01f;
		}
		if (pos.x  > SCREEN_RIGHT - 0.4f)
		{
			pos.x = SCREEN_RIGHT - 0.4f;
		}
		if (pos.y < SCREEN_BOTTOM)
		{
			pos.y = SCREEN_BOTTOM + 0.01f;
		}
		if (pos.y > SCREEN_TOP - 1.0f)
		{
			pos.y = SCREEN_TOP - 1.0f;
		}
		// END

		// update pozitia spriteului
		m_Sprite->SetPosition(pos);

	}


};



int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	glfwInit();
	
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(WWIDTH, WHEIGHT, "GL Framework", NULL, NULL);
	glfwSetWindowSizeCallback(window, glfw_window_size_callback);
	
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, g_gl_width, g_gl_height);	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);


	// Initializam spriteManagerul
	CSpriteManager::Get()->Init();

	// Initializam PlayerEntity si creem sprite-ul asociat
	PlayerTest PlayerEntity;


	//CSprite *projectile = CSpriteManager::Get()->AddSprite("projectile.png", -1);
	CSprite *back = CSpriteManager::Get()->AddSprite("Background.png",2);
	CSprite *playerSprite = CSpriteManager::Get()->AddSprite("player0000.png", 0);
//	CSprite *RandomRocket = CSpriteManager::Get()->AddSprite("PlayerRocket.png", 1);
	float x = -1;
//	RandomRocket->SetPosition(glm::vec3(-4.0f, 1.0f, -12));
	
	back->SetPosition(glm::vec3(-4, -4, -15));

	assert(playerSprite);
	PlayerEntity.Init(playerSprite);

	EnemyTest vec[100];
	int number_of_enemy = 5;
	//vector de inamici
	vector<CSprite*> enemies;
	for (int i = 0; i< number_of_enemy; i++)
	{
		CSprite *RandomRocket = CSpriteManager::Get()->AddSprite("PlayerRocket.png",1);
		RandomRocket->SetPosition(glm::vec3((i - number_of_enemy / 2.0f) * 2.0, 1.5, -11));
		vec[i].Init(RandomRocket);
		enemies.push_back(RandomRocket);


	}



	float t = 0.0f;
	float dt = 0.01f;

	float prevTime =(float) glfwGetTime();
	float accumulator = 0.0f;

	Collision *collide = new Collision();
	

	bool hit = false;
	bool hit2 = false;
	int i;
	while (!glfwWindowShouldClose(window))
	{

		_update_fps_counter(window);

		float newTime =(float) glfwGetTime();
		float deltaTime = (float)(newTime - prevTime);
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);


		if (deltaTime > 0.25f)
			deltaTime = 0.25f;
		prevTime = newTime;

		accumulator += deltaTime;

		while (accumulator >= dt)
		{
			// update logic pentru player
			PlayerEntity.Update(dt);
			// update animatie
			CSpriteManager::Get()->Update(dt);
			t += dt;
			accumulator -= dt;
		}

		// desenare
		CSpriteManager::Get()->Draw();

		//verificare coliziune intre player si inamic
		for ( i = 0; i< number_of_enemy; i++)
		{
			collide->ResolveCollision(playerSprite,enemies[i]);
		}
		//collide->ResolveCollision(playerSprite, RandomRocket);

		//verificare coliziune intre inamic si proiectilul player-ului
	/*	for (i = 0; i < CSpriteManager::Get()->projectiles.size(); i++){

			hit2 = collide->CheckCollision(CSpriteManager::Get()->projectiles[i], RandomRocket);
			if (hit2 == true)
				break;

		}

		if ( hit2 == true){
			CSpriteManager::Get()->RemoveSprite(CSpriteManager::Get()->projectiles[i]->index);
			CSpriteManager::Get()->projectiles.erase(CSpriteManager::Get()->projectiles.begin() + i);
			hit2 = false;
		}*/

		glfwPollEvents();
		glfwSwapBuffers(window);

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}

	}

	delete collide;
	glfwTerminate();
	return 0;
}