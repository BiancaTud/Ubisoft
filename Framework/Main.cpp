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
vector<CSprite*> lives;


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
	vector<CSprite*> enemy_projectiles;
	int iSecret;

	// Constructor
	EnemyTest() :m_Sprite(NULL){}

	// Initializare
	void Init(CSprite *spr)
	{
		m_Sprite = spr;

		glm::vec3 pz = m_Sprite->GetPosition();
		m_Sprite->PlayAnimation("BugEye_Idle");

	}

	//coliziune intre player si proiectile inamic
	void Collide(CSprite *playerSprite, Collision *collide){
		int pivot = 0;

		for (int i = 0; i < enemy_projectiles.size(); i++){
			collide->ResolveCollisionPlayer(playerSprite, enemy_projectiles[i]);
			if (collide->getStatusPlayer() == true){
				CSpriteManager::Get()->RemoveSprite(enemy_projectiles[i]->index - pivot);
				enemy_projectiles.erase(enemy_projectiles.begin() + i - pivot);
				pivot++;
				//stergere vieti
				if (playerSprite->life >= 0){
					CSpriteManager::Get()->RemoveSprite(lives[lives.size() - 1]->index);
					lives.erase(lives.end() - 1);

				}
			}
			
		}


	}

	//update proiectile inamici
	void Update(float dt, int &nr)
	{	
		glm::vec3 posProj;
		iSecret = rand() % 500 + 1;
		if ((int)(dt+iSecret) % 200 == 0 && enemy_projectiles.size()<4){
			enemy_projectiles.push_back(CSpriteManager::Get()->AddSprite("bugproj.png", enemy_proj));
			enemy_projectiles[enemy_projectiles.size() - 1]->
				SetPosition(glm::vec3(m_Sprite->GetPosition().x+0.235f, m_Sprite->GetPosition().y - 0.2f, -13));
		}

		int pivot = 0;
		
		for (int i = 0; i < enemy_projectiles.size(); i++){

			//daca depasesc ecranul sunt sterse
			
			if (enemy_projectiles[i]->GetPosition().y < SCREEN_BOTTOM + 0.5f)
			{

				CSpriteManager::Get()->RemoveSprite(enemy_projectiles[i]->index-pivot-nr);
				enemy_projectiles.erase(enemy_projectiles.begin() + i-pivot);
				nr++;
				pivot++;
			}

			//altfel este updatata pozitia lor
			else{

				posProj = enemy_projectiles[i]->GetPosition();
				enemy_projectiles[i]->SetPosition(glm::vec3(posProj.x,
					posProj.y - dt*enemy_projectiles[i]->speed, posProj.z));
			}
			
		}
		

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
			CSpriteManager::Get()->projectiles.push_back(CSpriteManager::Get()->AddSprite("projectile.png", player_proj));
			CSpriteManager::Get()->projectiles[CSpriteManager::Get()->projectiles.size() - 1]->SetPosition(glm::vec3(m_Sprite->GetPosition().x, m_Sprite->GetPosition().y+0.8f, -14));
			semafor = false;
		}

		if (GLFW_RELEASE == glfwGetKey(window, GLFW_KEY_SPACE) && !semafor) {
			semafor = true;
		}



		pos = m_Sprite->GetPosition();

		if (recalculate_position)
		{
			dir = glm::normalize(dir);
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


	CSprite *back = CSpriteManager::Get()->AddSprite("sky0000.png", background);

	
	for (int k = 0; k < 3; k++){
		lives.push_back(CSpriteManager::Get()->AddSprite("mini.png", mini));
		lives[lives.size() - 1]->SetPosition(glm::vec3(3 + k*0.3f, 3.4f, -19));
	}

	CSprite* lifeSprite = CSpriteManager::Get()->AddSprite("life.png", textLife);
	lifeSprite->SetPosition(glm::vec3(3 - 0.6f, 3.4f, -19));



	CSprite *playerSprite = CSpriteManager::Get()->AddSprite("player0000.png", player);
	
	back->SetPosition(glm::vec3(-4, -4, -20));

	assert(playerSprite);
	PlayerEntity.Init(playerSprite);
//	back->PlayAnimation("Sky");

	EnemyTest vec[100];
	int number_of_enemy =5;
	//vector de inamici
	vector<CSprite*> enemies;
	float x,y;
	for (int i = 0; i< number_of_enemy; i++)
	{
		
		CSprite *RandomRocket = CSpriteManager::Get()->AddSprite("PlayerRocket.png",enemy);
		x = -3.0f - i;
		y = RandomRocket->getFirstParabolaY(x);
		RandomRocket->SetPosition(glm::vec3(x, y, -11));
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

			//update enemies
			int nr = 0;
			for (int i = 0; i < number_of_enemy; i++)
			{
				if (enemies[i]->life>0){
					vec[i].Update(dt, nr);
				}
			}
			t += dt;
			accumulator -= dt;
		}

		// desenare
		CSpriteManager::Get()->Draw();

		//verificare coliziune intre player si inamic
		for ( i = 0; i< number_of_enemy; i++)
		{
			collide->ResolveCollisionPlayer(playerSprite,enemies[i]);
			if (collide->getStatusPlayer() == true){
				if (playerSprite->life>=0){
					CSpriteManager::Get()->RemoveSprite(lives[lives.size() - 1]->index);
					lives.erase(lives.end() - 1);
				}
				

			}



			//verificare coliziune intre player si proiectile inamici
			vec[i].Collide(playerSprite, collide);

			if (playerSprite->life == 0){
				playerSprite->SetPosition(glm::vec3(-5.0f, -5.0f, playerSprite->GetPosition().z));
			}

			if (enemies[i]->life == 0){
				enemies[i]->life = 2;
			}

		}


		//verificare coliziune intre inamic si proiectilul player-ului
		collide->ResolveCollisionEnemy(CSpriteManager::Get()->projectiles, enemies );

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