#include "common.h"
#include "Sprite.h"
#include "SpriteManager.h"




void printMat(glm::mat4  mat){
	int i, j;
	for (j = 0; j<4; j++){
		for (i = 0; i<4; i++){
			printf("%f ", mat[i][j]);
		}
		printf("\n");
	}
}

CSprite::CSprite()
{
	m_ShaderProgram					= -1;
	m_VertexAttributeObjectID		= -1;
	m_PositionVertexBufferID		= -1;
	m_TextureCoordinatesBufferID	= -1;
	m_IndexBufferID					= -1;
	model_mat = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	range = 0;
	pas = 0.1f;
	life = 3;
	speed = 0.0f;
}


void CSprite::SetType(int type){

	this->type = type;

	if (type == 0)
		speed = 2.0f;
	else if (type == -1)
		speed = 12;
	else if (type == 1)
		speed =5.0f;

}


void CSprite::SetPosition(glm::vec3 p){

	m_Position = p;
	model_mat = glm::translate(p);
	

}

void CSprite::Init(GLuint shader_programme, float *texture_coordinates, float aspect_ratio)
{
	m_AspectRatio = aspect_ratio;



	GLfloat Vertices[] = {	0.0f,			0.0f,	0.0f,
							m_AspectRatio,	0.0f,	0.0f,
							m_AspectRatio,	1.0f ,	0.0f,
							0.0f,			1.0f ,	0.0f };

	//pentru proiectil
	GLfloat VerticesProj[] = { 0, 0, 0.0f,
		m_AspectRatio / 3, 0, 0.0f,
		m_AspectRatio / 3, 1.0f / 3, 0.0f,
		0, 1.0f / 3, 0.0f };

	//pentru background
	float x = SCREEN_RIGHT - SCREEN_LEFT;
	float y = SCREEN_TOP - SCREEN_BOTTOM;

	GLfloat VerticesBack[] = { 0.0f, 0.0f, 0.0f,
		m_AspectRatio*x, 0.0f, 0.0f,
		m_AspectRatio*x, 1.0f*y, 0.0f,
		0.0f, 1.0f*y, 0.0f

	};

	GLubyte indices[] = {	0, 1, 2, // first triangle (bottom left - top left - top right)
							0, 2, 3 }; // second triangle (bottom left - top right - bottom right)



	
	glGenBuffers(1, &m_PositionVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_PositionVertexBufferID);
	if (type == 0 || type==1){
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof (float), Vertices, GL_STATIC_DRAW);
	}
	else if (type==2){

		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof (float), VerticesBack, GL_STATIC_DRAW);
	}
	else if (type == -1){

		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof (float), VerticesProj, GL_STATIC_DRAW);

	}

	glGenBuffers(1, &m_TextureCoordinatesBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureCoordinatesBufferID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof (float), texture_coordinates, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned char), &indices, GL_STATIC_DRAW);

	m_VertexAttributeObjectID = 0;
	glGenVertexArrays(1, &m_VertexAttributeObjectID);
	glBindVertexArray(m_VertexAttributeObjectID);

	glEnableVertexAttribArray(POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, m_PositionVertexBufferID);
	glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(TEXTURE_COORDINATE_0);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureCoordinatesBufferID);
	glVertexAttribPointer(TEXTURE_COORDINATE_0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);

	
	m_ShaderProgram		= shader_programme;
	
	m_ModelMatrix_loc	= glGetUniformLocation(m_ShaderProgram, "u_transfMatrix");

	m_Position.x = m_Position.y=0.0f;
	if (type == 0){
		m_Position.z = -10.0f;
	}
}

void	CSprite::PlayAnimation(const char * name)
{
	// setam animatia curenta
	m_CurrentAnimation.currentAnimationTemplate = CSpriteManager::Get()->GetAnimation(name);
	m_CurrentAnimation.currentAnimationPlayTime = 0.0f;
	m_CurrentAnimation.currentFrame = m_CurrentAnimation.currentAnimationTemplate->m_BeginFrame;
	assert(m_CurrentAnimation.currentAnimationTemplate);
}

void CSprite::UpdateAnimation(float dt)
{
	// determinam frame-ul curent pentru animatie in functie de timp
	// daca timpul animatiei a trecut, o luam de la inceput
	if (!m_CurrentAnimation.currentAnimationTemplate){
		//pentru racheta update
		if (type == 1){
			if (range<10.5f){
				SetPosition(glm::vec3( m_Position.x + speed*dt, m_Position.y, m_Position.z));
				range += abs(speed*dt);
			}
			else{
				range = 0;
				speed *= -1;
			}
		}


		return;
	}

	/*if (type == 1){
		if (range<8.5f){
			SetPosition(glm::vec3(m_Position.x + speed*dt, m_Position.y, m_Position.z));
			range += abs(speed*dt);
		}
		else{
			range = 0;
			speed *= -1;
		}
	}*/


	m_CurrentAnimation.currentAnimationPlayTime += dt;
	float anim_len =   (float)(m_CurrentAnimation.currentAnimationTemplate->m_EndFrame - m_CurrentAnimation.currentAnimationTemplate->m_BeginFrame) ;
	float time_len =	anim_len* 1.0f/m_CurrentAnimation.currentAnimationTemplate->m_Speed;
	if(m_CurrentAnimation.currentAnimationPlayTime >= time_len) // loop animation
	{
		m_CurrentAnimation.currentAnimationPlayTime  = 0.0f;
	}
	m_CurrentAnimation.currentFrame = (int)floor((float)m_CurrentAnimation.currentAnimationTemplate->m_BeginFrame + 
												 (float)m_CurrentAnimation.currentAnimationPlayTime/(1.0f/m_CurrentAnimation.currentAnimationTemplate->m_Speed));
}

void CSprite::Draw()
{	

	glUseProgram(m_ShaderProgram);
	glBindVertexArray(m_VertexAttributeObjectID);

	glActiveTexture(GL_TEXTURE0);
	// Daca nu avem animatie, punem textura cu care a fost creat sprite-ul
	if(!m_CurrentAnimation.currentAnimationTemplate)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}
	else
	{	
		//daca avem animatie luam subtextura de la indexul calculat in UpdateAnimation
		// recalculam uv-urile si le retrimitem la placa video
		SDictEntry * entry =  CSpriteManager::Get()->GetSubTextureAt(m_CurrentAnimation.currentFrame);
		glm::vec4 &TexOff = entry->m_TextureOffsets;
		float uv[8];
		uv[0] = TexOff.x;					uv[1] =  1.0f-(TexOff.y + TexOff.w);
		uv[2] = TexOff.x + TexOff.z;		uv[3] =  1.0f-(TexOff.y + TexOff.w) ;
		uv[4] = TexOff.x + TexOff.z;		uv[5] =  1.0f-TexOff.y;
		uv[6] = TexOff.x;					uv[7] =  1.0f-TexOff.y;
		
	
		glBindTexture(GL_TEXTURE_2D, entry->m_RefTexture->m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_TextureCoordinatesBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof (uv), uv, GL_STATIC_DRAW);
	
	}




	model_mat = glm::translate(m_Position);

	/*if (type == 0){
		printMat(model_mat);
	}*/


	// trimitem ModelMatrix la shader (pentru deplasarea vertecsilor)
	glUniformMatrix4fv(m_ModelMatrix_loc, 1, GL_FALSE, glm::value_ptr(model_mat));

	// desenam obiectul
	glDrawElements(
		GL_TRIANGLES,		 // mode
		6,					// count
		GL_UNSIGNED_BYTE,   // type
		(void*)0			// element array buffer offset
		);
}

CSprite::~CSprite()
{

}